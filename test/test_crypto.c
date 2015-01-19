/*
 * =====================================================================================
 *
 *       Filename:  test_crypto.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  19/01/15 11:20:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "crypto/cryptography.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>

void test_rsa_key() {
  RSA *key = asymmetrical_generate_key(2048);
  JNXCHECK(key);
  
  jnx_char *keystring = asymmetrical_key_to_string(key,PUBLIC);

  JNX_LOG(NULL,"Key \n%s",keystring);
  
  jnx_char *keystringprivate = asymmetrical_key_to_string(key,PRIVATE);

  JNX_LOG(NULL,"Key \n%s",keystringprivate);
  
  free(keystring);
  free(keystringprivate);
  asymmetrical_destroy_key(key);

}
int main(int argc, char **argv) {

  test_rsa_key();

  return 0;
}
