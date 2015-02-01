/*
 * =====================================================================================
 *
 *       Filename:  keystore.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/31/2015 02:43:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "keystore.h"

session_key_store *session_key_store_create() {
  session_key_store *s = malloc(sizeof(session_key_store));
  s->key_data_list = jnx_list_create();
}
session_key_store_state session_key_store_destroy(session_key_store *s) {
  /*
   * TODO:
   * Are there keys you also wish to destroy? 
   */
  jnx_list_destroy(&s->key_data_list);
  free(s);

  return SESSION_KEY_STORE_OKAY;
}
