/*
 * session.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#include "session.h"

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
