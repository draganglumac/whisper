/*
 * =====================================================================================
 *
 *       Filename:  peerstore.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/01/2015 08:07:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (DG), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxhash.h>
#include <jnxc_headers/jnxguid.h>
#include "peerstore.h"

#define PEERSTORE(x) ((jnx_hashmap *) x)

peerstore *peerstore_init(peer *local_peer) {
	peerstore *store = malloc(sizeof(peerstore));
	store->local_peer = local_peer;
	store->store_lock = jnx_thread_mutex_create();
	store->peers = (void *) jnx_hash_create(1024);
	return (peerstore *) store;
}
peer *peerstore_get_local_peer(peerstore *ps) {
	return ps->local_peer;
}
void peerstore_store_peer(peerstore *ps, peer *p) {
	jnx_thread_lock(ps->store_lock);
	
	jnx_list_add(PEERSTORE(ps->peers), p);
	jnx_thread_unlock(ps->store_lock);
}
void peerstore_destroy(peerstore *ps) {
	peer_free(&(ps->local_peer));
	peer *next;
	jnx_list *peers = PEERSTORE(ps->peers);
	while ((next = (peer *) jnx_list_remove_front(&peers)) != NULL) {
		peer_free(&next);
	}
	jnx_list_destroy(&peers);
	jnx_thread_mutex_destroy(&ps->store_lock);
	free(ps);
}
peer *peerstore_lookup(peerstore *ps, jnx_guid *guid) {
	jnx_thread_lock(ps->store_lock);
	jnx_list *peers = PEERSTORE(ps->peers);
	jnx_node *current = peers->head;
	while (current != NULL) {
		peer *curr_peer = (peer *) current->_data;
		if (jnx_guid_compare(guid, &(curr_peer->guid)) == JNX_GUID_STATE_SUCCESS) {
			jnx_thread_unlock(ps->store_lock);
			return curr_peer;
		}
		current = current->next_node;
	}
	jnx_thread_unlock(ps->store_lock);
	return NULL;
}
void peerstore_get_active_guids(peerstore *ps, jnx_guid *guids, int *num_guids) {
	jnx_list *temp = jnx_list_create();
	jnx_thread_lock(ps->store_lock);
	jnx_list *peers = (jnx_list *) ps->peers;
	jnx_node *curr = peers->head;
	while (curr != NULL) {
		peer *p = (peer *) curr->_data;
		jnx_list_add(temp, (void *) &p->guid);
	}
	jnx_thread_unlock(ps->store_lock);
}
