/*
 * =====================================================================================
 *
 *       Filename:  test_session.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/09/2015 05:57:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "../src/session/session_service.h"
void test_create_destroy() {
  JNX_LOG(NULL,"test_create_destroy");
  session_service *service = session_service_create();
  session *os;
  //Create a session
  session_state e = session_service_create_session(service,&os);
  JNXCHECK(e == SESSION_STATE_OKAY);
  JNXCHECK(service->session_list->counter == 1);
  //Pulling out that session guid lets get a handle on our session
  session *retrieved = NULL;
  //Lets test we can retrieve our session by giving our service the guid
  e = session_service_fetch_session(service,&os->session_guid,&retrieved);
  JNXCHECK(e == SESSION_STATE_OKAY);
  JNXCHECK(jnx_guid_compare(&os->session_guid,&retrieved->session_guid) == JNX_GUID_STATE_SUCCESS);
  e = session_service_destroy_session(service,&os->session_guid);
  JNXCHECK(e == SESSION_STATE_OKAY);
  JNXCHECK(service->session_list->counter == 0);
  session_service_destroy(&service);
  JNXCHECK(service == NULL);
}
void test_heavy_load() {
  JNX_LOG(NULL,"test_heavy_load");
  session_service *service = session_service_create();
  jnx_int c = 20,d;
  for(d=0;d<c;++d) {
    session *os;
    session_state e = session_service_create_session(service,&os);
    JNXCHECK(service->session_list->counter == d + 1);
  }
  session_service_destroy(&service);
  JNXCHECK(service == NULL);
}
void test_linking() {
  JNX_LOG(NULL,"test_linking");
  session_service *service = session_service_create();
  session *os;
  session_state e = session_service_create_session(service,&os);
  JNXCHECK(session_service_session_is_linked(service,&os->session_guid) == 0); 
  //Lets generate the guid of some remote session
  jnx_guid h;
  jnx_guid_create(&h);
  e = session_service_link_sessions(service,&os->session_guid,&h,&h);
  JNXCHECK(e == SESSION_STATE_OKAY);
  print_pair(&os->local_peer_guid,&os->remote_peer_guid);
  JNXCHECK(session_service_session_is_linked(service,&os->session_guid) == 1); 
  e = session_service_unlink_sessions(service,&os->session_guid);
  JNXCHECK(e == SESSION_STATE_OKAY);
  JNXCHECK(session_service_session_is_linked(service,&os->session_guid) == 0); 
  session_service_destroy(&service);
  JNXCHECK(service == NULL);
}
int main(int argc, char **argv) {
  test_create_destroy();
  test_heavy_load();
  test_linking();
  return 0;
}
