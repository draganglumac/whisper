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
#include "session_store.h"

session_store *session_store_create() {
  session_store *s = malloc(sizeof(session_store));
  s->key_data_list = jnx_list_create();
  JNXCHECK(s->key_data_list);
  JNXCHECK(s->key_data_list->internal_lock);
  return s;
}
session_store_state session_store_destroy(session_store *s) {
  //TODO: Do we need to destroy keys
  jnx_list_destroy(&s->key_data_list);
  free(s);
  return SESSION_STORE_OKAY;
}
jnx_int session_store_does_exist(session_store *s, jnx_guid *g) {
  jnx_int does_exist = 0;
  jnx_node *h = s->key_data_list->head,
           *r = s->key_data_list->head;
  while(h) {
    session_data *d = h->_data;
    jnx_guid *stored_guid = d->guid;
    if(jnx_guid_compare(g,stored_guid)) {
      does_exist = 1;
    }
    h = h->next_node;
  }
  s->key_data_list->head = r;
  return does_exist;
}
session_store_state session_store_add(session_store *s,SessionObject *session) {
  JNXCHECK(s);
  jnx_guid g;
  jnx_guid_from_string(session->guid,&g); 
  if(!session_store_does_exist(s,&g)) {
    session_data *skd = malloc(sizeof(session_data));
    skd->guid = &g;
    skd->session = session;
    jnx_list_add_ts(s->key_data_list,skd);
    return SESSION_STORE_OKAY;
  }
  return SESSION_STORE_EXISTS;
}
session_store_state session_store_remove(session_store *s,jnx_guid *g, session_data **okeydata) {
  JNXFAIL("Not implemented");
  return SESSION_STORE_NOT_FOUND;
}
void session_store_retrieve_session(session_store *s, jnx_guid *g,SessionObject **osession) {
  *osession = NULL;
  jnx_node *h = s->key_data_list->head,
           *r = s->key_data_list->head;
  while(h) {
    session_data *d = h->_data;
    jnx_guid *stored_guid = d->guid;
    if(jnx_guid_compare(g,stored_guid)) {
      *osession = d->session;
    }
    h = h->next_node;
  }
  s->key_data_list->head = r;
}
