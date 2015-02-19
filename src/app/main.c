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
int main(int argc, char **argv) {

  create_gui_session();

  return 0;
}
