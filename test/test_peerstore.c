/*
 * =====================================================================================
 *
 *       Filename:  test_peerstore.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24/02/2015 16:35:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>
#include "data/peer.h"
#include "data/peerstore.h"

peer *local_test_peer() {
  jnx_guid guid;
  int i;
  for (i = 0; i < 16; i++) {
    guid.guid[i] = i;
  }
  return peer_create(guid, "127.0.0.1", "LocalUser");
}
peer *other_test_peer() {
  jnx_guid guid;
  int i;
  for (i = 0; i < 16; i++) {
    guid.guid[i] = 15;
  }
  return peer_create(guid, "127.0.0.2", "OtherUser");
}
peerstore *create_test_peerstore() {
  peerstore *store = peerstore_init(local_test_peer(), 0);
  return store;
}
void test_peerstore_lookup_by_username() {
  peerstore *ps = create_test_peerstore();
  peerstore_store_peer(ps, local_test_peer());
  peerstore_store_peer(ps, other_test_peer());

  peer *other = peerstore_lookup_by_username(ps, "OtherUser");
  peer *local = peerstore_lookup_by_username(ps, "LocalUser");
  
  int status = peers_compare(other, peerstore_get_local_peer(ps));
  JNXCHECK(status == PEERS_DIFFERENT);
  status = peers_compare(local, peerstore_get_local_peer(ps));
  JNXCHECK(status == PEERS_EQUIVALENT);
  peerstore_destroy(&ps);
}
int main() {
  JNX_LOG(0, "test_peerstore_lookup_by_username");
  test_peerstore_lookup_by_username();
  
  return 0;
}
