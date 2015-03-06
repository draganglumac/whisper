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
#include <jnxc_headers/jnxthread.h>

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
static int connect_for_socket_fd(jnx_socket *s, peer *remote_peer,session *ses) {
  struct addrinfo hints, *res;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;

  jnx_int32 rg = 0;

  if((rg = getaddrinfo(remote_peer->host_address,ses->secure_comms_port,&hints,&res)) != 0) {
    JNX_LOG(DEFAULT_CONTEXT,"%s\n",gai_strerror(rg));
    return -1;
  }
  if(!s->isconnected) {
    if(connect(s->socket,res->ai_addr,res->ai_addrlen) != 0) {
      perror("connect:");
      freeaddrinfo(res);
      return 1;
    }
    s->isconnected = 1;
  }
  freeaddrinfo(res);
  return s->socket;
}
typedef struct {
  jnx_int sockfd;
  session *s;
}secure_comms_dto;
void *comms_listener_bootrap(void *args) {
  secure_comms_dto *d = (secure_comms_dto*)args;

  jnx_char buffer[1024];
  memset(buffer,0,1024);
  while(1) {
    jnx_int bytes_read = read(d->sockfd,buffer,1024);
    if(bytes_read > 0) {
      jnx_size len = strlen(buffer);
      jnx_size olen;
      jnx_char *decrypted = symmetrical_decrypt(d->s->shared_secret,buffer,len,&olen);
      printf("Decrypted message => %s\n",decrypted);
    }
  }
  free(d);
}
void secure_comms_start(secure_comms_endpoint e, discovery_service *ds,
    session *s,jnx_unsigned_int addr_family) {
  JNXCHECK(s->is_connected);
  printf("Starting secure comms on %s.\n",s->secure_comms_port);

  peer *local_peer = peerstore_get_local_peer(ds->peers);
  peer *remote_peer = peerstore_lookup(ds->peers,&(*s).remote_peer_guid);
  printf("Starting a tunnel to %s\n",remote_peer->host_address);

  jnx_socket *secure_sock = jnx_socket_tcp_create(addr_family);
  /* Not using standard jnx_socket networking here due to bespoke nature of 
   * bi directional socket with non-blocking write properties */
  jnx_int sockfd = -1;
  switch(e) {

    case SC_INITIATOR:
      printf("About to initiate connection to remote secure_comms_port.\n");
      sleep(3);
      sockfd = connect_for_socket_fd(secure_sock,remote_peer,s); 
      s->secure_comms_fd = sockfd;
      printf("Secure socket fd: %d\n",s->secure_comms_fd);
      break;

    case SC_RECEIVER:
      printf("Setting up recevier.\n");
      sockfd = listen_for_socket_fd(secure_sock,remote_peer,s);     
      JNXCHECK(sockfd != -1);
      s->secure_comms_fd = sockfd;
      printf("Secure socket fd: %d\n",s->secure_comms_fd);
      break;
  }
  JNXCHECK(sockfd != -1);
  /* At this point both the initiator and receiver are equal and have fd's relevent to them 
   * that are connected */

  secure_comms_dto *dto = malloc(sizeof(secure_comms_dto));
  dto->sockfd = sockfd;
  dto->s = s;
  /* Let's thread out a receiver for both sockets */
  jnx_thread_create_disposable(comms_listener_bootrap,(void*)dto);

  /* lets test this */
  sleep(3);
  char buffer[256];
  bzero(buffer,256);
  strcpy(buffer,"Hello from ");
  strcat(buffer,local_peer->host_address);

  jnx_size len = strlen(buffer);
  jnx_size olen;
  jnx_char *encrypted_string = symmetrical_encrypt(s->shared_secret,buffer,len,&olen); 

  write(s->secure_comms_fd,encrypted_string,olen);
  free(encrypted_string);
} 
void secure_comms_receiver_start(discovery_service *ds,
    session *s,jnx_unsigned_int addr_family) {
  secure_comms_start(SC_RECEIVER,ds,s,addr_family);
}
void secure_comms_initiator_start(discovery_service *ds,
    session *s,jnx_unsigned_int addr_family) {
  secure_comms_start(SC_INITIATOR,ds,s,addr_family);
} 

