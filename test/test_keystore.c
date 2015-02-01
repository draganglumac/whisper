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
#include <openssl/rsa.h>
#include "crypto/cryptography.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlog.h>

void test_create_destroy() {
  session_key_store *s = session_key_store_create();
  JNXCHECK(s);
  session_key_store_state e = session_key_store_destroy(s);
}
void test_keystore_add() {
  session_key_store *s = session_key_store_create();
  JNXCHECK(s);
  
  RSA *keypair = asymmetrical_generate_key(2048);
  jnx_guid g;

  jnx_guid_create(&g);
  session_key_store_add(s,&g,keypair);

  session_key_store_state e = session_key_store_does_exist(s,&g);

  JNXCHECK(e == SESSION_KEY_STORE_EXISTS);

  session_key_store_state eagain = session_key_store_add(s,&g,keypair);

  JNXCHECK(eagain == SESSION_KEY_STORE_EXISTS);

  session_key_store_destroy(s);
}
int main(int argc, char **argv) {

  test_create_destroy();
  test_keystore_add();
  return 0;
}
