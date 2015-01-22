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
#include "discovery.h"

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
