/*
 * =====================================================================================
 *
 *       Filename:  test_keystore.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/31/2015 04:35:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "session/keystore.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>

void test_create_destroy() {
  session_key_store *s = session_key_store_create();
  JNXCHECK(s);
  
  session_key_store_state e = session_key_store_destroy(s);
}
int main(int argc, char **argv) {

  test_create_destroy();

  return 0;
}
