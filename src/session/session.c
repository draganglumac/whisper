/*
 * session.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#include "session.h"

session_state session_message_write(session *s,jnx_char *message) {

}
 
session_state session_message_read_connect(session *s, session_read_callback cb) {

}
void session_add_initiator_public_key(session *s, jnx_char *key) {
  JNXCHECK(key);
  jnx_size len = strlen(key);
  s->initiator_public_key = malloc(len * sizeof(jnx_char));
  memcpy(s->initiator_public_key,key,len);
}
void session_add_receiver_public_key(session *s, jnx_char *key) {
  JNXCHECK(key);
  jnx_size len = strlen(key);
  s->receiver_public_key = malloc(len * sizeof(jnx_char));
  memcpy(s->receiver_public_key,key,len);
}
void session_add_shared_secret(session *s, jnx_char *secret) {
  JNXCHECK(secret);
  jnx_size len = strlen(secret);
  s->shared_secret = malloc(len * sizeof(jnx_char));
  memcpy(s->shared_secret,secret,len);
}
void session_add_secure_comms_port(session *s, jnx_char *comms_port) {
  JNXCHECK(comms_port);
  jnx_size len = strlen(comms_port) +1;
  s->secure_comms_port = malloc(len * sizeof(jnx_char));
  bzero(s->secure_comms_port,len);
  memcpy(s->secure_comms_port,comms_port,len);
}
void session_add_remote_peer_guid(session *s, jnx_uint8 *guid_str) {
  JNXCHECK(guid_str);
  jnx_guid g;
  jnx_guid_from_string(guid_str,&g);
  s->remote_peer_guid = g;
}
