/*
 * =====================================================================================
 *
 *       Filename:  test_encoder.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/20/2015 08:31:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "session/session.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxtypes.h>

void session_create_destroy() {

  session_ref *s = session_create();

}
int main(int argc, char **argv) {

  session_create_destroy();
  return 0;
}
