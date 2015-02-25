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
#include "../util/utils.h"

static void destroy_session(session *s);

static void generate_blank_guid(jnx_guid *g) {
  jnx_char *zero_out = "00000000000000000000000000000000";
  jnx_guid_from_string(zero_out,g);
}
static int is_guid_blank(jnx_guid *g) {
  jnx_int is_blank = 0;
  jnx_guid local;
  jnx_char *zero_out = "00000000000000000000000000000000";
  jnx_guid_from_string(zero_out,&local);
  if(jnx_guid_compare(&local,g) == JNX_GUID_STATE_SUCCESS) {
    is_blank = 1;
  }
  return is_blank;
}
static int session_service_does_exist(session_service *service, jnx_guid *g) {
  jnx_int does_exist = 0;
  jnx_node *h = service->session_list->head,
           *r = service->session_list->head;
  while(h) {
    session *retrieved_session = h->_data;
    jnx_guid *retrieved_guid = &retrieved_session->session_guid;
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
  s->communication = communication_service_create();
  return s;
}
void session_service_destroy(session_service **service) {
  if((*service)->session_list->counter){
    jnx_list *sessions_list;
    JNXCHECK(session_service_fetch_all_sessions(*service,&sessions_list) == SESSION_STATE_OKAY);
    jnx_node *h = sessions_list->head,
             *r = sessions_list->head;
    while(h) {
      session *s = h->_data;
      session_service_destroy_session(*service,&s->session_guid);
      h = h->next_node;
    }
    h = r;
    jnx_list_destroy(&sessions_list);
  }
  jnx_list *s = (*service)->session_list;
  jnx_list_destroy(&s);
  free(*service);
  *service = NULL;
}
session_state session_service_create_session(session_service *service, session **osession) {
  session *s = malloc(sizeof(session));
  s->keypair = asymmetrical_generate_key(2048);  
  jnx_guid_create(&s->session_guid);
  generate_blank_guid(&s->local_peer_guid); 
  generate_blank_guid(&s->local_peer_guid); 
  if(session_service_does_exist(service,&s->session_guid)){
    return SESSION_STATE_EXISTS;
  }
  session_service_add_session(service,s);
  *osession = s;
  return SESSION_STATE_OKAY;
}
session_state session_service_fetch_all_sessions(session_service *service, jnx_list **olist) {
  *olist = NULL;
  if(service->session_list->counter == 0) {
    JNX_LOG(NULL,"Session list is empty");
    return SESSION_STATE_NOT_FOUND;
  }
  jnx_node *h = service->session_list->head,
           *r = service->session_list->head;
  *olist = jnx_list_create();
  while(h) {
    session *s = h->_data;
    jnx_list_add_ts(*olist,s); 
    h = h->next_node;
  }
  service->session_list->head = r;
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
    if(jnx_guid_compare(g,&s->session_guid) == JNX_GUID_STATE_SUCCESS) {
      *osession = s;
      return SESSION_STATE_OKAY;
    }
    h = h->next_node;
  }
  service->session_list->head = r;
  return SESSION_STATE_NOT_FOUND;
}
session_handshake_state session_service_fetch_session_state(session_service *service, jnx_guid *session_guid) {

  return SESSION_HANDSHAKE_FINISH;
}
static void destroy_session(session *s) {
  JNXCHECK(s);
  asymmetrical_destroy_key(s->keypair);
  free(s);
}
session_state session_service_destroy_session(session_service *service, jnx_guid *g) {
  JNXCHECK(service);
  session_state e = SESSION_STATE_NOT_FOUND;
  jnx_node *h = service->session_list->head,
           *r = service->session_list->head;
  jnx_list *cl = jnx_list_create();
  session *retrieved_session = NULL;
  while(h) {
    session *s = h->_data;
    if(jnx_guid_compare(g,&s->session_guid) == JNX_GUID_STATE_SUCCESS) {
      retrieved_session = s;
      e = SESSION_STATE_OKAY;
    }else {
      jnx_list_add_ts(cl,s);
    }
    h = h->next_node;
  }
  service->session_list->head = r;
  jnx_list_destroy(&service->session_list); 
  if(cl) {
    destroy_session(retrieved_session);
  }
  service->session_list = cl;
  return e;
}
session_state session_service_link_sessions(session_service *s,\
    jnx_guid *session_guid,peer *local_peer, peer *remote_peer) {
  session *osession;
  session_state e = session_service_fetch_session(s,session_guid,&osession);
  if(e != SESSION_STATE_OKAY) {
    return e;
  }
  osession->local_peer_guid = local_peer->guid;
  osession->remote_peer_guid = remote_peer->guid;
  return e;
}
session_state session_service_unlink_sessions(session_service *s,\
    jnx_guid *session_guid) {
  session *osession;
  session_state e = session_service_fetch_session(s,session_guid,&osession);
  if(e != SESSION_STATE_OKAY) {
    return e;
  }
  jnx_guid g;
  generate_blank_guid(&g);
  osession->local_peer_guid = g;
  osession->remote_peer_guid = g;
  JNXCHECK(is_guid_blank(&osession->local_peer_guid));
  JNXCHECK(is_guid_blank(&osession->remote_peer_guid));
  return SESSION_STATE_OKAY;
}
jnx_int session_service_session_is_linked(session_service *s,\
    jnx_guid *session_guid) {
  session *osession;
  session_state e = session_service_fetch_session(s,session_guid,&osession);
  if(e != SESSION_STATE_OKAY) {
    JNX_LOG(NULL,"Could not retrieve session");
    return 0;
  }
  return  is_guid_blank(&osession->remote_peer_guid) ? 0 : 1;
}
