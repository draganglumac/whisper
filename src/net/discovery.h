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
	jnx_socket *sock_send;
	jnx_socket *sock_receive;
} discovery_service;

discovery_service* discovery_service_create(int port, jnx_unsigned_int family);

#endif
