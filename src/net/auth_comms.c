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
static void send_data(jnx_char *hostname, jnx_char *port,
	unsigned int family,
	jnx_uint8 *buffer, jnx_int bytes) {

	jnx_socket *sock = jnx_socket_tcp_create(family);
	jnx_socket_tcp_send(sock,hostname,port,buffer,bytes);
	jnx_socket_destroy(&sock);
}
static jnx_int32 listener_callback(jnx_uint8 *payload,
	jnx_size bytes_read, jnx_socket *s) {

	printf("Read data on auth comms\n");

	return 0;
}
static void *listener_bootstrap(void *args) {
	auth_comms_service *ac = (auth_comms_service*)args;
	JNXCHECK(ac->listener_socket);
	jnx_socket_tcp_listen(ac->listener_socket,
		ac->listener_port,100,listener_callback);
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

	peer *remote_peer = peerstore_lookup(ds->peers,&(*s).remote_peer_guid);

	jnx_socket *sender = jnx_socket_tcp_create(ac->listener_family);

	jnx_uint8 *obuffer;
	jnx_int bytes_read = handshake_generate_public_key_request(s,&obuffer);

	printf("Generated initial handshake...[%d/bytes]\n",bytes_read);

	send_data(remote_peer->host_address,ac->listener_port, 
		ac->listener_family,
		obuffer,bytes_read);

	free(obuffer);
}
void auth_comms_receiver_start(auth_comms_service *ac, \
    discovery_service *ds, session *s) {

}

