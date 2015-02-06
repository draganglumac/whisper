/*
 * =============================================================================
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
 * =============================================================================
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

typedef enum session_service_state {
  SESSION_SERVICE_OKAY,
  SESSION_SERVICE_FAIL,
  SESSION_SERVICE_NOT_FOUND,
}session_service_state;

session_service *session_service_create();

void session_service_destroy(session_service *ss);

void session_service_create_session(session_service *ss,jnx_guid *outguid);

void session_service_destroy_session(session_service *ss, jnx_guid *inguid);

session_service_state session_service_fetch_session(session_service *s,
    jnx_guid *g, SessionObject **osessionObject);

void session_service_fetch_session_keys(session_service *ss,
    jnx_guid *g, RSA **okeys);

void session_service_update_session(jnx_guid *g);

#endif
