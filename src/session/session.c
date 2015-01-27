/*
 * =====================================================================================
 *
 *       Filename:  session.c
 *
 *    Description:  
 *
 *        Version:  1.0
<<<<<<< HEAD
 *        Created:  25/01/2015 20:51:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com)
=======
 *        Created:  26/01/15 12:32:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
>>>>>>> upstream/master
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxlog.h>
#include "../integrity/encoding.h"
#include <string.h>
#include "session.h"

session_ref* session_create() {

  jnx_guid g;
  jnx_guid_create(&g);
  session_ref *s = malloc(sizeof(session_ref));
  memcpy(&(*s).guid_ref,&g,sizeof g);

  jnx_char *ostr;
  jnx_guid_to_string(&(*s).guid_ref,&ostr);
  JNX_LOG(NULL,"Created new session %s\n",ostr);
  free(ostr);
 

  return s;
}
