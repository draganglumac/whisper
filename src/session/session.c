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
  s->guid = malloc(strlen(ostr) + 1);
  bzero(s->guid,strlen(ostr) +1);
  memcpy(s->guid,ostr,strlen(ostr) + 1);
  JNX_LOG(DEFAULT_CONTEXT,"New session %s\n",s->guid);
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
  RSA *keys = asymmetrical_generate_key(2048); 
  jnx_char *publickey = asymmetrical_key_to_string(keys,PUBLIC);
  jnx_size len = strlen(publickey);
  s->rsa_public_key = malloc(len);
  bzero(s->rsa_public_key,len);
  memcpy(s->rsa_public_key,publickey,len);
  /*
   * Store if we can
   */
  jnx_guid g;
  jnx_guid_from_string(s->guid,&g);
  session_key_store_state e = session_key_store_does_exist(sk,&g);
  JNXCHECK(e == SESSION_KEY_STORE_NOT_FOUND);
  e = session_key_store_add(sk,&g,keys);
  JNXCHECK(e == SESSION_KEY_STORE_OKAY);
}
void session_create(SessionObject *s, session_key_store *sk) {
  session_object__init(s);
  session_guid_create(s);
  session_generate_keys(s,sk);
}
