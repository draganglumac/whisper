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
#include <string.h>
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>
#include "data/peer.h"

void test_peer_serialisation() {
	peer p;

	for (int j = 0; j < 16; j++) {
		p.guid[j] = (uint8_t) 0x01;
	}	
	p.host_address = "127.0.0.1";
	p.public_key = "0123456789PublicKey";

	uint8_t *buffer;
	size_t length = peerton(&p, &buffer);

	JNX_LOG(0, "length = %d", length);
	for (int i = 0; i < length; i++) {
		printf("%02x ", buffer[i]);
		fflush(stdout);
	}
	printf("\n");

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
