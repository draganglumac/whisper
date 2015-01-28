/*
 * =====================================================================================
 *
 *       Filename:  session.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/01/15 12:02:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __SESSION_H__
#define __SESSION_H__
#include "session_object.pb-c.h"
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxtypes.h>
#include <jnxc_headers/jnxhash.h>

typedef struct session_key_store {
  jnx_hashmap *store;
}session_key_store;

typedef enum SESSION_STORE_STATE {
  SESSION_STORE_OKAY,
  SESSION_STORE_FAILURE,
  SESSION_STORE_FOUND_OKAY,
  SESSION_STORE_NOT_FOUND,
}SESSION_STORE_STATE;
/*
 * Key store is where we keep our local session RSA full keys
 */
session_key_store *session_key_store_create();

void session_key_store_destroy(session_key_store *s);

void session_key_store_send(session_key_store *s,jnx_uint8 *guid, jnx_size guid_len, RSA *keypair);

SESSION_STORE_STATE session_key_store_retrieve(session_key_store *s, jnx_uint8 *guid, jnx_size guid_len, RSA **okeypair);
/*
 * 
 */

void session_guid_create(SessionObject *s);

jnx_size session_pack(SessionObject *s,jnx_uint8 **obuffer);

SessionObject *session_unpack(jnx_uint8 *buffer,jnx_size len);

void session_generate_keys(SessionObject *s,session_key_store *sk);
#endif
