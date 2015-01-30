/*
 * =====================================================================================
 *
 *       Filename:  discovery.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/01/15 16:14:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (draganglumac), dragan.glumac@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxsocket.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxthread.h>
#include <jnxc_headers/jnxcheck.h>
#include "discovery.h"
#include "data/peer.h"
#include "err/whisper_errors.h"

typedef struct {
	jnx_socket *sock;
	discovery_service *svc;
} discovery_data;

// Private and helper functions
static char* port_to_string(discovery_service *svc) {
	char *tmp = calloc(6, sizeof(char));
	snprintf(tmp, 6, "%d", svc->port);
	return tmp;
}

static void send_peer_packet(discovery_service *svc) {
	void *buffer;
	size_t len = peerton(svc->peers->local_peer, &buffer);
	jnx_uint8 *message = malloc(4 + len);
	memcpy(message, "PEER", 4);
	memcpy(message + 4, buffer, len);

	jnx_socket_udp_send(svc->sock_send, svc->broadcast_group_address, port_to_string(svc), message, len + 4);

	free(message);
	free(buffer
}
static void ndle_peer(discovery_service *svc, jnx_uint8 *payload, jnx_size bytesread) {
	peer *p = ntopeer(payload, bytesread);
	peerstore_store_peer(svc->peers, p);	
}

// *** Peer discovery strategies ***
jnx_int32 send_discovery_request(discovery_service *svc) {
	char *tmp = "LIST";
	char *port = port_to_string(svc);
	jnx_socket_udp_send(svc->sock_send, svc->broadcast_group_address, port, (jnx_uint8 *) tmp, 5);	
	return 0;
}

void *polling_update_loop(void *data) {
	discovery_service *svc = (discovery_service *) data;
	time_t next_update = time(0) + peer_update_timeout;
	while (1) {
		send_discovery_request(svc);
		sleep(next_update - time(0));
		next_update += peer_update_timeout;
	}
	return NULL;
}
jnx_int32 polling_update_strategy(discovery_service *svc) {
	return jnx_thread_create_disposable(polling_update_loop, (void *) svc);
}

void *broadcast_update_loop(void *data) {
	discovery_service *svc = (discovery_service *) data;
	time_t next_update = time(0) + peer_update_timeout;
	while (1) {
		send_peer_packet(svc);
		sleep(next_update - time(0));
		next_update += peer_update_timeout;
	}
	return NULL;
}
jnx_int32 broadcast_update_strategy(discovery_service *svc) {
	return jnx_thread_create_disposable(broadcast_update_loop, (void *) svc);
}
// Discovery listener and loop - async thread
static jnx_int32 discovery_receive_handler(jnx_uint8 *payload, jnx_size bytesread, jnx_socket *s, void *context) {
	char command[5];
	memset(command, 0, 5);
	memcpy(command, payload, 4);
	if (0 == strcmp(command, "LIST")) {
		discovery_service *svc = (discovery_service *) context;
		send_peer_packet(svc);
	}
	else if (0 == strcmp(command, "PEER")) {
		discovery_service *svc = (discovery_service *) context;
		handle_peer(svc, payload + 4, bytesread - 4);
	}
	else {
		JNX_LOG(0, "[DISCOVERY] Received unknown command. Ignoring the packet.");
	}
	free(payload);
	return 0;
}
static void *discovery_loop(void* data) {
	discovery_service *svc = (discovery_service*) data;
	char *port = port_to_string(svc);
	jnx_socket_udp_listen_with_context(svc->sock_receive, port, 0, svc->receive_callback, data);
	free(port);
	return NULL;
}
static jnx_int32 listen_for_discovery_packets(discovery_service *svc) {
	return jnx_thread_create_disposable(discovery_loop, (void*) svc);
}

// Public interface functions
discovery_service* discovery_service_create(int port, unsigned int family, char *broadcast_group_address, peerstore *peers) {
	discovery_service *svc = calloc(1, sizeof(discovery_service));
	svc->port = port;
	svc->family = family;
	svc->broadcast_group_address = broadcast_group_address;
	svc->receive_callback = discovery_receive_handler;
	svc->isrunning = 0;
	svc->peers = peers; 
	return svc;
}
int discovery_service_start(discovery_service *svc, discovery_strategy *strategy) {
	JNXCHECK(svc);
	svc->sock_receive = jnx_socket_udp_create(svc->family);
	jnx_socket_udp_enable_broadcast_send_or_listen(svc->sock_receive);
	svc->sock_send = jnx_socket_udp_create(svc->family);
	jnx_socket_udp_enable_broadcast_send_or_listen(svc->sock_send);

	if (0 != listen_for_discovery_packets(svc)) {
		JNX_LOG(0, "[DISCOVERY] Couldn't start the discovery listener.\n");
		return ERR_DISCOVERY_START;
	}

	if (strategy == NULL) {
		send_discovery_request(svc);
	}
	else {
		strategy(svc);
	}

	svc->isrunning = 1;
	return 0;
}
int discovery_service_stop(discovery_service *svc) {
	JNXCHECK(svc);
	jnx_socket_destroy(&(svc->sock_receive));
	jnx_socket_destroy(&(svc->sock_send));
	svc->isrunning = 0;
	return 0;
}
void discovery_service_cleanup(discovery_service **svc) {
	JNXCHECK(*svc);
	if ((*svc)->isrunning) {
		discovery_service_stop(*svc);
	}
	free(*svc);
	*svc = 0;
}

