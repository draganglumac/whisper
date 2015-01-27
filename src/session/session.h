/*
 * =====================================================================================
 *
 *       Filename:  session.h
 *
 *    Description:  
 *
 *        Version:  1.0
<<<<<<< HEAD
 *        Created:  25/01/2015 20:50:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com)
=======
 *        Created:  26/01/15 12:02:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
>>>>>>> upstream/master
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __SESSION_H__
#define __SESSION_H__

#include "session_object.pb-c.h"
#include <jnxc_headers/jnxguid.h>

typedef struct session_ref {
  /*
   * Guid created on stack
   */
  jnx_guid guid_ref;
}session_ref;

session_ref* session_create();

#endif
