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
#include <jnxc_headers/jnxqueue.h>

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
  jnx_char *initiator_public_key;
  jnx_char *receiver_public_key;
  jnx_char *shared_secret;
  jnx_int is_connected;
  jnx_char *secure_comms_port;
  jnx_int secure_comms_fd;
  /* local only */
  RSA *keypair;
}session;

void session_add_initiator_public_key(session *s, jnx_char *key);

void session_add_receiver_public_key(session *s, jnx_char *key);

void session_add_shared_secret(session *s, jnx_char *secret);

void session_add_secure_comms_port(session *s, jnx_char *comms_port);

void session_add_remote_peer_guid(session *s, jnx_uint8 *guid_str);
#endif
