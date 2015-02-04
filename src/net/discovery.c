/*
 * =====================================================================================
 *
 *       Filename:  discovery.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/01/15 16:14:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (draganglumac), dragan.glumac@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <sys/ioctl.h>
#include <jnxc_headers/jnxsocket.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxcheck.h>
#include "discovery.h"
#include "data/peer.h"
#include "err/whisper_errors.h"

typedef struct {
	jnx_socket *sock;
	discovery_service *svc;
} discovery_data;

// Private and helper functions
static char* port_to_string(discovery_service *svc) {
	char *tmp = calloc(6, sizeof(char));
	snprintf(tmp, 6, "%d", svc->port);
	return tmp;
}

static void safely_update_last_update_time(discovery_service *svc) {
	jnx_thread_lock(svc->update_time_lock);
	svc->last_updated = time(0);
	peerstore_set_last_update_time(svc->peers, svc->last_updated);
	jnx_thread_unlock(svc->update_time_lock);
}

static void send_peer_packet(discovery_service *svc) {
	void *buffer;
	size_t len = peerton(peerstore_get_local_peer(svc->peers), &buffer);
	jnx_uint8 *message = malloc(4 + len);
	memcpy(message, "PEER", 4);
	memcpy(message + 4, buffer, len);

	jnx_socket_udp_send(svc->sock_send, svc->broadcast_group_address, port_to_string(svc), message, len + 4);

	safely_update_last_update_time(svc);
	free(message);
	free(buffer);
}
static void handle_peer(discovery_service *svc, jnx_uint8 *payload, jnx_size bytesread) {
	peer *p = ntopeer(payload, bytesread);
	peerstore_store_peer(svc->peers, p);	
}

// *** Peer discovery strategies ***
int peer_update_interval = 10;

int is_active_peer_ask_once(time_t last_update_time, peer *p) {
	return 1;
}
int is_active_peer_periodic_update(time_t last_update_time, peer *p) {
	if (p->last_seen >= last_update_time - 2 * peer_update_interval) {
		return 1;
	}
	return 0;
}

jnx_int32 send_discovery_request(discovery_service *svc) {
	char *tmp = "LIST";
	char *port = port_to_string(svc);
	jnx_socket_udp_send(svc->sock_send, svc->broadcast_group_address, port, (jnx_uint8 *) tmp, 5);	
	return 0;
}

// Polling update strategy
void *polling_update_loop(void *data) {
	// The slightly more complicated logic is here to ensure that updates
	// do not happen more frequently than peer_update_interval on average.
	// 
	// This means that whichever node sends LIST packet last, all of the
	// discovery services on the network will synhronise to that time. Since
	// every time a LIST packet is received the service sends local peer packet,
	// we update the last_update time in send_peer_packet function. This way
	// we ensure that regradless of which service sends the LIST packets, all
	// services update the last_updated time, i.e. synchronise on that packet.
	//
	// The only time we may get a time shorter than peer_update_interval between
	// updates is when a new discovery service joins the broadcast group..
	int old_cancel_state;
	pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, &old_cancel_state);
	discovery_service *svc = (discovery_service *) data;
	time_t next_update = get_last_update_time(svc) + peer_update_interval;
	while (1) {
		if (!svc->isrunning) {
			return NULL;
		}
		if (next_update == get_last_update_time(svc) + peer_update_interval) {
			send_discovery_request(svc);
		}
		next_update = get_last_update_time(svc) + peer_update_interval;
		sleep(next_update - time(0));
	}
	return NULL;
}
jnx_int32 polling_update_strategy(discovery_service *svc) {
	svc->update_thread = jnx_thread_create_disposable(polling_update_loop, (void *) svc);
	return 0;
}

// Broadcast update strategy
void *broadcast_update_loop(void *data) {
	int old_cancel_state;
	pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, &old_cancel_state);
	discovery_service *svc = (discovery_service *) data;
	time_t next_update = time(0) + peer_update_interval;
	while (1) {
		if (!svc->isrunning) {
			return NULL;
		}
		send_peer_packet(svc);
		sleep(next_update - time(0));
		next_update += peer_update_interval;
	}
	return NULL;
}
jnx_int32 broadcast_update_strategy(discovery_service *svc) {
	svc->update_thread = jnx_thread_create_disposable(broadcast_update_loop, (void *) svc);
	return 0;
}

// Discovery listener and loop - async thread
static jnx_int32 discovery_receive_handler(jnx_uint8 *payload, jnx_size bytesread, jnx_socket *s, void *context) {
	discovery_service *svc = (discovery_service *) context;
	char command[5];
	memset(command, 0, 5);
	memcpy(command, payload, 4);
	if (0 == strcmp(command, "LIST")) {
		send_peer_packet(svc);
	}
	else if (0 == strcmp(command, "PEER")) {
		handle_peer(svc, payload + 4, bytesread - 4);
	}
	else {
		JNX_LOG(0, "[DISCOVERY] Received unknown command. Ignoring the packet.");
	}
	free(payload);
	return 0;
}
static void ensure_listening_on_port(int port) {
	// Until we find a smarter strategy just sleep a little
	sleep(1);
}
static void *discovery_loop(void* data) {
	int old_cancel_state;
	pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS, &old_cancel_state);
	discovery_service *svc = (discovery_service*) data;
	char *port = port_to_string(svc);
	jnx_socket_udp_listen_with_context(svc->sock_receive, port, 0, svc->receive_callback, data);
	free(port);
	return NULL;
}
static jnx_int32 listen_for_discovery_packets(discovery_service *svc) {
	int retval = jnx_thread_create_disposable(discovery_loop, (void*) svc);
	ensure_listening_on_port(svc->port);
	return retval;
}
static void cancel_thread(jnx_thread **thr) {
	jnx_thread *temp = *thr;
	pthread_cancel(temp->system_thread);
	pthread_join(temp->system_thread, NULL);
	jnx_thread_handle_destroy(temp);
	*thr = NULL;
}
static int stop_listening(discovery_service *svc) {
	int dont_block = 1; 
	return ioctl(svc->sock_receive, FIONBIO, (char *) &dont_block);
}

// Public interface functions
discovery_service* discovery_service_create(int port, unsigned int family, char *broadcast_group_address, peerstore *peers) {
	discovery_service *svc = calloc(1, sizeof(discovery_service));
	svc->port = port;
	svc->family = family;
	svc->broadcast_group_address = broadcast_group_address;
	svc->receive_callback = discovery_receive_handler;
	svc->isrunning = 0;
	svc->peers = peers;
	svc->update_thread = NULL;
	svc->last_updated = time(0);
	svc->update_time_lock = jnx_thread_mutex_create();	
	return svc;
}
int discovery_service_start(discovery_service *svc, discovery_strategy *strategy) {
	JNXCHECK(svc);
	svc->sock_receive = jnx_socket_udp_create(svc->family);
	jnx_socket_udp_enable_broadcast_send_or_listen(svc->sock_receive);
	svc->sock_send = jnx_socket_udp_create(svc->family);
	jnx_socket_udp_enable_broadcast_send_or_listen(svc->sock_send);

	if (0 != listen_for_discovery_packets(svc)) {
		JNX_LOG(0, "[DISCOVERY] Couldn't start the discovery listener.\n");
		return ERR_DISCOVERY_START;
	}

	if (strategy == NULL) {
		svc->peers->is_active_peer = is_active_peer_ask_once;
		send_discovery_request(svc);
	}
	else {
		svc->peers->is_active_peer = is_active_peer_periodic_update;
		strategy(svc);
	}

	svc->isrunning = 1;
	return 0;
}
int discovery_service_stop(discovery_service *svc) {
	JNXCHECK(svc);
	svc->isrunning = 0;
	stop_listening(svc);
	if (svc->update_thread != NULL) {
		cancel_thread(svc->update_thread);
	}
	jnx_socket_destroy(&(svc->sock_receive));
	jnx_socket_destroy(&(svc->sock_send));
	return 0;
}
void discovery_service_cleanup(discovery_service **ppsvc) {
	discovery_service *svc = *ppsvc;
	JNXCHECK(svc);
	if (svc->isrunning) {
		discovery_service_stop(svc);
	}
	jnx_thread_mutex_destroy(&svc->update_time_lock);
	peerstore_destroy(&(svc->peers));
	free(svc);
	*ppsvc = 0;
}
time_t get_last_update_time(discovery_service *svc) {
	jnx_thread_lock(svc->update_time_lock);
	time_t last_update = svc->last_updated;
	jnx_thread_unlock(svc->update_time_lock);
	return last_update;
}
