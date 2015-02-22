/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main launch point for whisper
 *
 *        Version:  1.0
 *        Created:  19/01/15 09:36:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  AlexsJones (jonesax@hush.com), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#include "app.h"

jnx_hashmap *load_config(int argc, char **argv) {
  if (argc > 1) {
    int i = 1;
    while (i < argc) {
      if (0 == strncmp("--config=", argv[i], 9)) {
        char *path = argv[i] + 9;
        jnx_hashmap *config = jnx_file_read_kvp(path, 80, "="); 
        if (config != NULL) {
          return config;
        }
      }
      i++;
    }
  }

  JNX_LOG(0, "[ERROR] You must supply a valid configuration file on the command line. Pass it using --config=PATH_TO_CONFIG_FILE command line option.");
  exit (1);
}

jnx_list *user_input;
pthread_mutex_t user_input_mutex;
pthread_cond_t user_input_condition;

void *input_loop(void *data) {
  char command[CMDLEN];
  int *safe_to_poll = (int *) data;
  while(*safe_to_poll) {
    scanf("%s", command);
    jnx_list_add_ts(user_input, (void *) command);
    pthread_cond_signal(&user_input_condition);
  }
}
void start_polling_user_input(int *safe_to_poll) {
  jnx_thread_create_disposable(input_loop, (void *) safe_to_poll);
}
// *** FAKE STUFF ***
void *dud(void *data) {
  sleep(10);
  jnx_list_add_ts(user_input, "accept");
  pthread_cond_signal(&user_input_condition);
}
void accept_connections(app_context_t *context) {
  jnx_thread_create_disposable(dud, (void *) context);
}
// *** END FAKE STUFF ***
int run_app(app_context_t *context) {
  user_input = jnx_list_create();
  pthread_mutex_init(&user_input_mutex, NULL);
  pthread_cond_init(&user_input_condition, NULL);
  pthread_mutex_lock(&user_input_mutex);
  int safe_to_poll = 1;
  intro();
  start_polling_user_input(&safe_to_poll);
  accept_connections(context);
  while (1) {
    prompt();
    pthread_cond_wait(&user_input_condition, &user_input_mutex);
    char *command = (char *) jnx_list_remove_ts(&user_input);
    switch(code_for_command(command)) {
      case CMD_SESSION:
        safe_to_poll = 0;
        create_gui_session();
        safe_to_poll = 1;
        start_polling_user_input(&safe_to_poll);
        break;
      case CMD_ACCEPT:
        handle_connection_request(context);
        break;
      case CMD_LIST:
        list_active_peers(context);
        break;
      case CMD_HELP:
        show_help();
        break;
      case CMD_QUIT:
        quit_message();
        return 0;
    }
    pthread_mutex_unlock(&user_input_mutex);
  }

  jnx_list_destroy(&user_input);
  return 0;
}
int main(int argc, char **argv) {
  jnx_hashmap *config = load_config(argc, argv);
  app_context_t *app_context = create_app_context(config);
  run_app(app_context);
  //  destroy_app_context(&app_context);
  jnx_hash_destroy(&config);
  printf("Done.\n");
  return 0;
}
