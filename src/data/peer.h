/*
 * =====================================================================================
 *
 *       Filename:  peer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/01/2015 15:49:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (DG), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __PEER_H__
#define __PEER_H__

#include <time.h>

typedef struct {
	char *guid;
	char *host_address;
	char *public_key;
	time_t last_seen;
} peer;

peer *peer_local();
size_t peerton(peer *p, void **out);
peer *ntopeer(void *in, size_t len);

#endif
