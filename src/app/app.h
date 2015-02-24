/*
 * =====================================================================================
 *
 *       Filename:  app.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/02/15 15:47:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (draganglumac), dragan.glumac@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __APP_H__
#define __APP_H__

#include <jnxc_headers/jnxhash.h>

#include "../net/discovery.h"

#define CMDLEN 64

#define CMD_SESSION 1
#define CMD_LIST    2
#define CMD_HELP    3
#define CMD_QUIT    4

typedef struct {
  discovery_service *discovery;
} app_context_t;

app_context_t *app_create_context(jnx_hashmap *config);
void app_destroy_context(app_context_t **app_context);

void app_intro();
void app_prompt();
void app_show_help();
void app_quit_message();
int app_code_for_command_with_param(char *command,\
    jnx_size cmd_len, char **oparam);
void app_create_gui_session();
void app_list_active_peers(app_context_t *context);

#endif // __APP_H__
