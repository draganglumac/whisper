/*
 * =====================================================================================
 *
 *       Filename:  discovery.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/01/15 14:57:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (draganglumac), dragan.glumac@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __DISCOVERY_H__
#define __DISCOVERY_H__

#include <jnxc_headers/jnxtypes.h>
#include <jnxc_headers/jnxsocket.h>

typedef struct {
	char *guid;
	char *ip;
} peer;

typedef struct {
	int port;
	unsigned int family;
	jnx_socket *sock_send;
	jnx_socket *sock_receive;
	char *broadcast_group_address;
	udp_socket_listener_callback receive_callback;
	int isrunning;
} discovery_service;

discovery_service* discovery_service_create(int port, unsigned int family, char *broadcast_group_address);
void discovery_service_cleanup(discovery_service **svc);
int discovery_service_start(discovery_service *svc);
int discovery_service_stop(discovery_service *svc);

#endif
