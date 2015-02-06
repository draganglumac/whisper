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
#include "../src/session/session.h"
#include <openssl/rsa.h>
#include "../src/crypto/cryptography.h"
#include "../src/crypto/keystore.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlog.h>

void test_create_destroy() {
  JNX_LOG(NULL,"test_create_destroy");
  session_store *s = session_store_create();
  JNXCHECK(s);
  JNXCHECK(s->key_data_list->counter == 0);
  session_store_state e = session_store_destroy(s);
}
void generate_sessions(session_store *sstore, session_key_store *sk) {
  SessionObject so;
  //Create a session 
  session_create(&so,sk);
  //Add session to the store 
  session_store_add(sstore,&so);
  //Check that our session has been correctly added
  jnx_guid g;
  session_fetch_guid(&so,&g);
  JNXCHECK(session_store_does_exist(sstore,&g));
}
void test_add_store() {
  JNX_LOG(NULL,"test_add_store");
  //Create a key store
  session_key_store *sk = session_key_store_create();
  JNXCHECK(sk);
  JNXCHECK(sk->key_data_list->counter == 0);
  //Create the session store
  session_store *s = session_store_create();
  JNXCHECK(s);
  JNXCHECK(s->key_data_list->counter == 0);

  int i;
  for(i=0;i < 5; ++i) {
    generate_sessions(s,sk);
    JNXCHECK(s->key_data_list->counter == i + 1);
  }
  session_store_state e = session_store_destroy(s);
}
int main(int argc, char **argv) {
  test_create_destroy();
  test_add_store();
  return 0;
}
