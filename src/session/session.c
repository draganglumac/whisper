/*
 * =====================================================================================
 *
 *       Filename:  session.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/01/15 12:32:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxguid.h>
#include "../integrity/encoding.h"
#include <string.h>
#include "session.h"

void session_guid_create(SessionObject *s) {
  jnx_guid g;
  jnx_guid_create(&g);
  s->n_guid = sizeof(g.guid);
  s->guid = malloc(sizeof(jnx_uint64) * s->n_guid);
  memcpy(s->guid,g.guid,s->n_guid);
  jnx_char *ostr;
  jnx_guid_to_string(&g,&ostr);
  JNX_LOG(NULL,"New session %s\n",ostr);
  free(ostr);
}
jnx_size session_pack(SessionObject *s,jnx_uint8 **obuffer) {
  jnx_size size = session_object__get_packed_size(s);
  *obuffer = malloc(size);
  session_object__pack(s,*obuffer);
  return size;
}
SessionObject *session_unpack(jnx_uint8 *buffer,jnx_size len) {
  SessionObject *s=session_object__unpack(NULL,len,buffer);
  return s;
}
