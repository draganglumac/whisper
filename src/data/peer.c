/*
 * =====================================================================================
 *
 *       Filename:  peer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  27/01/2015 15:51:07
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
#include <jnxc/jxnlog.h>
#include "peer.h"
#include "peer.pb-c.h"

peer *peer_local() {
	exit(1);
}

size_t peerton(peer *p, void **out) {
	Peer msg = PEER__INIT;
	size_t len;

	// required bytes guid=1;
	msg.guid.len = 16;
	msg.guid.data = malloc(sizeof(char) * msg.guid.len);
	for (int i = 0; i < msg.guid.len; i++) {
		msg.guid.data[i] = p->guid[i];
	}

	// required string host_address=2;
	msg.host_address = malloc(1 + sizeof(char) * strlen(p->host_address));
	strcpy(msg.host_address, p->host_address);

	// optional string public_key=3;
	if (0 != p->public_key) {
		msg.public_key = malloc(1 + sizeof(char) * strlen(p->public_key));
		strcpy(msg.public_key, p->public_key);
	}

	len = peer__get_packed_size(&msg);
	*out = malloc(len);
	peer__pack(&msg, *out);
	
	return len;
}

peer *ntopeer(void *in, size_t len) {
	Peer *msg;
	peer *peer = calloc(1, sizeof(peer));

	msg = peer__unpack(NULL, len, in);
	if (NULL == msg) {
		JNX_LOG(NULL, "Error unpacking a peer message.");
		return NULL;
	}

	// required bytes guid=1;
	memcpy(p->guid, msg.guid.data, msg.guid.len);

	// required string host_address=2;
	p->host_address = malloc(1 + strlen(msg.host_address));
	strcpy(p->host_address, msg.host_address);

	// optional string public_key=3;
	if (strlen(msg.public_key) > 0) {
		p->public_key = malloc(1 + strlen(msg.public_key));
		strcpy(p->public_key, msg.public_key);
	}

	peer__free_unpacked(msg, NULL);
	
	return peer;	
}
