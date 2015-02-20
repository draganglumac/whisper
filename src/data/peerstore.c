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
	store->last_update = time(0);
	store->is_active_peer = NULL;
	return (peerstore *) store;
}
peer *peerstore_get_local_peer(peerstore *ps) {
	return ps->local_peer;
}
void peerstore_store_peer(peerstore *ps, peer *p) {
	jnx_thread_lock(ps->store_lock);
	p->last_seen = time(0);
	char *guid_str;
	jnx_guid_to_string(&p->guid, &guid_str);
	peer *old = jnx_hash_get(PEERSTORE(ps->peers), guid_str);
	if (old != NULL) {
		peer_free(&old);
	}
	jnx_hash_put(PEERSTORE(ps->peers), guid_str, (void *) p);
	free(guid_str);
	jnx_thread_unlock(ps->store_lock);
}
void peerstore_destroy(peerstore **pps) {
	peerstore *ps = *pps;
	peer_free(&(ps->local_peer));
	
	jnx_hashmap *peers = PEERSTORE(ps->peers);
	const char **keys;
	int num_keys = jnx_hash_get_keys(peers, &keys); 
	int i;
	for (i = 0; i < num_keys; i++) {
		peer *temp = jnx_hash_get(peers, *(keys + i));
		peer_free(&temp);
	}
	jnx_hash_destroy(&peers);
	jnx_thread_mutex_destroy(&ps->store_lock);
	free(ps);
	pps = NULL;
}
peer *peerstore_lookup(peerstore *ps, jnx_guid *guid) {
	JNXCHECK(ps->is_active_peer);
	jnx_thread_lock(ps->store_lock);
	char *guid_str;
	jnx_guid_to_string(guid, &guid_str);
	peer *p = (peer *) jnx_hash_get(PEERSTORE(ps->peers), guid_str);
	if (p != NULL && !ps->is_active_peer(ps->last_update, p)) {
		jnx_hash_delete_value(PEERSTORE(ps->peers), guid_str);
		peer_free(&p);
		p = NULL;
	}
	free(guid_str);
	jnx_thread_unlock(ps->store_lock);
	return p;
}
int peerstore_get_active_guids(peerstore *ps, jnx_guid **guids) {
	jnx_thread_lock(ps->store_lock);
	jnx_hashmap *peers = PEERSTORE(ps->peers);
	const char **keys;
	int num_keys = jnx_hash_get_keys(peers, &keys); 
	int i, num_guids = 0;
	guids = calloc(num_guids, sizeof(jnx_guid *));
	for (i = 0; i < num_keys; i++) {
		peer *temp = jnx_hash_get(peers, *(keys + i));
		if (ps->is_active_peer(ps->last_update, temp)) {
			guids[num_guids] = &temp->guid;
			num_guids++;
		}	
	}
	jnx_thread_unlock(ps->store_lock);
	return num_guids;
}
void peerstore_set_last_update_time(peerstore *ps, time_t last_update) {
	jnx_thread_lock(ps->store_lock);
	ps->last_update = last_update;
	jnx_thread_unlock(ps->store_lock);
}