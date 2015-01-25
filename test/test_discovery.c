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

void test_service_creation() {
	discovery_service *svc = 0;
	JNXCHECK(svc == NULL);
	svc = discovery_service_create(1234, AF_INET, baddr);
	JNXCHECK(svc != NULL);
	JNXCHECK(svc->port == 1234);
	JNXCHECK(svc->sock_send->socket > 0 
			&& svc->sock_send->addrfamily == AF_INET
			&& svc->sock_send->stype == SOCK_DGRAM);
	JNXCHECK(svc->sock_receive->socket > 0 
			&& svc->sock_receive->addrfamily == AF_INET
			&& svc->sock_receive->stype == SOCK_DGRAM);
	JNXCHECK(svc->running == 0);
}
void test_service_cleanup() {
	discovery_service *svc;
	svc = discovery_service_create(1234, AF_INET, baddr);
	discovery_service_cleanup(svc);
	JNXCHECK(svc->port == 0);
	JNXCHECK(svc->sock_send == 0);
	JNXCHECK(svc->sock_receive == 0);
	JNXCHECK(svc->running == 0);
}
static int service_started = 0;
int starting_service_spy(char *message, int len, jnx_socket *s) {
	JNXCHECK(strcmp(message, "LIST") == 0);
	service_started = 1;
	return 0;
}
void test_starting_service() {
	discovery_service *svc;
	svc = discovery_service_create(1234, AF_INET, baddr);
	svc->receive_callback = starting_service_spy;
	int retval = discovery_service_start(svc);
	JNXCHECK(retval == 0);
	if (svc->running) {
		// check send socket is up and listening
		int error = 0;
		socklen_t len = sizeof(error);
		retval = getsockopt(svc->sock_send->socket, SOL_SOCKET, SO_ERROR, &error, &len);
		JNXCHECK(retval == 0);
	}

	while (!service_started) {
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	service_started = 0;
}
void test_stopping_service() {
	//	JNXCHECK(1 == 0);
}
int main(int argc, char **argv) {
	get_broadcast_address(baddr);
	
	JNX_LOG(NULL,"Test service creation.");
	test_service_creation();
	JNX_LOG(NULL,"Test service cleanup.");
	test_service_cleanup();
	JNX_LOG(NULL, "Test starting discovery service.");
	test_starting_service();
	JNX_LOG(NULL, "Test stopping discovery service.");
	test_stopping_service();
	return 0;
}
