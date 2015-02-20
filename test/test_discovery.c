/*
 * =====================================================================================
 *
 *       Filename:  test_discovery.c
 *
 *    Description:  :
 *
 *        Version:  1.0
 *        Created:  21/01/2015 08:39:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (DG), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <string.h>
#include "net/discovery.h"
#include "data/peer.h"
#include "data/peerstore.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>

static char baddr[16];
void get_broadcast_address(char *buffer) {
	struct ifaddrs *ifap;
	if (0 != getifaddrs(&ifap)) {
		JNX_LOG(0, "[ERROR] Couldn't get descriptions of network interfaces.");
		exit(1);
	}

	struct ifaddrs *current = ifap;
	while (0 != current) {
		struct sockaddr *ip = current->ifa_addr;
		if (ip->sa_family == AF_INET) {
			struct sockaddr *netmask = current->ifa_netmask;
			char *aip = inet_ntoa(((struct sockaddr_in *) ip)->sin_addr);
			if (strcmp("127.0.0.1", aip) == 0) {
				current = current->ifa_next;
				continue;
			}
			((struct sockaddr_in *) ip)->sin_addr.s_addr |= ~(((struct sockaddr_in *) netmask)->sin_addr.s_addr);
			char *broadcast = inet_ntoa(((struct sockaddr_in *) ip)->sin_addr);
			strncpy(buffer, broadcast, strlen(broadcast) + 1);
			JNX_LOG(0, "Broadcast IP is %s", buffer); 
			break;
		} 
		current = current->ifa_next;
	}

	freeifaddrs(ifap);
}
void update_time_checks(discovery_service *svc) {
	time_t last_update_time = get_last_update_time(svc);
	int i;
	for (i = 0; i < 5; i++) {
		printf(".");
		fflush(stdout);
		sleep(peer_update_interval);
		time_t temp = get_last_update_time(svc);
		JNXCHECK(temp > last_update_time);
		last_update_time = temp;
	}
	printf("\n");
}

void wait_for_flag(int *flag) {
	while (!(*flag)) {
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	*flag = 0;
}

static int list_message_received = 0;
int starting_service_spy(char *message, int len, jnx_socket *s, void *context) {
	JNXCHECK(strcmp(message, "LIST") == 0);
	list_message_received = 1;
	return -1;
}

// *** Discovery service test runner ***
#define CLEANUP 0
#define NO_CLEANUP 1
typedef int (*discovery_test)(discovery_service *);

void run_discovery_service_test(discovery_test test) {
	jnx_guid guid;
	int i;
	for (i = 0; i < 16; i++) {
		guid.guid[i] = 1;
	}
	peerstore *store = peerstore_init(peer_create(guid, "127.0.0.1", "UserName"));
	discovery_service *svc = discovery_service_create(1234, AF_INET, baddr, store);
	int retval = test(svc);
	if (retval == CLEANUP) {
		discovery_service_cleanup(&svc);
	}
}

// *** Tests ***
int test_service_creation(discovery_service *svc) {
	JNXCHECK(svc != NULL);
	JNXCHECK(svc->port == 1234);
	JNXCHECK(svc->family == AF_INET);
	JNXCHECK(svc->isrunning == 0);
	JNXCHECK(svc->sock_send == NULL);
	JNXCHECK(svc->sock_receive == NULL);
	return CLEANUP;
}
int test_service_cleanup(discovery_service *svc) {
	discovery_service_cleanup(&svc);
	JNXCHECK(svc == 0);
	return NO_CLEANUP;
}
int test_starting_service(discovery_service *svc) {
	svc->receive_callback = starting_service_spy;
	int retval = discovery_service_start(svc, ASK_ONCE_STRATEGY);
	JNXCHECK(retval == 0);
	JNXCHECK(svc->sock_send->socket > 0 
			&& svc->sock_send->addrfamily == AF_INET
			&& svc->sock_send->stype == SOCK_DGRAM);
	JNXCHECK(svc->sock_receive->socket > 0 
			&& svc->sock_receive->addrfamily == AF_INET
			&& svc->sock_receive->stype == SOCK_DGRAM);

	if (svc->isrunning) {
		// check send socket is up and listening
		int error = 0;
		socklen_t len = sizeof(error);
		retval = getsockopt(svc->sock_send->socket, SOL_SOCKET, SO_ERROR, &error, &len);
		JNXCHECK(retval == 0);
	}

	wait_for_flag(&list_message_received);
	return CLEANUP;
}
int test_stopping_service(discovery_service *svc) {
	test_starting_service(svc);
	discovery_service_stop(svc);
	JNXCHECK(svc->sock_receive == 0);
	JNXCHECK(svc->sock_send == 0);
	JNXCHECK(svc->isrunning == 0);
	list_message_received = 0;
	return CLEANUP;
}
int test_restarting_service(discovery_service *svc) {
	test_starting_service(svc);
	discovery_service_stop(svc);
	test_starting_service(svc);
	return CLEANUP;
}

int test_peer_packet_sent_after_list_packet_received(discovery_service *svc) {
	jnx_guid *guid = &(svc->peers->local_peer->guid);
	discovery_service_start(svc, ASK_ONCE_STRATEGY);
	JNXCHECK(NULL == peerstore_lookup(svc->peers, guid));
	while (NULL == peerstore_lookup(svc->peers, guid)) {
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	return CLEANUP;
}
int test_setting_peer_update_interval(discovery_service *svc) {
	JNXCHECK(peer_update_interval == 10);
	peer_update_interval = 20;
	JNXCHECK(peer_update_interval == 20);
	peer_update_interval = 10;
	JNXCHECK(peer_update_interval == 10);
	return CLEANUP;
}
int test_polling_update_strategy(discovery_service *svc) {
	peer_update_interval = 1;
	discovery_service_start(svc, POLLING_UPDATE_STRATEGY);
	sleep(2);
	update_time_checks(svc);
	return CLEANUP;
}
int test_broadcast_update_strategy(discovery_service *svc) {
	peer_update_interval = 1;
	discovery_service_start(svc, BROADCAST_UPDATE_STRATEGY);
	sleep(2);
	update_time_checks(svc);
	return CLEANUP;
}
int main(int argc, char **argv) {
	get_broadcast_address(baddr);
	
	JNX_LOG(NULL,"Test service creation.");
	run_discovery_service_test(test_service_creation);
	
	JNX_LOG(NULL,"Test service cleanup.");
	run_discovery_service_test(test_service_cleanup);
	
	JNX_LOG(NULL, "Test starting discovery service.");
	run_discovery_service_test(test_starting_service);
	
	JNX_LOG(NULL, "Test stopping discovery service.");
	run_discovery_service_test(test_stopping_service);

	JNX_LOG(NULL, "Test restarting discovery service.");
	run_discovery_service_test(test_restarting_service);

	JNX_LOG(NULL, "Test PEER packet is sent after discovery service recieves LIST packet.");
	run_discovery_service_test(test_peer_packet_sent_after_list_packet_received);	
	
	JNX_LOG(NULL, "Test setting peer_update_interval global variable.");
	run_discovery_service_test(test_setting_peer_update_interval);
	
	JNX_LOG(NULL, "Test polling update strategy.");
	run_discovery_service_test(test_polling_update_strategy);

	JNX_LOG(NULL, "Test broadcast update strategy.");
	run_discovery_service_test(test_broadcast_update_strategy);
	return 0;
}