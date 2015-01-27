/*
 * =====================================================================================
 *
 *       Filename:  test_encoder.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/20/2015 08:31:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "session/session.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxtypes.h>

void session_create_destroy() {

  SessionObject s = SESSION_OBJECT__INIT;
  session_guid_create(&s);

  jnx_uint8 *obuffer;
  jnx_size size = session_pack(&s,&obuffer);
  JNXCHECK(obuffer);

  SessionObject *us = session_unpack(obuffer,size);
  free(obuffer);
  
  JNXCHECK(jnx_guid_compare_raw(s.guid,us->guid) == JNX_GUID_STATE_SUCCESS);
}
int main(int argc, char **argv) {

  session_create_destroy();
  return 0;
}
