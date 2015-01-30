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
#include "peerstore.h"

#define PEERSTORE(x) ((jnx_list *) x)

peerstore *peerstore_init(peer *local_peer) {
	peerstore *store = malloc(sizeof(peerstore));
	store->local_peer = local_peer;
	store->store_lock = jnx_thread_mutex_create();
	store->peers = (void *) jnx_list_create();
	return (peerstore *) store;
}
peer *peerstore_get_local_peer(peerstore *ps) {
	return ps->local_peer;
}
void peerstore_store_peer(peerstore *ps, peer *p) {
	jnx_list_add(PEERSTORE(ps->peers), p);
}
void peerstore_destroy(peerstore *ps) {
	peer_free(&(ps->local_peer));
	peer *next;
	jnx_list *peers = PEERSTORE(ps->peers);
	while ((next = (peer *) jnx_list_remove_front(&peers)) != NULL) {
		peer_free(&next);
	}
	jnx_list_destroy(&peers);
	free(ps);
}
peer *peerstore_lookup(peerstore *ps, jnx_guid *guid) {
	jnx_list *peers = PEERSTORE(ps->peers);
	jnx_node *current = peers->head;
	while (current != NULL) {
		peer *curr_peer = (peer *) current->_data;
		if (jnx_guid_compare(guid, &(curr_peer->guid)) == JNX_GUID_STATE_SUCCESS) {
			return curr_peer;
		}
		current = current->next_node;
	}
	return NULL;
}
