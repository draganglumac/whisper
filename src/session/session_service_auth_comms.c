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
#include "../util/utils.h"
auth_comms_service *auth_comms_create(jnx_hashmap *config) {
  auth_comms_service *ac = malloc(sizeof(auth_comms_service));
  ac->listener_thread = NULL; 
  jnx_unsigned_int init_family = AF_INET;
  jnx_char *conf_init_family = jnx_hash_get(config,"AUTH_COMMS_INITIATOR_FAMILY");
  if(conf_init_family != NULL) {
    JNX_LOG(0,"Using user defined address family for initiator socket");
    init_family = atoi(conf_init_family);
  }
  jnx_char *init_port = "6361";
  jnx_char *conf_init_port = jnx_hash_get(config,"AUTH_COMMS_INITIATOR_PORT");
  if(conf_init_port != NULL) {
    init_port = conf_init_port;
    JNX_LOG(0,"Using user defined port for initiator socket [%d]",init_port);
  }
  jnx_unsigned_int list_family = AF_INET;
  jnx_char *conf_list_family = jnx_hash_get(config,"AUTH_COMMS_LISTENER_FAMILY");
  if(conf_list_family != NULL) {
    JNX_LOG(0,"Using user definate address family for listener socket");
    list_family = atoi(conf_list_family);
  }
  jnx_char *list_port = "6362";
  jnx_char *conf_list_port = jnx_hash_get(config,"AUTH_COMMS_LISTENER_PORT");
  if(conf_list_port != NULL) {
    JNX_LOG(0,"Using user defined port for listener socket [%d]",list_port);
    list_port = conf_list_port;
  }
  ac->initiator_port = init_port;
  ac->initiator_family = init_family;
  ac->listener_port = list_port;
  ac->listener_family = list_family;

  JNX_LOG(0,"\nInitiator port: %s\nInitiator family: %d\nListener port: %s\nListener family: %d ",ac->initiator_port,ac->initiator_family,
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
  jnx_socket *temp = jnx_socket_tcp_create(ac->listener_family);
  jnx_socket_tcp_send(temp,"127.0.0.1",ac->listener_port, (jnx_uint8 *) tmp, 5);
  jnx_socket_destroy(&temp);
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

  JNX_LOG(0,"auth_comms_listener_receive_handler raw input: [%dbytes] -%s",bytesread,payload);

  char command[5];
  memset(command, 0, 5);
  memcpy(command, payload, 4);
  if(0 == strcmp(command,"STOP")){
    JNX_LOG(0,"Terminating auth comms listener");
    return -1;
  }
  free(payload);
  return 0;
}
static void *auth_comms_listener_loop(void *data) {
  auth_comms_service *ac = (auth_comms_service*)data;
  ac->comms_listener_socket = jnx_socket_tcp_create(ac->listener_family); 
  jnx_socket_tcp_listen_with_context(ac->comms_listener_socket,ac->listener_port,0,ac->listener_callback,ac); 
}
void auth_comms_start_listener(auth_comms_service *ac) {
  JNXCHECK(ac->comms_listener_socket != NULL);
  JNXCHECK(ac->listener_thread == NULL);
  ac->listener_thread = jnx_thread_create(auth_comms_listener_loop, (void*) ac);
  ac->listener_callback = auth_comms_listener_receive_handler;  
}

static int auth_comms_initiator_send_and_await_public_key(jnx_socket *s,jnx_char *hostname, jnx_char *port) {
  printf("Authentication comms initiating public key request...\n");

  jnx_uint8 *oreceipt;
  jnx_char *msg = "STOP";
  jnx_size msg_len = 5;
  /* This is not the final command to send */
  jnx_socket_tcp_send_with_receipt(s,hostname,port,msg,msg_len,&oreceipt);
}
void auth_comms_initiate_handshake(auth_comms_service *ac,discovery_service *ds, session *s) {

  ac->comms_initiator_socket = jnx_socket_tcp_create(ac->initiator_family);
  
  /* Retrieve our remote peer */
  peer *remote_peer = peerstore_lookup(ds->peers,&(*s).remote_peer_guid);
  JNXCHECK(remote_peer);
  print_peer(remote_peer);
  /* Let's make the assumption the remote peer will use the listener_port */
  auth_comms_initiator_send_and_await_public_key(ac->comms_initiator_socket,
      remote_peer->host_address,ac->listener_port);    

}
void auth_comms_receive_handshake(auth_comms_service *ac,discovery_service *ds, session *s) {

}
