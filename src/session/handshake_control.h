/*
 * session_service_auth_comms.h
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef HANDSHAKE_CONTROL_H
#define HANDSHAKE_CONTROL_H
#include "session.h"
#include "../net/discovery.h"
#include <jnxc_headers/jnxsocket.h>
#include <jnxc_headers/jnxthread.h>

int handshake_initiator_command_generate(session *ses,\
    jnx_int is_initial_challenge, jnx_uint8 **onetbuffer);

int handshake_generate_public_key_request(session *ses,\
    jnx_uint8 **onetbuffer);

int handshake_generate_finish_request(session *ses,\
    jnx_uint8 **onetbuffer);
#endif /* !HANDSHAKE_CONTROL_H */
