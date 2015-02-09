/*
 * =====================================================================================
 *
 *       Filename:  session.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/01/15 12:32:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxguid.h>
#include "../crypto/cryptography.h"
#include "../integrity/encoding.h"
#include <string.h>
#include "session.h"

void session_guid_create(SessionObject *s) {
  jnx_guid g;
  jnx_guid_create(&g);
  jnx_char *ostr;
  jnx_guid_to_string(&g,&ostr);
  JNXCHECK(strlen(ostr) > 24);
  s->guid = malloc(strlen(ostr) + 1);
  bzero(s->guid,strlen(ostr) +1);
  memcpy(s->guid,ostr,strlen(ostr) + 1);
  JNX_LOG(DEFAULT_CONTEXT,"New session %s\n",s->guid);
  JNXCHECK(s->guid);
  free(ostr);
}
jnx_size session_pack(SessionObject *s,jnx_uint8 **obuffer) {
  jnx_size size = session_object__get_packed_size(s);
  *obuffer = malloc(size);
  session_object__pack(s,*obuffer);
  return size;
}
SessionObject *session_unpack(jnx_uint8 *buffer,jnx_size len) {
  SessionObject *s=session_object__unpack(DEFAULT_CONTEXT,len,buffer);
  return s;
}
void session_fetch_guid(SessionObject *s,jnx_guid *g) {
  jnx_guid_from_string(s->guid,g);
}
void session_generate_keys(SessionObject *s,session_key_store *sk) {
  JNXCHECK(s);
  JNXCHECK(sk);
  RSA *keys = asymmetrical_generate_key(2048); 
  //Store key
  jnx_guid g;
  jnx_guid_from_string(s->guid,&g);
  JNXCHECK(session_key_store_does_exist(sk,&g) == 0);
  jnx_int key_count = sk->key_data_list->counter;
  JNXCHECK(session_key_store_add(sk,&g,keys) == SESSION_KEY_STORE_OKAY);
  JNXCHECK(sk->key_data_list->counter == (key_count + 1));
}
void session_create(SessionObject **s, session_key_store *sk) {
  session_object__init(*s);
  session_guid_create(*s);
  session_generate_keys(*s,sk);
}
