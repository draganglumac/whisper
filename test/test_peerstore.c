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

peer *create_fake_local_peer() {
  jnx_guid guid;
  int i;
  for (i = 0; i < 16; i++) {
    guid.guid[i] = 1;
  }
  return peer_create(guid, "127.0.0.1", "UserName");
}
int active(time_t lut, peer *p) {
  return 1;
}
peerstore *create_test_peerstore() {
  peerstore *store = peerstore_init(create_fake_local_peer());
  store->is_active_peer = active;
  return store;
}
void test_peerstore_create() {
  peerstore *ps = create_test_peerstore();
  peerstore_store_peer(ps, create_fake_local_peer());
  peer *result = peerstore_lookup_by_username(ps, "UserName");
  JNXCHECK(result == peerstore_get_local_peer(ps));
  peerstore_destroy(&ps);
}
int main() {
  JNX_LOG(0, "test_peerstore_create");
  test_peerstore_create();
}
