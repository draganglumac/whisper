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
  JNXCHECK(s->key_data_list);
  JNXCHECK(s->key_data_list->internal_lock);
  return s;
}
session_key_store_state session_key_store_destroy(session_key_store *s) {
  //TODO: Do we need to destroy keys
  jnx_list_destroy(&s->key_data_list);
  free(s);
  return SESSION_KEY_STORE_OKAY;
}
jnx_int session_key_store_does_exist(session_key_store *s, jnx_guid *g) {
  JNXCHECK(s->key_data_list);
  JNXCHECK(s->key_data_list->internal_lock);

  jnx_node *n = s->key_data_list->head;
  jnx_node *reset = s->key_data_list->head;

  while(n) {
    session_key_data *d = n->_data;
    jnx_guid *bguid = d->guid;
    if(jnx_guid_compare(g,bguid)) {
      s->key_data_list->head = reset;
      return 1;
    }
    n = n->next_node;
  }
  s->key_data_list->head = reset;
  return 0;
}
session_key_store_state session_key_store_add(session_key_store *s, jnx_guid *g, RSA *keypair) {
  JNXCHECK(s);
  if(!session_key_store_does_exist(s,g)) {
    session_key_data *skd = malloc(sizeof(session_key_data));
    skd->guid = g;
    skd->keypair = keypair; 
    jnx_list_add_ts(s->key_data_list,skd);
    return SESSION_KEY_STORE_OKAY;
  }
  return SESSION_KEY_STORE_EXISTS;
}
session_key_store_state session_key_store_remove(session_key_store *s,jnx_guid *g, session_key_data **okeydata) {
  JNXFAIL("Not implemented");
  return SESSION_KEY_STORE_NOT_FOUND;
}
session_key_store_state session_key_store_retrieve_key(session_key_store *s, jnx_guid *g,RSA **okeydata) {
  *okeydata = NULL;
  jnx_node *n = s->key_data_list->head;
  while(n) {
    session_key_data *data = n->_data;
    jnx_guid *c = data->guid;
    if(jnx_guid_compare(g,c)) {
      *okeydata = data->keypair;
      return SESSION_KEY_STORE_EXISTS;
    }
    n = n->next_node;
  }
  return SESSION_KEY_STORE_NOT_FOUND;
}
