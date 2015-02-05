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
void generate_new_key_entry(session_key_store *s, jnx_guid *ref) {
  JNX_LOG(NULL,"- generating asymmetrical key");
  RSA *keypair = asymmetrical_generate_key(2048);
  jnx_guid g;
  JNX_LOG(NULL,"- generating guid");
  jnx_guid_create(&g);
  ref = &g;
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
    jnx_guid *ref;
    generate_new_key_entry(s,ref);
    JNXCHECK(s->key_data_list->counter == i + 1);
  }
  session_key_store_destroy(s);
}
void test_keystore_retrieval() {
  JNX_LOG(NULL,"test_keystore_retrieval");
  session_key_store *s = session_key_store_create();
  JNXCHECK(s);
  int i;
  for(i=0;i<15;++i){
    jnx_guid ref;
    generate_new_key_entry(s,&ref); 
    
    RSA *okeys;
    session_key_store_retrieve_key(s,&ref,&okeys);
    JNXCHECK(okeys);
  }
  JNXCHECK(s->key_data_list->counter == 15);
}
int main(int argc, char **argv) {

  test_create_destroy();
  test_keystore_add();
  test_keystore_retrieval();
  return 0;
}
