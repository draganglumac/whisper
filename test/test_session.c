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
#include "../src/util/utils.h"
void test_create_destroy() {

  session_service *service = session_service_create();

  session *os;
  //Create a session
  session_state e = session_service_create_session(service,&os);
  JNXCHECK(e == SESSION_STATE_OKAY);
  JNXCHECK(service->session_list->counter == 1);
  //Pulling out that session guid lets get a handle on our session
  session *retrieved = NULL;
  //Lets test we can retrieve our session by giving our service the guid
  e = session_service_fetch_session(service,&os->local_guid,&retrieved);
  JNXCHECK(e == SESSION_STATE_OKAY);

  JNXCHECK(jnx_guid_compare(&os->local_guid,&retrieved->local_guid) == JNX_GUID_STATE_SUCCESS);
  e = session_service_destroy_session(service,&os->local_guid);
  JNXCHECK(e == SESSION_STATE_OKAY);
  JNXCHECK(service->session_list->counter == 0);
  session_service_destroy(&service);
  JNXCHECK(service == NULL);
}
int main(int argc, char **argv) {

  test_create_destroy();

  return 0;
}
