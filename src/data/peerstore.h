/*
 * =====================================================================================
 *
 *       Filename:  peerstore.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/01/2015 08:07:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (DG), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __PEERSTORE_H__
#define __PEERSTORE_H__

#include <jnxc_headers/jnxthread.h>
#include "peer.h"

typedef struct {
	peer *local_peer;
	jnx_thread_mutex *store_lock;
	void *peers;
} peerstore;

peerstore *peerstore_init(peer *local_peer);
peer *peerstore_get_local_peer(peerstore *ps);
void peerstore_store_peer(peerstore *ps, peer *p);
void peerstore_destroy(peerstore *ps);

#endif
