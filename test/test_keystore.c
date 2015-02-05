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
#include "../src/crypto/keystore.h"
#include <openssl/rsa.h>
#include "../src/crypto/cryptography.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlog.h>

void test_create_destroy() {
  JNX_LOG(NULL,"test_create_destroy");
  session_key_store *s = session_key_store_create();
  JNXCHECK(s);
  JNXCHECK(s->key_data_list->counter == 0);
  session_key_store_state e = session_key_store_destroy(s);
}
void generate_new_key_entry(session_key_store *s) {
  JNX_LOG(NULL,"- generating asymmetrical key");
  RSA *keypair = asymmetrical_generate_key(2048);
  jnx_guid g;
  JNX_LOG(NULL,"- generating guid");
  jnx_guid_create(&g);
  jnx_char *gstr;
  jnx_guid_to_string(&g,&gstr);
  JNX_LOG(NULL,"- created guid %s",gstr);
  free(gstr);
  JNX_LOG(NULL,"- inserting pair into keystore");
  JNXCHECK(session_key_store_add(s,&g,keypair) == SESSION_KEY_STORE_OKAY);
}
void test_keystore_add() {
  JNX_LOG(NULL,"test_keystore_add");
  session_key_store *s = session_key_store_create(); 
  JNXCHECK(s);
  JNXCHECK(s->key_data_list->counter == 0);
  int i;
  for(i=0;i<10;++i) {
    JNX_LOG(NULL,"Loop %d KeyStore size %d",i,s->key_data_list->counter);
    generate_new_key_entry(s);
    JNXCHECK(s->key_data_list->counter == i + 1);
  }
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
    session_key_store_state e = session_key_store_add(s,&g,keypair);
    JNXCHECK(e != SESSION_KEY_STORE_EXISTS);
    /*
     * Retrieve our stored keypair
     */
    RSA *okeys;
    session_key_store_retrieve_key(s,&g,&okeys);
    JNXCHECK(okeys);
  }
  JNXCHECK(s->key_data_list->counter == 5);
}
int main(int argc, char **argv) {

  test_create_destroy();
  test_keystore_add();
  test_keystore_collisions();
  return 0;
}
