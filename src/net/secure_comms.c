/*
 * secure_comms.c
 * Copyright (C) 2015 tibbar <tibbar@ubuntu>
 *
 * Distributed under terms of the MIT license.
 */

#include "secure_comms.h"
#include <jnxc_headers/jnxsocket.h>

void secure_comms_start(discovery_service *ds,
    session *s,jnx_unsigned_int addr_family){  
  JNXCHECK(s->is_connected);
  printf("Starting secure comms on %s.\n",s->secure_comms_port);
  
  jnx_socket *secure_sock = jnx_socket_tcp_create(addr_family);
  /* Not using standard jnx_socket networking here due to bespoke nature of 
   * bi directional socket with non-blocking write properties */


}

