/*
 * =====================================================================================
 *
 *       Filename:  session_service.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/01/2015 12:46:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "session_service.h"
#include "keystore.h"
#include "session.h"

session_service *session_service_create() {
  session_service *s = malloc(sizeof(session_service));
  s->keystore = session_key_store_create(); 
  return s;
}
