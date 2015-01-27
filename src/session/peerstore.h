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

#include <time.h>

typedef struct {
	char *guid;
	char *host;
	char *public_key;
	time_t last_seen;
} peer;

typedef void peerstore;

int peerstore_store_peer(peerstore *ps, peer *p);

#endif
