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

#define CMDLEN 64

#define CMD_SESSION 1
#define CMD_LIST    2
#define CMD_HELP    3
#define CMD_QUIT    4

void intro();
void prompt();
void show_help();
void quit_message();
int code_for_command(char *command);
void create_gui_session();
void list_active_peers();

#endif // __APP_H__