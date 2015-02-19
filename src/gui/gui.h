/*
 * =====================================================================================
 *
 *       Filename:  gui.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/19/2015 06:48:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __GUI_H__
#define __GUI_H__
#include <pthread.h>
#include <ncurses.h>
typedef struct {
  WINDOW *prompt;
  WINDOW *screen;	
  int next_line;
} ui_t;

typedef struct gui_object {
  ui_t *ui_handle;
}gui_object;

typedef struct {
  ui_t *ui;
  char *msg;
  gui_object *g;
} context_t;

typedef struct data_transfer_obj {
  gui_object *g;
  context_t *c;
}data_transfer_obj;
gui_object *gui_create();
void gui_destroy(gui_object **g);
char *get_message(context_t *c);;
void display_local_message(gui_object *g, char *msg);
void display_remote_message(gui_object *g, char *msg);

void *read_loop(void *data);
int output_next_message_in_context(context_t *context);

#endif
