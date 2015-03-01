/*
 * session_service_auth_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */
#include <jnxc_headers/jnxlog.h>
#include "auth_comms.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include "../crypto/cryptography.h"
#include "../util/utils.h"
#include "../session/handshake_control.h"

#define CHALLENGE_REQUEST_PUBLIC_KEY 1
#define CHALLENGE_REQUEST_FINISH 0

/* Listener Thread */



static void *listener_bootstrap(void *args) {

}
/* Listener Thread */

auth_comms_service *auth_comms_create() {
  return malloc(sizeof(auth_comms_service));
}

void auth_comms_listener_start(auth_comms_service *ac) {
  ac->listener_thread = jnx_thread_create(listener_bootstrap,NULL);
}
void auth_comms_destroy(auth_comms_service **ac) {

}

void auth_comms_initiator_start(auth_comms_service *ac, \
    discovery_service *ds, session *s) {

}

void auth_comms_receiver_start(auth_comms_service *ac, \
    discovery_service *ds, session *s) {

}

