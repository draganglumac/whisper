/*
 * =====================================================================================
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
 * =====================================================================================
 */
#ifndef __KEYSTORE_H__
#include <openssl/rsa.h>
#include <jnxc_headers/jnxlist.h>
#include <jnxc_headers/jnxguid.h>
#define __KEYSTORE_H__
/*
 * The keystore keeps information about a sessions keys asymmetrical/symmetrical
 */
typedef struct session_key_data {
  jnx_guid *guid;
  RSA *keypair;
}session_key_data;

typedef struct session_key_store {
  jnx_list *key_data_list;
}session_key_store;

typedef enum session_key_store_state {
  SESSION_KEY_STORE_EXISTS,
  SESSION_KEY_STORE_NOT_FOUND,
  SESSION_KEY_STORE_OKAY,
  SESSION_KEY_STORE_FAILURE
}session_key_store_state;

session_key_store *session_key_store_create();

session_key_store_state session_key_store_destroy(session_key_store *s);

jnx_int session_key_store_does_exist(session_key_store *s, jnx_guid *g);

session_key_store_state session_key_store_add(session_key_store *s, jnx_guid *g, RSA *keypair);

session_key_store_state session_key_store_remove(session_key_store *s,jnx_guid *g, session_key_data **okeydata);

void session_key_store_retrieve_key(session_key_store *s, jnx_guid *g,RSA **okeydata);
#endif
