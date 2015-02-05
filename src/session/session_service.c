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
#include "session_service.h"
#include "keystore.h"
#include "session.h"

session_service *session_service_create() {
  session_service *s = malloc(sizeof(session_service));
  s->keystore = session_key_store_create(); 
  return s;
}
void session_service_destroy(session_service *ss) {
  session_key_store_destroy(ss->keystore);
  free(ss);
}
void session_service_create_session(session_service *ss,SessionObject *os) {
  session_create(os,ss->keystore);
}
void session_service_fetch_session_keys(session_service *ss,SessionObject *session, RSA **okeys) {
  jnx_guid g;
  session_fetch_guid(session,&g);
  *okeys = NULL;
  JNXCHECK(session_key_store_does_exist(ss->keystore,&g));
  JNXCHECK(session_key_store_retrieve_key(ss->keystore,&g,okeys) == SESSION_KEY_STORE_EXISTS);  
}
