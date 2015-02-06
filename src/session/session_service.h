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
//---------------------------------
// Session service is the manager for all things session
// It will be used to create a session and return you the guid of that session
// Behind the scenes it will generate cryptographic keypairs
// It will also store your SessionObject for transmission
//---------------------------------
//Example Usage:
//
//Peer A wants to talk to Peer B
//
//Peer A creates a local session through service
//
//Session service links the guid against keystore and sessionstore
//
//SessionObject is packed and sent to Peer B
//
//Peer B generates session and links against incoming guid (guids form a pair)
//
//Peer B packs and transmits encrypted public key to A
//
//Peer A decrypts public key of B
//
//Session service generates session symmetrical key
//
//Peer A encrypts symmetrical key with public key of B
//
//Session service sends packed encrypted key to Peer B
//
//B unpacks encrypted session key and replies that handshake is complete
//---------------------------------
typedef struct session_service {
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
