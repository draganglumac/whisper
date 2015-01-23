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
	int retval = jnx_socket_udp_listen(svc->sock_receive, port, 0, discovery_receive_handler);
	free(port);
	return retval;
}
static jnx_int32 listen_for_discovery_packets(discovery_service *svc) {
	return jnx_thread_create_disposable(discovery_loop, (void*) svc);
}
static jnx_int32 send_discovery_request(discovery_service *svc) {
	JNX_LOG(0, "[TODO] send_discovery_request\n");
	return 0;
}
// Public interface functions
discovery_service* discovery_service_create(int port, unsigned int family) {
	discovery_service *svc = calloc(1, sizeof(discovery_service));
	svc->port = port;
	svc->sock_send = jnx_socket_udp_create(family);
	svc->sock_receive = jnx_socket_udp_create(family);
	svc->running = 0;
	return svc;
}
void discovery_service_cleanup(discovery_service *svc) {
	jnx_socket_destroy(&(svc->sock_send));
	jnx_socket_destroy(&(svc->sock_receive));
	svc->port = 0;
	svc->running = 0;
}
int discovery_service_start(discovery_service *svc) {
	jnx_socket_udp_enable_broadcast_send_or_listen(svc->sock_receive);
	if (0 != listen_for_discovery_packets(svc)) {
		JNX_LOG(0, "[DISCOVERY] Couldn't start the discovery listener.\n");
		return ERR_DISCOVERY_START;
	}

	jnx_socket_udp_enable_broadcast_send_or_listen(svc->sock_send);
	send_discovery_request(svc);
	
	return 0;
}
int discovery_service_stop(discovery_service *svc) {
	exit(1);
}


