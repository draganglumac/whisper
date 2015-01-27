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

typedef void peerstore;

peer* peerstore_get_local_peer(peerstore *ps);
int peerstore_store_peer(peerstore *ps, peer *p);

#endif
