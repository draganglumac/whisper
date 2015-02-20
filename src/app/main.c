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

int main(int argc, char **argv) {
	char command[CMDLEN];

	intro();
	while (1) {
		prompt();
		scanf("%s", command); 

		switch(code_for_command(command)) {
			case CMD_SESSION:
				create_gui_session();
				break;
			case CMD_LIST:
				list_active_peers();
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
