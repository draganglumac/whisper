/*
 * session_service_auth_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxencoder.h>
#include "auth_comms.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "../crypto/cryptography.h"
#include "../util/utils.h"
#include "../session/auth_initiator.pb-c.h"
#include "../session/auth_receiver.pb-c.h"
#include "../session/handshake_control.h"
#define CHALLENGE_REQUEST_PUBLIC_KEY 1
#define CHALLENGE_REQUEST_FINISH 0

typedef struct transport_options {
  discovery_service *ds;
  session_service *ss;
  auth_comms_service *ac;
}transport_options;
/* Listener Thread */
static void send_data(jnx_char *hostname, jnx_char *port,
    unsigned int family,
    jnx_uint8 *buffer, jnx_int bytes) {
  jnx_socket *sock = jnx_socket_tcp_create(family);
  jnx_socket_tcp_send(sock,hostname,port,buffer,bytes);
  jnx_socket_destroy(&sock);
}
static jnx_uint8 *send_data_await_reply(jnx_char *hostname, jnx_char *port,
    unsigned int family, jnx_uint8 *buffer, jnx_int bytes, jnx_size *receipt_bytes) {
  jnx_socket *sock = jnx_socket_tcp_create(family);
  jnx_uint8 *reply;
  *receipt_bytes = jnx_socket_tcp_send_with_receipt(sock,hostname,port,buffer,bytes,&reply);
  jnx_socket_destroy(&sock);
  return reply;
}
static void send_stop_packet(auth_comms_service *ac) {
  void *buffer;
  send_data("127.0.0.1",ac->listener_port,ac->listener_family,
      "STOP",5);
}
static jnx_int32 listener_callback(jnx_uint8 *payload,
    jnx_size bytes_read, jnx_socket *s,int connected_socket, void *context) {

  transport_options *t = (transport_options*)context;
  void *object;
  if(handshake_did_receive_initiator_request(payload,bytes_read,&object)) {
    AuthInitiator *a = (AuthInitiator*)object;

    /*
     *At this point the receiver does not have a session for PeerA/B
     *We'll need to insert one so the session reference is usable in the ongoing
     *comms
     */
    if(a->is_requesting_public_key && !a->is_requesting_finish){

      printf("Did receive handshake request.\n");
      session *osession;
      session_state e = session_service_create_shared_session(t->ss,
          a->session_guid,&osession);
      /* setting our response key as the 'remote public key' */
      session_add_initiator_public_key(osession,a->initiator_public_key); 

      printf("Generated shared session\n");
      /*
       *Now we have a session on the receiver with a matching GUID to the sender
       *We'll have a valid public key we can send over
       */
      jnx_uint8 *onetbuffer;
      printf("About to generate handshake.\n");
      int bytes = handshake_generate_public_key_response(osession,
          &onetbuffer);
      write(connected_socket,onetbuffer,bytes);
      free(onetbuffer);    
      return 0;
    }
    if(!a->is_requesting_public_key && a->is_requesting_finish){
      printf("Did receive encrypted shared secret.\n"); 
      session *osession;
      jnx_guid g;
      jnx_guid_from_string(a->session_guid,&g);
      session_service_fetch_session(t->ss,
          &g,&osession);

      jnx_uint8 *onetbuffer;
      int bytes = handshake_generate_finish_response(osession,
          &onetbuffer);
      write(connected_socket,onetbuffer,bytes);
      free(onetbuffer);    

      /* The last thing to do is to decrypt the shared secret and store it in
       * the session */

      jnx_size olen;

      jnx_size decoded_len;
      jnx_encoder *encoder = jnx_encoder_create();
      jnx_uint8 *decoded_secret = 
        jnx_encoder_b64_decode(encoder,a->shared_secret,
            strlen(a->shared_secret),&decoded_len);

      jnx_char *decrypted_shared_secret = 
        asymmetrical_decrypt(osession->keypair,decoded_secret,
            decoded_len,
            &olen);

      //DEBUG ONLY
      printf("DEBUG!!! -> Peer B says the shared secret is %s\n", decrypted_shared_secret);
      session_add_shared_secret(osession,decrypted_shared_secret);
      //
      jnx_encoder_destroy(&encoder);
    }
  } 

  char command[5];
  bzero(command,5);
  memcpy(command,payload,4);
  if(strcmp(command,"STOP") == 0) {
    printf("Stopping auth comms listener...\n");
    return -1;
  }
  return 0;
}
static void *listener_bootstrap(void *args) {
  transport_options *t = (transport_options *)args;
  JNXCHECK(t->ac->listener_socket);
  jnx_socket_tcp_listen_with_context(t->ac->listener_socket,
      t->ac->listener_port,100,listener_callback,t);
  free(t);
}
auth_comms_service *auth_comms_create() {
  return malloc(sizeof(auth_comms_service));
}
void auth_comms_listener_start(auth_comms_service *ac, discovery_service *ds,
    session_service *ss) {
  transport_options *ts = malloc(sizeof(ts));
  ts->ac = ac;
  ts->ds = ds;
  ts->ss = ss;

  ac->listener_thread = jnx_thread_create(listener_bootstrap,ts);
}
void auth_comms_destroy(auth_comms_service **ac) {
  cancel_thread(&(*ac)->listener_thread);
  send_stop_packet(*ac);
  jnx_socket_destroy(&(*ac)->listener_socket);
}
void auth_comms_initiator_start(auth_comms_service *ac, \
    discovery_service *ds, session *s) {

  peer *remote_peer = peerstore_lookup(ds->peers,&(*s).remote_peer_guid);

  jnx_uint8 *obuffer;
  jnx_int bytes_read = handshake_generate_public_key_request(s,&obuffer);

  printf("Generated initial handshake...[%d/bytes]\n",bytes_read);

  jnx_size replysize;
  jnx_uint8 *reply = send_data_await_reply(remote_peer->host_address,ac->listener_port, 
      ac->listener_family,
      obuffer,bytes_read,&replysize);

  /* expect an AuthReceiver public key reply here */
  void *object;
  if(handshake_did_receive_receiver_request(reply,replysize,&object)) {
    AuthReceiver *r = (AuthReceiver *)object;
    printf("Got the public key of peer B => [%s]\n",r->receiver_public_key);
    session_add_receiver_public_key(s,r->receiver_public_key);
    /* At this point we have a session with the receiver public key
       we can generate the shared secret and transmit it back */
    jnx_uint8 *secret;
    jnx_size s_len = generate_shared_secret(&secret);

    session_add_shared_secret(s,secret);

    RSA *remote_pub_keypair = asymmetrical_generate_key_from_public_string(
        r->receiver_public_key); 

    print_public_key(remote_pub_keypair);
    
      jnx_size encrypted_secret_len;
    jnx_char *encrypted_secret = asymmetrical_encrypt(remote_pub_keypair,
        secret, &encrypted_secret_len);

    /* the shared secret needs to encoded */
    jnx_size encoded_len;
    jnx_encoder *encoder = jnx_encoder_create();
    jnx_uint8 *encoded_secret = jnx_encoder_b64_encode(encoder,
        encrypted_secret,encrypted_secret_len,
        &encoded_len);

    jnx_encoder_destroy(&encoder);
    jnx_uint8 *fbuffer;
    bytes_read = handshake_generate_finish_request(s,encoded_secret,encoded_len,&fbuffer);

    jnx_size replysizetwo;
    reply = send_data_await_reply(remote_peer->host_address,ac->listener_port, 
        ac->listener_family,
        fbuffer,bytes_read,&replysizetwo);

    void *finish_object;
    if(handshake_did_receive_receiver_request(reply,replysizetwo,&finish_object)){
      AuthReceiver *r = (AuthReceiver *)object;
      if(r->is_receiving_finish && !r->is_receiving_public_key) {

        printf("Handshake complete.\n");
      }
    }
    free(encrypted_secret);
    free(secret);
    free(obuffer);
  }
}
void auth_comms_receiver_start(auth_comms_service *ac, \
    discovery_service *ds, session *s) {

}

