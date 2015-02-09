/*
 * =====================================================================================
 *
 *       Filename:  session_service.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/01/2015 12:46:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "../crypto/cryptography.h"
#include "../util/utils.h"
#include "session_service.h"
#include "session.h"

session_service *session_service_create() {
  session_service *s = malloc(sizeof(session_service));
  s->keystore = session_key_store_create();
  s->sessionstore = session_store_create();
  return s;
}
void session_service_destroy(session_service *ss) {
  session_key_store_destroy(ss->keystore);
  free(ss);
}
void session_service_create_session(session_service *ss,jnx_guid *outguid) {
  SessionObject *os;
  session_create(&os,ss->keystore);
  session_store_add(ss->sessionstore,os);
  session_fetch_guid(os,outguid);
}
void session_service_destroy_session(session_service *ss, jnx_guid *inguid) {
  session_data *sessiondata = NULL;
  session_store_remove(ss->sessionstore,inguid,&sessiondata);
  session_key_data *keydata = NULL;
  session_key_store_remove(ss->keystore,inguid,&keydata);
  if(sessiondata) {
    free(sessiondata);
  }
  if(keydata) {
    asymmetrical_destroy_key(keydata->keypair); 
    free(keydata);
  }
}
session_service_state session_service_fetch_session(session_service *s, jnx_guid *g, SessionObject **osessionObject) {
  *osessionObject = NULL;
  session_store_retrieve_session(s->sessionstore,g,osessionObject);
  if(!*osessionObject) {
    return SESSION_STORE_NOT_FOUND; 
  }
  return SESSION_STORE_EXISTS;
}
void session_service_link_session(session_service *ss,jnx_guid *inguid, jnx_guid *foriegn_guid) {
  print_guid(inguid);
  print_guid(foriegn_guid);
}
void session_service_fetch_session_keys(session_service *ss,
    jnx_guid *g, RSA **okeys) {
  *okeys = NULL;
  SessionObject *os;
  session_store_retrieve_session(ss->sessionstore,g,&os);
  JNXCHECK(session_key_store_does_exist(ss->keystore,g));
  session_key_store_retrieve_key(ss->keystore,g,okeys); 
}
void session_service_update_session(jnx_guid *g) {

}
