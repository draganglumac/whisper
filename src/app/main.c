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
int run_app(app_context_t *context) {
  char command[CMDLEN];

  intro();
  while (1) {
    prompt();
  
    char *cmd_string = NULL;
    jnx_size read_bytes;
    jnx_size s = getline(&cmd_string,&read_bytes,stdin);
    char *param = NULL;
    switch(code_for_command_with_param(cmd_string,read_bytes,&param)) {
      case CMD_SESSION:
        //peer *p = peerstore_lookup_with_name(context->??,param);     
        if(param) {
          free(param);
        }
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
  }
  return 0;
}
int main(int argc, char **argv) {
  jnx_hashmap *config = load_config(argc, argv);
  app_context_t *app_context = create_app_context(config);
  run_app(app_context);
  destroy_app_context(&app_context);
  jnx_hash_destroy(&config);
  printf("Done.\n");
  return 0;
}
