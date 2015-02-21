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

#include "app.h"
#include "../gui/gui.h"

void create_gui_session() {
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
void prompt() {
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
static void show_active_peers(peerstore *ps) {
  jnx_guid **active_guids;
  int num_guids = peerstore_get_active_guids(ps, &active_guids);
	int i;
	for (i = 0; i < num_guids; i++) {
		jnx_guid *next_guid = active_guids[i];
		peer *p = peerstore_lookup(ps, next_guid);
		pretty_print_peer(p);
	}
}
void list_active_peers(app_context_t *context) {
  printf("\n");
  printf("Active Peers:\n");
  printf("%-32s %-16s %-16s\n", "UUID", "IP Address", "Username");
  printf("--------------------------------+----------------+----------------\n");
	show_active_peers(context->discovery->peers);
	printf("\n");
}
void intro() {
  printf("\n");
  printf("Welcome to Whisper Chat\n");
  printf("\n");
}
void show_help() {
  printf("\n");
  printf("Valid commands are:\n");
  printf("\n");
  printf("\th or help    - show help\n");
  printf("\tl or list    - show the list of active peers\n");
  printf("\ts or session - create session (PLACEHOLDER to launch GUI for now)\n");
  printf("\tq or quit    - quit the program\n");
  printf("\n");
}
void quit_message() {
  printf("\n");
  printf("Leaving Whisper chat.\n");
  printf("\n");
}

extern int peer_update_interval;

static void set_up_discovery_service(jnx_hashmap *config, app_context_t *context) {
	char *user_name = (char *) jnx_hash_get(config, "USER_NAME");
	if (user_name == NULL) {
		JNX_LOG(0, "[ERROR] You must supply the user name in the configuration. Add USER_NAME=username line to the config file.");
		exit(1);
	}
	peerstore *ps = peerstore_init(local_peer_for_user(user_name));

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
app_context_t *create_app_context(jnx_hashmap *config) {
	app_context_t *context = calloc(1, sizeof(app_context_t));
	set_up_discovery_service(config, context);
	return context;
}
void destroy_app_context(app_context_t **context) {
	discovery_service_cleanup(&(*context)->discovery);
	free(*context);
	*context = NULL;
}
