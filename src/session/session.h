/*
 * =====================================================================================
 *
 *       Filename:  session.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/01/15 12:02:43
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
#include "session_object.pb-c.h"
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxtypes.h>
#include <jnxc_headers/jnxhash.h>
#include <openssl/rsa.h>

/*
 * @fn session_create(SessionObject *s)
 * @brief Pass a ref in for population with session object, will generate keys
 * and unique identiifier
 * @param s is a ref to be passed in
 */
void session_create(SessionObject *s);

void session_fetch_guid(SessionObject *s,jnx_guid *g);

#endif
