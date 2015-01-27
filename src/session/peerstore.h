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

#include "data/peer.h"

typedef void peerstore;

int peerstore_store_peer(peerstore *ps, peer *p);

#endif
