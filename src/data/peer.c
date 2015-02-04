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
#include <jnxc_headers/jnxlog.h>
#include "peer.h"
#include "peer.pb-c.h"

size_t peerton(peer *p, void **out) {
	Peer msg = PEER__INIT;
	size_t len;

	// required bytes guid=1;
	msg.guid.len = 16;
	msg.guid.data = malloc(sizeof(char) * msg.guid.len);
	int i;
	for (i = 0; i < msg.guid.len; i++) {
		msg.guid.data[i] = p->guid.guid[i];
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
	msg = peer__unpack(NULL, len, in);
	if (NULL == msg) {
		JNX_LOG(NULL, "Error unpacking a peer message.");
		return NULL;
	}

	jnx_guid gd;
	memcpy(gd.guid, msg->guid.data, msg->guid.len);	
	peer *p = peer_create(gd, msg->host_address, msg->public_key); 
	peer__free_unpacked(msg, NULL);
	return p;	
}
peer *peer_create(jnx_guid guid, char *host_address, char *public_key) {
	JNXCHECK(host_address);
	peer *temp = malloc(sizeof(peer));
	
	memcpy(temp->guid.guid, guid.guid, 16);
	temp->host_address = malloc(1 + strlen(host_address));
	strcpy(temp->host_address, host_address);
	if (NULL != public_key && strlen(public_key) > 0) {
		temp->public_key = malloc(1 + strlen(public_key));
		strcpy(temp->public_key, public_key);
	}
	return temp;
}
void peer_free(peer **p) {
	peer *temp = *p;
	free(temp->host_address);
	if (NULL != temp->public_key) {
		free(temp->public_key);
	}
	free(temp);
	*p = NULL;
}
