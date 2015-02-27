/*
 * session_service_auth_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */
#include <jnxc_headers/jnxlog.h>
#include "session_service_auth_comms.h"

auth_comms_service *auth_comms_create(jnx_hashmap *config) {
  auth_comms_service *ac = malloc(sizeof(auth_comms_service));
  ac->listener_thread = NULL; 
  jnx_unsigned_int init_family = AF_INET;
  jnx_char *conf_init_family = jnx_hash_get(config,"AUTH_COMMS_INITIATOR_FAMILY");
  if(conf_init_family != NULL) {
    JNX_LOG(0,"Using user defined address family for initiator socket");
    init_family = atoi(conf_init_family);
  }
  jnx_int init_port = 6361;
  jnx_char *conf_init_port = jnx_hash_get(config,"AUTH_COMMS_INITIATOR_PORT");
  if(conf_init_port != NULL) {
    init_port = atoi(conf_init_port);
    JNX_LOG(0,"Using user defined port for initiator socket [%d]",init_port);
  }
  jnx_unsigned_int list_family = AF_INET;
  jnx_char *conf_list_family = jnx_hash_get(config,"AUTH_COMMS_LISTENER_FAMILY");
  if(conf_list_family != NULL) {
    JNX_LOG(0,"Using user definate address family for listener socket");
    list_family = atoi(conf_list_family);
  }
  jnx_int list_port = 6362;
  jnx_char *conf_list_port = jnx_hash_get(config,"AUTH_COMMS_LISTENER_PORT");
  if(conf_list_port != NULL) {
    JNX_LOG(0,"Using user defined port for listener socket [%d]",list_port);
    list_port = atoi(conf_list_port);
  }
  auth_comms_start_listener(ac);
  JNX_LOG(0,"Started auth comms listener thread");

  return ac;
}
void auth_comms_destroy(auth_comms_service **ac) {

  free(*ac);
  *ac = NULL;
}
static jnx_int32 auth_comms_listener_receive_handler(jnx_uint8 *payload, jnx_size bytesread, jnx_socket *s, void *context) {

}
static void *auth_comms_listener_loop(void *data) {
  auth_comms_service *ac = (auth_comms_service*)data;


}
void auth_comms_start_listener(auth_comms_service *ac) {
  JNXCHECK(ac->comms_listener_socket != NULL);
  JNXCHECK(ac->listener_thread == NULL);
  ac->listener_thread = jnx_thread_create(auth_comms_listener_loop, (void*) ac);
  ac->listener_callback = auth_comms_listener_receive_handler;  
}
void auth_comms_stop_listener(auth_comms_service *ac) {

}
