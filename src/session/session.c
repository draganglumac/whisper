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

void session_create() {

  jnx_guid g;
  jnx_guid_create(&g);
  jnx_char *ostr;
  jnx_guid_to_string(&g,&ostr);
  JNX_LOG(NULL,"Created new session %s\n",ostr);
  free(ostr);



}
