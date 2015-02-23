/*
 * session_service_auth_comms.h
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SESSION_SERVICE_AUTH_COMMS_H
#define SESSION_SERVICE_AUTH_COMMS_H
#include "session.h"
#include "../net/discovery.h"
#include <jnxc_headers/jnxsocket.h>
#include <jnxc_headers/jnxthread.h>
typedef struct auth_comms_service {
  jnx_socket *comms_initiator_socket;

  jnx_socket *comms_listener_socket;
  jnx_unsigned_int listener_family;
  jnx_char *listener_port;
  jnx_thread *listener_thread;
  tcp_socket_listener_callback_with_context listener_callback;

}auth_comms_service;

auth_comms_service *auth_comms_create(jnx_hashmap *config);

void auth_comms_destroy(auth_comms_service **ac);

void auth_comms_start_listener(auth_comms_service *ac);

void auth_comms_stop_listener(auth_comms_service *ac);

void auth_comms_initiate_handshake(auth_comms_service *ac,discovery_service *ds, session *s);

void auth_comms_receive_handshake(auth_comms_service *ac,discovery_service *ds, session *s);
#endif /* !SESSION_SERVICE_AUTH_COMMS_H */
