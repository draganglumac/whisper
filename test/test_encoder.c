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
#include "integrity/encoding.h"
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxtypes.h>

void test_encoder() {
  jnx_char *test_message = "This is a test message";
  jnx_size size = strlen(test_message);
  jnx_size *olen;
  jnx_uint8 *network_encoded_string = encode_from_string(test_message,size,&olen);

  JNX_LOG(NULL,"Encoded string %s\n",network_encoded_string);
  
  jnx_size *secondolen;
  jnx_char *decoded_string = decode_to_string(network_encoded_string,olen,&secondolen);
  
  JNX_LOG(NULL,"Decoded string %s\n",decoded_string);

  JNXCHECK(strcmp(decoded_string,test_message) == 0);
}
void test_uuid() {
  
  uuid_t guid;
  generate_uuid(&guid);
  jnx_char str[37];
  uuid_unparse_lower(guid,str);
  
  JNXCHECK(str);
  
  JNX_LOG(NULL,"Generated guid => %s\n",str);
}
int main(int argc, char **argv) {

  test_encoder();
  test_uuid();
  return 0;
}
