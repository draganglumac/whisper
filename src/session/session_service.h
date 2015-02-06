/*
 * =====================================================================================
 *
 *       Filename:  session_service.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/01/2015 12:45:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __SESSION_SERVICE_H__
#define __SESSION_SERVICE_H__
#include "session.h"
#include "../crypto/keystore.h"
#include "session_store.h"
typedef struct session_service {
  /*
   * Session components are tied together through a guid
   */
  session_key_store *keystore;
  session_store *sessionstore;
}session_service;

session_service *session_service_create();

void session_service_destroy(session_service *ss);
/*
 *@fn SessionObject *session_service_create_session(session_service *ss)
 *@return fully initialised protobuf SessionObject with populated guid and associated
 *keys in the keystore
 */
void session_service_create_session(session_service *ss,SessionObject *os);

//SessionObject* session_service_fetch_session(jnx_guid *g);

//void session_service_update_session(jnx_guid *g);

//sesssion_service_state session_service_resolve_session(jnx_guid *g, SessionObject **osession);

void session_service_fetch_session_keys(session_service *ss,SessionObject *session, RSA **okeys);
#endif
