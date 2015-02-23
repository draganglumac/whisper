/*
 * =====================================================================================
 *
 *       Filename:  communications.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/24/2015 08:36:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  AlexsJones(jonesax@hush.com),
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __COMMUNICATIONS_H__
#define __COMMUNICATIONS_H__
#include <jnxc_headers/jnxcheck.h>
#include "../session/session.h"

typedef struct communication_service {

}communication_service;

communication_service *communication_service_create();

void communication_service_push(communication_service *cs,session *s);

void communication_service_destroy(communication_service **cs);

#endif
