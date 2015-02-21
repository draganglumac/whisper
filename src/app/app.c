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
void list_active_peers() {
  printf("\n");
  printf("Listing active peers (TODO)\n");
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
app_context_t *create_app_context(jnx_hashmap *config) {
	app_context_t *context = calloc(1, sizeof(app_context_t));

	int port = atoi((char *) jnx_hash_get(config, "DISCOVERY_PORT"));
	char *broadcast_address = (char *) jnx_hash_get(config, "DISCOVERY_MULTICAST_IP");
	
//	context->discovery = discovery_service_create(port, AF_INET, broadcast_address, );
//	discovery_service_start(context->discovery, BROADCAST_UPDATE_STRATEGY);
	return context;
}
void destroy_app_context(app_context_t **context) {
//	discovery_service_cleanup(&context->discovery);
	free(*context);
	*context = NULL;
}
