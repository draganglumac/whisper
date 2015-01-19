/*
 * =====================================================================================
 *
 *       Filename:  cryptography.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  19/01/15 10:59:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "cryptography.h"
#include <time.h>

RSA *asymmetrical_generate_key(jnx_size length) {
  srand(time(NULL));
  return RSA_generate_key(length,3,NULL,NULL);
}
void asymmetrical_destroy_key(RSA *key) {
  if(key != NULL) RSA_free(key);
}
jnx_char *asymmetrical_key_to_string(RSA *keypair,key_type type) {
   BIO *key = BIO_new(BIO_s_mem());
    switch(type) {
    case PUBLIC:
        PEM_write_bio_RSAPublicKey(key,keypair);
        break;

    case PRIVATE:
        PEM_write_bio_RSAPrivateKey(key,keypair,NULL,NULL,0,NULL,NULL);
        break;
    }
    jnx_size len = BIO_pending(key);
    jnx_char *skey = malloc(len + 1);
    jnx_size read = BIO_read(key,skey,len);
    skey[read + 1] = '\0';
    BIO_free(key);
    return skey;
}
jnx_char *asymmetrical_encrypt(RSA *keypair, jnx_uint8 *message, jnx_size *out_len) {

}
jnx_char *asymmetrical_decrypt(RSA *keypair, jnx_uint8 *message, jnx_size in_len, jnx_size *out_len) {

}
jnx_char *symmetrical_encrypt(jnx_uint8 *key,jnx_uint8 *msg, jnx_size size) {

}
jnx_char *symmetrical_decrypt(jnx_uint8 *key,jnx_uint8 *msg, jnx_size size) {


}
