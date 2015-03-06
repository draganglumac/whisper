/*
 * secure_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#include "secure_comms.h"
#include <jnxc_headers/jnxsocket.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
static int listen_for_socket_fd(jnx_socket *s, peer *remote_peer,session *ses) {
  jnx_int32 optval = 1;
  struct addrinfo hints, *res, *p;
  struct sockaddr_storage their_addr;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;
  hints.ai_flags = AI_PASSIVE;
  JNXCHECK(getaddrinfo(NULL,ses->secure_comms_port,&hints,&res) == 0);
  p = res;
  while(p != NULL) {
    if (setsockopt(s->socket,
          SOL_SOCKET,
          SO_REUSEADDR,
          &optval,sizeof(jnx_int32)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
      perror("server: bind");
      return -1;
    }
    break;
    p = p->ai_next;
  }
  freeaddrinfo(res);
  listen(s->socket,1);
  socklen_t addr_size = sizeof(their_addr);
  return accept(s->socket,(struct sockaddr*)&their_addr,&addr_size);
}
void secure_comms_start(secure_comms_endpoint e, discovery_service *ds,
    session *s,jnx_unsigned_int addr_family) {
  JNXCHECK(s->is_connected);
  printf("Starting secure comms on %s.\n",s->secure_comms_port);

  peer *remote_peer = peerstore_lookup(ds->peers,&(*s).remote_peer_guid);
  printf("Starting a tunnel to %s\n",remote_peer->host_address);

  jnx_socket *secure_sock = jnx_socket_tcp_create(addr_family);
  /* Not using standard jnx_socket networking here due to bespoke nature of 
   * bi directional socket with non-blocking write properties */
  switch(e) {

    case SC_INITIATOR:
      printf("About to initiate connection to remote secure_comms_port.\n");
      sleep(3);
      break;

    case SC_RECEIVER:
      printf("Setting up recevier.\n");
      jnx_int sockfd = listen_for_socket_fd(secure_sock,remote_peer,s);     
      JNXCHECK(sockfd != -1);
      s->secure_comms_fd = sockfd;
      break;
  }


} 
void secure_comms_receiver_start(discovery_service *ds,
    session *s,jnx_unsigned_int addr_family) {
  secure_comms_start(SC_RECEIVER,ds,s,addr_family);
}
void secure_comms_initiator_start(discovery_service *ds,
    session *s,jnx_unsigned_int addr_family) {
  secure_comms_start(SC_INITIATOR,ds,s,addr_family);
} 

