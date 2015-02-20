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
typedef struct session_service {
  jnx_list *session_list;
}session_service;

typedef enum session_state {
  SESSION_STATE_OKAY,
  SESSION_STATE_FAIL,
  SESSION_STATE_NOT_FOUND,
  SESSION_STATE_EXISTS
}session_state;

session_service *session_service_create();

void session_service_destroy(session_service **service);

session_state session_service_create_session(session_service *service, session **osession);

session_state session_service_fetch_session(session_service *service, jnx_guid *g, session **osession);

session_state session_service_fetch_all_sessions(session_service *service, jnx_list **olist);

session_state session_service_destroy_session(session_service *service, jnx_guid *g);

session_state session_service_link_sessions(session_service *s, jnx_guid *local_guid, jnx_guid *foreign_guid);

session_state session_service_unlink_sessions(session_service *s, jnx_guid *local_guid);

jnx_int session_service_session_is_linked(session_service *, jnx_guid *local_guid);
#endif
