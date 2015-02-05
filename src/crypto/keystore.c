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
  jnx_int does_exist = 0;
  jnx_node *h = s->key_data_list->head,
           *r = s->key_data_list->head;
  while(h) {
    session_key_data *d = h->_data;
    jnx_guid *stored_guid = d->guid;
    if(jnx_guid_compare(g,stored_guid)) {
      does_exist = 1;
    }
    h = h->next_node;
  }
  s->key_data_list->head = r;
  return does_exist;
}
session_key_store_state session_key_store_add(session_key_store *s, jnx_guid *g, RSA *keypair) {
  JNXCHECK(s);
  JNXCHECK(g);
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
void session_key_store_retrieve_key(session_key_store *s, jnx_guid *g,RSA **okeydata) {
  *okeydata = NULL;
  jnx_node *h = s->key_data_list->head,
           *r = s->key_data_list->head;
  while(h) {
    session_key_data *d = h->_data;
    jnx_guid *stored_guid = d->guid;
    if(jnx_guid_compare(g,stored_guid)) {
      *okeydata = d->keypair;
    }
    h = h->next_node;
  }
  s->key_data_list->head = r;
}
