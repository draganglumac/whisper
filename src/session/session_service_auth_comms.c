/*
 * session_service_auth_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */
#include <jnxc_headers/jnxlog.h>
#include "session_service_auth_comms.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "../crypto/cryptography.h"
#include "../util/utils.h"
#include "auth_initiator.pb-c.h"
#include "auth_receiver.pb-c.h"

#define CHALLENGE_REQUEST_PUBLIC_KEY 1
#define CHALLENGE_REQUEST_FINISH 0

#define TRANSPORT_CREATE jnx_socket_tcp_create
#define TRANSPORT_LISTEN jnx_socket_tcp_listen_with_context
#define TRANSPORT_SEND jnx_socket_tcp_send
#define TRANSPORT_DESTROY jnx_socket_destroy


auth_comms_service *auth_comms_create(jnx_hashmap *config) {
  auth_comms_service *ac = malloc(sizeof(auth_comms_service));
  ac->listener_thread = NULL; 
  jnx_unsigned_int list_family = AF_INET;
  jnx_char *conf_list_family = jnx_hash_get(config,"AUTH_COMMS_LISTENER_FAMILY");
  if(conf_list_family != NULL) {
    JNX_LOG(0,"Using user definate address family for listener socket");
    list_family = atoi(conf_list_family);
  }
  jnx_char *list_port = "6362";
  jnx_char *conf_list_port = jnx_hash_get(config,"AUTH_COMMS_LISTENER_PORT");
  if(conf_list_port != NULL) {
    JNX_LOG(0,"Using user defined port for listener socket [%s]",list_port);
    list_port = conf_list_port;
  }
  ac->listener_port = list_port;
  ac->listener_family = list_family;
  
#ifndef AUTH_COMMS_TCP
  printf("Using UDP\n");
#else
  printf("Using TCP\n");
#endif


  JNX_LOG(0,"\nListener port: %s\nListener family: %d ",\
      ac->listener_port,ac->listener_family);
  auth_comms_start_listener(ac);
  JNX_LOG(0,"Started auth comms listener thread");

  return ac;
}
static int stop_listening(auth_comms_service *ac) {
  int dont_block = 1; 
  return ioctl(ac->comms_listener_socket->socket, FIONBIO, (char *) &dont_block);
}
jnx_int32 auth_comms_send_stop_packet(auth_comms_service *ac) {
  char *tmp = "STOP";
  jnx_socket *temp = TRANSPORT_CREATE(ac->listener_family);
  TRANSPORT_SEND(temp,"127.0.0.1",ac->listener_port, (jnx_uint8 *) tmp, 5);
  TRANSPORT_DESTROY(&temp);
  return 0;
}
void auth_comms_destroy(auth_comms_service **ac) {
  JNXCHECK(*ac);
  cancel_thread(&(*ac)->listener_thread);
  auth_comms_send_stop_packet(*ac);
  free(*ac);
  *ac = NULL;
}
static jnx_int32 auth_comms_listener_receive_handler(jnx_uint8 *payload,\
    jnx_size bytesread, jnx_socket *s, void *context) {
  auth_comms_service *ac = (auth_comms_service*)context;

  AuthInitiator *incoming_parcel = auth_initiator__unpack(NULL,bytesread,payload);
  if(incoming_parcel != NULL) {
    
    printf("Recieved incoming handshake parcel.\n");
    printf("Initiator guid = %s\n",incoming_parcel->initiator_guid);
    printf("Flags: requesting_public_key=%d requesting_finish=%d\n", \
        incoming_parcel->is_requesting_public_key,
        incoming_parcel->is_requesting_finish);
    auth_initiator__free_unpacked(incoming_parcel,NULL);
  }
  /* Listener exit strategy */
  char command[5];
  memset(command, 0, 5);
  memcpy(command, payload, 4);
  if(0 == strcmp(command,"STOP")){
    JNX_LOG(0,"Terminating auth comms listener");
    return -1;
  }
  /*                        */
  free(payload);
  return 0;
}
static void *auth_comms_listener_loop(void *data) {
  auth_comms_service *ac = (auth_comms_service*)data;
  ac->comms_listener_socket = TRANSPORT_CREATE(ac->listener_family); 
  TRANSPORT_LISTEN(ac->comms_listener_socket,ac->listener_port,
      100,ac->listener_callback,ac); 
}
void auth_comms_start_listener(auth_comms_service *ac) {
  ac->listener_thread = jnx_thread_create(auth_comms_listener_loop, (void*) ac);
  ac->listener_callback = auth_comms_listener_receive_handler;  
}

static int auth_comms_initiator_send_and_await_challenge(jnx_socket *s,\
    jnx_char *hostname, jnx_char *port,session *ses, jnx_int is_initial_challenge) {
  printf("Authentication comms initiating public key request...\n");

  jnx_uint8 *oreceipt;
  jnx_char *msg = "STOP";
  jnx_size msg_len = 5;
  /* This is not the final command to send */
  JNX_LOG(0,"Host %s Port %s",hostname,port);

  /* send the local initiator guid */
  jnx_char *local_guid_str;
  jnx_guid_to_string(&(*ses).local_peer_guid,&local_guid_str);
  JNX_LOG(0,"Packing initiator guid %s",local_guid_str);
  jnx_size len = strlen(local_guid_str);

  AuthInitiator auth_parcel = AUTH_INITIATOR__INIT;
  auth_parcel.initiator_guid = malloc(sizeof(char) * len);
  if(is_initial_challenge) {
    auth_parcel.is_requesting_public_key = 1;
    auth_parcel.is_requesting_finish = 0;
  }else {
    auth_parcel.is_requesting_public_key = 0;
    auth_parcel.is_requesting_finish = 1;
  }
  memcpy(auth_parcel.initiator_guid,local_guid_str,len); 
  free(local_guid_str);

  jnx_char *pub_key_str = asymmetrical_key_to_string(ses->keypair,PUBLIC);
  jnx_size pub_len = strlen(pub_key_str);
  auth_parcel.initiator_public_key = malloc(sizeof(char*) * pub_len);

  memcpy(auth_parcel.initiator_public_key,pub_key_str,pub_len);
  free(pub_key_str);

  jnx_int parcel_len = auth_initiator__get_packed_size(&auth_parcel);
  jnx_char *obuffer = malloc(parcel_len);
  auth_initiator__pack(&auth_parcel,obuffer);

  JNX_LOG(0,"-----\nSending request with flags\nis_requesting_public_key=%d\nis_requesting_finish=%d\n-----",
      auth_parcel.is_requesting_public_key,auth_parcel.is_requesting_finish);
  free(auth_parcel.initiator_guid);
  free(auth_parcel.initiator_public_key);

  TRANSPORT_SEND(s,hostname,port,obuffer,parcel_len);
}
void auth_comms_initiate_handshake(auth_comms_service *ac,\
    discovery_service *ds, session *s) {

  ac->comms_initiator_socket = TRANSPORT_CREATE(ac->listener_family);
  /* Retrieve our remote peer */
  peer *remote_peer = peerstore_lookup(ds->peers,&(*s).remote_peer_guid);
  JNXCHECK(remote_peer);
  print_peer(remote_peer);
  /* Let's make the assumption the remote peer will use the listener_port */
  auth_comms_initiator_send_and_await_challenge(ac->comms_initiator_socket,
      remote_peer->host_address,ac->listener_port,s,CHALLENGE_REQUEST_PUBLIC_KEY);
}
void auth_comms_receive_handshake(auth_comms_service *ac,\
    discovery_service *ds, session *s) {


}
