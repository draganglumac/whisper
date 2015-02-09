/*
 * =====================================================================================
 *
 *       Filename:  session.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/09/2015 09:10:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
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
