/*
 * =====================================================================================
 *
 *       Filename:  session.h
 *
 *    Description:  
 *
 *        Version:  1.0
<<<<<<< HEAD
<<<<<<< HEAD
 *        Created:  25/01/2015 20:50:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com)
=======
 *        Created:  26/01/15 12:02:43
=======
 *        Created:  02/09/2015 09:10:54 PM
>>>>>>> upstream/master
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

#include "../crypto/cryptography.h"

typedef struct session {
  jnx_guid local_guid;
  jnx_guid remote_guid;
  RSA *keypair;
}session;

#endif
