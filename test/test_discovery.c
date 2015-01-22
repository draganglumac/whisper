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
#include "net/discovery.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>

void test_service_creation() {
	discovery_service *svc = 0;
	JNXCHECK(svc == NULL);
	svc = discovery_service_create(1234, AF_INET);
	JNXCHECK(svc != NULL);
	JNXCHECK(svc->port == 1234);
	JNXCHECK(svc->sock_send->socket > 0 
			&& svc->sock_send->addrfamily == AF_INET
			&& svc->sock_send->stype == SOCK_DGRAM);
	JNXCHECK(svc->sock_receive->socket > 0 
			&& svc->sock_receive->addrfamily == AF_INET
			&& svc->sock_receive->stype == SOCK_DGRAM);
}
void test_service_cleanup() {
	discovery_service *svc = discovery_service_create(1234, AF_INET);
	discovery_service_cleanup(svc);
	JNXCHECK(svc->port == 0);
	JNXCHECK(svc->sock_send == 0);
	JNXCHECK(svc->sock_receive == 0);
}
int main(int argc, char **argv) {
  JNX_LOG(NULL,"Test service creation.");
  test_service_creation();
  JNX_LOG(NULL,"Test service cleanup.");
  test_service_cleanup();
  return 0;
}
