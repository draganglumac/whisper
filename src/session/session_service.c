/*
 * =====================================================================================
 *
 *       Filename:  session_service.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/09/2015 05:54:58 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "session_service.h"

static int session_service_does_exist(session_service *service, jnx_guid *g) {
  jnx_int does_exist = 0;
  jnx_node *h = service->session_list->head,
           *r = service->session_list->head;
  while(h) {
    session *retrieved_session = h->_data;
    jnx_guid *retrieved_guid = &retrieved_session->local_guid;
    if(jnx_guid_compare_raw(g->guid,retrieved_guid->guid) == JNX_GUID_STATE_SUCCESS) {
      does_exist = 1;
    }
    h = h->next_node;
  }
  service->session_list->head = r;
  return does_exist;
}
static void session_service_add_session(session_service *service, session *s) {
  jnx_int kc = service->session_list->counter;
  jnx_list_add_ts(service->session_list,s);
  JNXCHECK(service->session_list->counter == kc + 1);
}
session_service *session_service_create() {
  session_service *s = malloc(sizeof(session_service));
  s->session_list = jnx_list_create();
  return s;
}
void session_service_destroy(session_service **service) {
  jnx_list *s = (*service)->session_list;
  jnx_list_destroy(&s);
  free(*service);
  *service = NULL;
}
session_state session_service_create_session(session_service *service, session **osession) {
  session *s = malloc(sizeof(session));
  jnx_guid_create(&s->local_guid);
  if(session_service_does_exist(service,&s->local_guid)){
    return SESSION_STATE_EXISTS;
  }
  session_service_add_session(service,s);
  *osession = s;
  return SESSION_STATE_OKAY;
}
session_state session_service_fetch_session(session_service *service, jnx_guid *g, session **osession) {
  if(service->session_list->counter == 0) {
    JNX_LOG(NULL,"Session list is empty");
    return SESSION_STATE_NOT_FOUND;
  }
  if(session_service_does_exist(service,g) == 0) {
    JNX_LOG(NULL,"Session not found");
    return SESSION_STATE_NOT_FOUND;
  }
  jnx_node *h = service->session_list->head,
           *r = service->session_list->head;
  while(h) {
    session *s = h->_data;
    if(jnx_guid_compare(g,&s->local_guid) == JNX_GUID_STATE_SUCCESS) {
      *osession = s;
      return SESSION_STATE_OKAY;
    }
    h = h->next_node;
  }
  service->session_list->head = r;
  return SESSION_STATE_NOT_FOUND;
}
session_state session_service_destroy_session(session_service *service, jnx_guid *g) {
  session_state e = SESSION_STATE_NOT_FOUND;
  jnx_node *h = service->session_list->head,
           *r = service->session_list->head;
  jnx_list *replacement = jnx_list_create();
  session *retrieved_session = NULL;
  while(h) {
    retrieved_session = h->_data;
    jnx_guid *retrieved_guid = &retrieved_session->local_guid;
    if(jnx_guid_compare_raw(g->guid,retrieved_guid->guid) == JNX_GUID_STATE_SUCCESS) {
      e = SESSION_STATE_OKAY;
    }else {
      jnx_list_add_ts(replacement,retrieved_session);
    }
    h = h->next_node;
  }
  if(e == SESSION_STATE_OKAY && (retrieved_session != NULL)) {
    /*
     * Any other session related cleanup here!
     */
    free(retrieved_session);
  }
  jnx_list *old_list = service->session_list;
  service->session_list = replacement;
  jnx_list_destroy(&old_list); 
  return e;
}
