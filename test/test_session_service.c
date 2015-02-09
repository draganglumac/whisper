/*
 * =====================================================================================
 *
 *       Filename:  test_session_service.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/01/2015 02:55:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <jnxc_headers/jnxcheck.h>
#include <jnxc_headers/jnxguid.h>
#include <jnxc_headers/jnxlog.h>
#include <jnxc_headers/jnxtypes.h>
#include "../src/crypto/cryptography.h"
#include "../src/session/session_service.h"
void generate_session(session_service *service,jnx_guid *g) {
  session_service_create_session(service,g);
  RSA *okeys;
  session_service_fetch_session_keys(service,g,&okeys);
  jnx_char *pubkeystring = asymmetrical_key_to_string(okeys,PUBLIC);
  jnx_char *prikeystring = asymmetrical_key_to_string(okeys,PRIVATE);
  JNX_LOG(NULL,"Key %s",pubkeystring);
  JNX_LOG(NULL,"Key %s",prikeystring);
  JNXCHECK(pubkeystring);
  JNXCHECK(prikeystring);
  free(pubkeystring);
  free(prikeystring);
}
void test_session_create() {
  JNX_LOG(NULL,"test_session_create");
  session_service *service = session_service_create();
  JNXCHECK(service);
  JNXCHECK(service->keystore);
  int i;
  for(i=0;i<10;++i) {
    jnx_guid g;
    generate_session(service,&g);
    JNXCHECK(service->sessionstore->key_data_list->counter == i + 1);
  }
  session_service_destroy(service);
}
void test_session_destroy() {
  JNX_LOG(NULL,"test_session_destroy");
  SessionObject *so;
  session_service *ss = session_service_create();
  jnx_guid guid;
  //This created session will exist in the store
  session_service_create_session(ss,&guid);
  session_service_state e = session_service_fetch_session(ss,&guid, &so);
  JNXCHECK(e == SESSION_STORE_EXISTS);
  JNXCHECK(ss->keystore->key_data_list->counter == 1);
  JNXCHECK(ss->sessionstore->key_data_list->counter == 1);
  session_service_destroy_session(ss,&guid);
  JNXCHECK(ss->keystore->key_data_list->counter == 0);
  JNXCHECK(ss->sessionstore->key_data_list->counter == 0);
  e = session_service_fetch_session(ss,&guid, &so);
  JNXCHECK(e == SESSION_STORE_NOT_FOUND);
}
void test_session_linking() {
  JNX_LOG(NULL,"test_session_linking");
  session_service *service = session_service_create();
  //Generate session A
  jnx_guid a;
  generate_session(service,&a);
  //Generate session B
  jnx_guid b;
  generate_session(service,&b);
  //Link A to B
  session_service_link_session(service,&a,&b);
  JNXCHECK(service->sessionstore->key_data_list->counter == 2);
  JNXCHECK(service->keystore->key_data_list->counter == 2);
 
  SessionObject *s;
  session_service_fetch_session(service,&a,&s);

  JNX_LOG(NULL,"Fetched session object with guid %s of length %d",s->guid,strlen(s->guid));

  session_service_destroy(service);
}
int main(int argc, char **argv) {
  test_session_create();
 // test_session_destroy();
//  test_session_linking();
  return 0;
}

