/*
 * =====================================================================================
 *
 *       Filename:  test_serialisation.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/01/2015 15:42:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (DG), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>
#include "data/peer.h"

void test_peer_serialisation() {
	JNXCHECK(1 == 0);
}
void test_peer_deserialisation() {
	JNXCHECK(1 == 0);
}
int main(int argc, char **argv) {
	JNX_LOG(0, "Test peer to network serialisation.");
	test_peer_serialisation();

	JNX_LOG(0, "Test network to peer deserialisation.");
	test_peer_deserialisation();

	return 0;
}
