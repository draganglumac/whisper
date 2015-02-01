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

void session_create(SessionObject *s);

jnx_uint32* session_guid_create(SessionObject *s);

jnx_size session_pack(SessionObject *s,jnx_uint8 **obuffer);

SessionObject *session_unpack(jnx_uint8 *buffer,jnx_size len);

void session_generate_keys(SessionObject *s);
#endif
