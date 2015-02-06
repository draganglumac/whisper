/*
 * =============================================================================
 * 
 *       Filename:  keystore.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/31/2015 01:11:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =============================================================================
 */
#ifndef __SESSION_STORE_H__
#include <openssl/rsa.h>
#include <jnxc_headers/jnxlist.h>
#include <jnxc_headers/jnxguid.h>
#include "session_object.pb-c.h" 
#define __SESSION_STORE_H__
/*
 * The keystore keeps information about a sessions keys asymmetrical/symmetrical
 */
typedef struct session_data {
  jnx_guid *guid;
  SessionObject *session;
}session_data;

typedef struct session_store {
  jnx_list *key_data_list;
}session_store;

typedef enum session_store_state {
  SESSION_STORE_EXISTS,
  SESSION_STORE_NOT_FOUND,
  SESSION_STORE_OKAY,
  SESSION_STORE_FAILURE
}session_store_state;

session_store *session_store_create();

session_store_state session_store_destroy(session_store *s);

jnx_int session_store_does_exist(session_store *s, jnx_guid *g);

session_store_state session_store_add(session_store *s,SessionObject *session);

session_store_state session_store_remove(session_store *s, 
    jnx_guid *g, session_data **okeydata);

void session_store_retrieve_session(session_store *s,
    jnx_guid *g,SessionObject **osession);
#endif
