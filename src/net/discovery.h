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
#include <jnxc_headers/jnxthread.h>
#include <time.h>
#include "data/peer.h"
#include "data/peerstore.h"

typedef struct {
	int port;
	unsigned int family;
	jnx_socket *sock_send;
	jnx_socket *sock_receive;
	char *broadcast_group_address;
	udp_socket_listener_callback_with_context receive_callback;
	int isrunning;
	peerstore *peers; // synchronised
	jnx_thread *listening_thread;	
	jnx_thread *update_thread;
	time_t last_updated; // synchronised
	jnx_thread_mutex *update_time_lock;
} discovery_service;

typedef jnx_int32 (discovery_strategy)(discovery_service *);
extern discovery_strategy polling_update_strategy;
extern discovery_strategy broadcast_update_strategy;

extern int peer_update_interval; // seconds

#define ASK_ONCE_STRATEGY NULL
#define POLLING_UPDATE_STRATEGY polling_update_strategy
#define BROADCAST_UPDATE_STRATEGY broadcast_update_strategy

discovery_service* discovery_service_create(int port, unsigned int family, char *broadcast_group_address, peerstore *peers);
void discovery_service_cleanup(discovery_service **svc);
int discovery_service_start(discovery_service *svc, discovery_strategy *strategy);
int discovery_service_stop(discovery_service *svc);
time_t get_last_update_time(discovery_service *svc);

#endif
