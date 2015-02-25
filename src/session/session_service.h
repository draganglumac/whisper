/*
 * =====================================================================================
 *
 *       Filename:  session_service.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/09/2015 05:50:25 AM
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
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlist.h>
#include <jnxc_headers/jnxcheck.h>
#include "session.h"
#include "../data/peer.h"
#include "../net/communications.h"
typedef struct session_service {
  jnx_list *session_list;
  communication_service *communication;
}session_service;

typedef enum session_state {
  SESSION_STATE_OKAY,
  SESSION_STATE_FAIL,
  SESSION_STATE_NOT_FOUND,
  SESSION_STATE_EXISTS
}session_state;

typedef enum session_handshake_state {
  SESSION_HANDSHAKE_START,
  SESSION_HANDSHAKE_RETURN_PUBLIC,
  SESSION_HANDSHAKE_SEND_SHARED,
  SESSION_HANDSHAKE_FINISH
}session_handshake_state;

session_service *session_service_create();

void session_service_destroy(session_service **service);

session_state session_service_create_session(session_service *service,session \
    **osession);

session_state session_service_fetch_session(session_service *service,jnx_guid \
    *session_guid, session **osession);

session_handshake_state session_service_fetch_session_state(session_service *service,\
    jnx_guid *session_guid);

session_state session_service_tick_session(session_service *service,\
    jnx_guid *session_guid);

session_state session_service_fetch_all_sessions(session_service *service,\
    jnx_list **olist);

session_state session_service_destroy_session(session_service *service,jnx_guid \
    *session_guid);

session_state session_service_link_sessions(session_service *s,jnx_guid \
    *session_guid, peer *local_peer, peer *remote_peer);

session_state session_service_unlink_sessions(session_service *s,jnx_guid \
    *session_guid);

jnx_int session_service_session_is_linked(session_service *,jnx_guid \
    *session_guid);
#endif
