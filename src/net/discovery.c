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
#include "err/whisper_errors.h"

// Private and helper functions
static char* port_to_string(discovery_service *svc) {
	char *tmp = calloc(6, sizeof(char));
	snprintf(tmp, 6, "%d", svc->port);
	return tmp;
}

// Discovery listener and loop - async thread
static jnx_int32 discovery_receive_handler(jnx_uint8 *payload, jnx_size bytesread, jnx_socket *s) {
	JNX_LOG(0, "[DISCOVERY] %s\n", payload);
}
static void* discovery_loop(void* data) {
	discovery_service *svc = (discovery_service*) data;
	char *port = port_to_string(svc);
	int retval = jnx_socket_udp_listen(svc->sock_receive, port, 0, svc->receive_callback);
	free(port);
	return retval;
}
static jnx_int32 listen_for_discovery_packets(discovery_service *svc) {
	return jnx_thread_create_disposable(discovery_loop, (void*) svc);
}

// Discovery requests
static jnx_int32 send_discovery_request(discovery_service *svc) {
	char *tmp = "LIST";
	char *port = port_to_string(svc);
	jnx_socket_udp_send(svc->sock_send, svc->broadcast_group_address, port, tmp, 5);	
	return 0;
}

// Public interface functions
discovery_service* discovery_service_create(int port, unsigned int family, char *broadcast_group_address) {
	discovery_service *svc = calloc(1, sizeof(discovery_service));
	svc->port = port;
	svc->family = family;
	svc->broadcast_group_address = broadcast_group_address;
	svc->receive_callback = discovery_receive_handler;
	svc->isrunning = 0;
	return svc;
}
int discovery_service_start(discovery_service *svc) {
	JNXCHECK(svc);
	svc->sock_receive = jnx_socket_udp_create(svc->family);
	jnx_socket_udp_enable_broadcast_send_or_listen(svc->sock_receive);
	svc->sock_send = jnx_socket_udp_create(svc->family);
	jnx_socket_udp_enable_broadcast_send_or_listen(svc->sock_send);
	
	if (0 != listen_for_discovery_packets(svc)) {
		JNX_LOG(0, "[DISCOVERY] Couldn't start the discovery listener.\n");
		return ERR_DISCOVERY_START;
	}
	send_discovery_request(svc);

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

