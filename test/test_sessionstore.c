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
#include "../src/session/session_store.h"
#include <openssl/rsa.h>
#include "../src/crypto/cryptography.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlog.h>

void test_create_destroy() {
  JNX_LOG(NULL,"Test_create_destroy");
  session_store *s = session_store_create();
  JNXCHECK(s);
  JNXCHECK(s->key_data_list->counter == 0);
  session_store_state e = session_store_destroy(s);
}
int main(int argc, char **argv) {
  test_create_destroy();
  return 0;
}
