/*
 * secure_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#include "secure_comms.h"
#include <jnxc_headers/jnxsocket.h>

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

      break;

    case SC_RECEIVER:

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

