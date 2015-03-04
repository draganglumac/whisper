/*
 * =====================================================================================
 *
 *       Filename:  app.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/02/15 15:46:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (draganglumac), dragan.glumac@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <jnxc_headers/jnxterm.h>
#include "app.h"
#include "../gui/gui.h"
#include "../net/auth_comms.h"
void app_create_gui_session(session *s) {
  context_t *c = context_create();
  pthread_t read_thread;
  pthread_create(&read_thread, 0,read_loop,(void*)c);
  while (TRUE) {
    if (-1 == output_next_message_in_context(c)) {
      break;
    }
  }
  context_destroy(c);
}
int is_equivalent(char *command, char *expected) {
  if (strcmp(command, expected) == 0) {
    return 1;
  }
  else if (strncmp(command, expected, 1) == 0) {
    return 1;
  }
  return 0;
}
int code_for_command(char *command) {
  if (is_equivalent(command, "list")) {
    return CMD_LIST;
  }
  else if (is_equivalent(command, "session")) {
    return CMD_SESSION;
  }
  else if (is_equivalent(command, "quit")) {
    return CMD_QUIT;
  }
  return CMD_HELP;
}
int app_code_for_command_with_param(char *command, jnx_size cmd_len, char **oparam) {
  *oparam = NULL;
  char *raw_cmd = strtok(command," \n\r\t");
  if(!raw_cmd) {
    return CMD_HELP;
  }
  char *extra_params = strtok(NULL," \n\r\t");
  if(is_equivalent(raw_cmd,"session")) {
    if(!extra_params) {
      printf("Requires name of peer as argument.\n");
      return CMD_HELP;
    }
    *oparam = malloc(strlen(extra_params) + 1);
    strcpy(*oparam,extra_params);
    return CMD_SESSION;
  }else {
    return code_for_command(raw_cmd);
  }
}
void app_prompt() {
  printf("Enter a command into the prompt.\n");
  printf("(For the list of commands type 'h' or 'help'.)\n");
  printf("\n");
  printf("$> ");
  fflush(stdout);
}
static void pretty_print_peer(peer *p) {
  char *guid;
  jnx_guid_to_string(&p->guid, &guid);
  printf("%-32s %-16s %s\n", guid, p->host_address, p->user_name);
  free(guid);
}
static void pretty_print_peer_in_col(peer *p, jnx_int32 colour) {
  char *guid;
  jnx_guid_to_string(&p->guid, &guid);
  jnx_term_printf_in_color(colour,
      "%-32s %-16s %s\n", guid, p->host_address, p->user_name);
  free(guid);
}
static void show_active_peers(peerstore *ps) {
  jnx_guid **active_guids;
  int num_guids = peerstore_get_active_guids(ps, &active_guids);
  int i;
  peer *local = peerstore_get_local_peer(ps);
  for (i = 0; i < num_guids; i++) {
    jnx_guid *next_guid = active_guids[i];
    peer *p = peerstore_lookup(ps, next_guid);
    if (peers_compare(p, local) != 0) {
      pretty_print_peer(p);
    }
  }
}
void app_list_active_peers(app_context_t *context) {
  printf("\n");
  printf("Active Peers:\n");
  printf("%-32s %-16s %-16s\n", "UUID", "IP Address", "Username");
  printf("--------------------------------+\
      ----------------+----------------\n");
  show_active_peers(context->discovery->peers);
  peer *local = peerstore_lookup_by_username(context->discovery->peers, 
      peerstore_get_local_peer(context->discovery->peers)->user_name);
  printf("\n");
  printf("Local peer:\n");
  pretty_print_peer_in_col(local, JNX_COL_GREEN);
  printf("\n");
}
void app_intro() {
  printf("\n");
  printf("Welcome to Whisper Chat\n");
  printf("\n");
}
void app_show_help() {
  printf("\n");
  printf("Valid commands are:\n");
  printf("\n");
  printf("\th or help    - show help\n");
  printf("\tl or list    - show the list of active peers\n");
  printf("\ts or session - create session (PLACEHOLDER to launch GUI for now)\n");
  printf("\tq or quit    - quit the program\n");
  printf("\n");
}
void app_quit_message() {
  printf("\n");
  printf("Shutting down cleanly...\n");
}

extern int peer_update_interval;

static void set_up_discovery_service(app_context_t *context) {
  jnx_hashmap *config = context->config;
  char *user_name = (char *) jnx_hash_get(config, "USER_NAME");
  if (user_name == NULL) {
    JNX_LOG(0, "[ERROR] You must supply the user name in the configuration. Add USER_NAME=username line to the config file.");
    exit(1);
  }
  peerstore *ps = peerstore_init(local_peer_for_user(user_name), 0);
  char *local_ip = (char *) jnx_hash_get(config, "LOCAL_IP");
  if (local_ip != NULL) {
    free(peerstore_get_local_peer(ps)->host_address);
    peerstore_get_local_peer(ps)->host_address = local_ip;
  }

  int port = DEFAULT_BROADCAST_PORT;
  char *disc_port = (char *) jnx_hash_get(config, "DISCOVERY_PORT");
  if (disc_port != NULL) {
    port = atoi(disc_port);
  }

  char *broadcast_address = (char *) jnx_hash_get(config, "DISCOVERY_MULTICAST_IP");
  if (broadcast_address == NULL) {
    get_broadcast_ip(&broadcast_address);
  }

  char *discovery_interval = (char *) jnx_hash_get(config, "DISCOVERY_INTERVAL");
  if (discovery_interval != NULL) {
    peer_update_interval = atoi(discovery_interval);
  }

  context->discovery = discovery_service_create(port, AF_INET, broadcast_address, ps);
  char *discovery_strategy = (char *) jnx_hash_get(config, "DISCOVERY_STRATEGY");
  if (discovery_strategy == NULL) {
    JNX_LOG(0, "Starting discovery service with heartbeat srategy.");
    discovery_service_start(context->discovery, BROADCAST_UPDATE_STRATEGY);
  }	else {
    if (0 == strcmp(discovery_strategy, "polling")) {
      JNX_LOG(0, "Starting discovery service with polling srategy.");
      discovery_service_start(context->discovery, POLLING_UPDATE_STRATEGY);
    }
  }
}
int app_is_input_guid_size(char *input) {
  if((strlen(input) / 2) == 16) {
    return 1;
  }
  return 0;
}
peer *app_peer_from_input(app_context_t *context, char *param) {
  if(app_is_input_guid_size(param)) {
    jnx_guid g;
    jnx_guid_from_string(param,&g);
    peer *p = peerstore_lookup(context->discovery->peers,&g);
    return p;
  }else {
    peer *p = peerstore_lookup_by_username(context->discovery->peers,param);
    return p;
  }
  return NULL;
}
void app_initiate_handshake(app_context_t *context,session *s) {
  auth_comms_initiator_start(context->auth_comms,context->discovery,s);
}
void set_up_session_service(app_context_t *context){
  context->session_serv = session_service_create();
}
void set_up_auth_comms(app_context_t *context) {
  context->auth_comms = auth_comms_create();
  context->auth_comms->listener_port = "9991";
  context->auth_comms->listener_family = AF_INET;
  context->auth_comms->listener_socket = jnx_socket_tcp_create(AF_INET);
  auth_comms_listener_start(context->auth_comms,context->discovery,context->session_serv);
}
app_context_t *app_create_context(jnx_hashmap *config) {
  app_context_t *context = calloc(1, sizeof(app_context_t));
  context->config = config;
  set_up_discovery_service(context);
  set_up_session_service(context);
  set_up_auth_comms(context);
  char *broadcast_ip, *local_ip;
  jnx_term_printf_in_color(JNX_COL_GREEN, "Broadcast IP: %s\n",
      context->discovery->broadcast_group_address);
  jnx_term_printf_in_color(JNX_COL_GREEN, "Local IP:     %s\n", 
      peerstore_get_local_peer(context->discovery->peers)->host_address);
  return context;
}
void app_destroy_context(app_context_t **context) {
  discovery_service_cleanup(&(*context)->discovery);
  session_service_destroy(&(*context)->session_serv);
  auth_comms_destroy(&(*context)->auth_comms);
  free(*context);
  *context = NULL;
}
