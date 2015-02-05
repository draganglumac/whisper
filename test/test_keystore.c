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
#include "../src/session/keystore.h"
#include <openssl/rsa.h>
#include "../src/crypto/cryptography.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlog.h>

void test_create_destroy() {
  JNX_LOG(NULL,"test_create_destroy");
  session_key_store *s = session_key_store_create();
  JNXCHECK(s);
  session_key_store_state e = session_key_store_destroy(s);
}
void test_keystore_add() {
  JNX_LOG(NULL,"test_keystore_add");
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
void test_keystore_collisions() {
  JNX_LOG(NULL,"test_keystore_collisions");
  session_key_store *s = session_key_store_create();
  JNXCHECK(s);
  int i;
  for(i=0;i<5;++i){
    RSA *keypair = asymmetrical_generate_key(2048);
    jnx_guid g;
    jnx_guid_create(&g);
    jnx_char *ostr;
    jnx_guid_to_string(&g,&ostr);
    free(ostr);
    session_key_store_state e = session_key_store_add(s,&g,keypair);
    JNXCHECK(e != SESSION_KEY_STORE_EXISTS);
  }
  JNXCHECK(s->key_data_list->counter == 5);
}
int main(int argc, char **argv) {

  //  test_create_destroy();
  //  test_keystore_add();
  test_keystore_collisions();
  return 0;
}
