/*
 * session_service_auth_comms.h
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef AUTH_COMMS_H
#define AUTH_COMMS_H
#include "../net/discovery.h"
#include <jnxc_headers/jnxsocket.h>
#include "../session/session.h"
#include <jnxc_headers/jnxthread.h>
typedef struct auth_comms_service {
  jnx_socket *listener_socket;
  jnx_unsigned_int listener_family;
  jnx_char *listener_port;
  jnx_thread *listener_thread;
  tcp_socket_listener_callback_with_context listener_callback;

}auth_comms_service;

auth_comms_service *auth_comms_create();

void auth_comms_listener_start(auth_comms_service *ac);

void auth_comms_destroy(auth_comms_service **ac);

void auth_comms_initiator_start(auth_comms_service *ac, \
    discovery_service *ds, session *s);

void auth_comms_receiver_start(auth_comms_service *ac, \
    discovery_service *ds, session *s);

#endif /* !AUTH_COMMS_H */
