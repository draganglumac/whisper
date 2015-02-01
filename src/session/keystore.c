/*
   :48
   :37
 * =====================================================================================
 *
 *       Filename:  keystore.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/31/2015 02:43:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxguid.h>
#include "keystore.h"

session_key_store *session_key_store_create() {
  session_key_store *s = malloc(sizeof(session_key_store));
  s->key_data_list = jnx_list_create();
  return s;
}
session_key_store_state session_key_store_destroy(session_key_store *s) {
  /*
   * TODO:
   * Are there keys you also wish to destroy? 
   */
  jnx_list_destroy(&s->key_data_list);
  free(s);

  return SESSION_KEY_STORE_OKAY;
}
jnx_int32 compare_sessions(void *a, void *b) {

  session_key_data *bdata = (session_key_data*)b;
  jnx_guid *bguid = bdata->guid;

  jnx_guid *aguid = (jnx_guid*)a;

  jnx_guid_state r = jnx_guid_compare(aguid,bguid);
  if(r == JNX_GUID_STATE_SUCCESS) {
    return 1;
  }
  return 0;
}
session_key_store_state session_key_store_does_exist(session_key_store *s, jnx_guid *g) {

  jnx_int r = jnx_list_contains(s->key_data_list,g,compare_sessions);
  if(r == 1) {
    return SESSION_KEY_STORE_EXISTS;
  }
  return SESSION_KEY_STORE_NOT_FOUND;
}
session_key_store_state session_key_store_add(session_key_store *s, jnx_guid *g, RSA *keypair) {
  //does exist?
  if(session_key_store_does_exist(s,g) == SESSION_KEY_STORE_EXISTS) {
    return SESSION_KEY_STORE_EXISTS;
  }else {
    session_key_data *skd = malloc(sizeof(session_key_data));
    skd->guid = g;
    skd->keypair = keypair; 
    jnx_list_add(s->key_data_list,skd);
  }
  return SESSION_KEY_STORE_OKAY;
}
