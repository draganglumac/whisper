/*
 * =====================================================================================
 *
 *       Filename:  session.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/01/2015 20:50:54
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __SESSION_H__
#define __SESSION_H__
#include "../crypto/cryptography.h"
#include <jnxc_headers/jnxguid.h>
typedef enum session_handshake_state {
  SESSION_HANDSHAKE_DORMANT,
  SESSION_HANDSHAKE_START,
  SESSION_HANDSHAKE_RETURN_PUBLIC,
  SESSION_HANDSHAKE_SEND_SHARED,
  SESSION_HANDSHAKE_FINISH,
  SESSION_HANDSHAKE_FAIL
}session_handshake_state;

typedef enum session_state {
  SESSION_STATE_OKAY,
  SESSION_STATE_FAIL,
  SESSION_STATE_NOT_FOUND,
  SESSION_STATE_EXISTS
}session_state;

typedef struct session {
  jnx_guid session_guid;
  jnx_guid local_peer_guid;
  jnx_guid remote_peer_guid;
  RSA *keypair;
  session_handshake_state handshake_state;
}session;

#endif
