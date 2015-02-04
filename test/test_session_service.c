/*
 * =====================================================================================
 *
 *       Filename:  test_session_service.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/01/2015 02:55:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxtypes.h>
#include "session/session_service.h"


int main(int argc, char **argv) {
  JNX_LOG(NULL,"Testing session service");


  session_service *service = session_service_create();
  JNXCHECK(service);
  JNXCHECK(service->keystore);

  return 0;
}
