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
#include <ncurses.h>
typedef struct {
  WINDOW *prompt;
  WINDOW *screen;	
  int next_line;
} ui_t;

typedef struct gui_object {
  pthread_mutex_t output_mutex;
  pthread_cond_t output_cond;
  ui_t *ui_handle;

}gui_object;

typedef struct {
  ui_t *ui;
  char *msg;
} context_t;

gui_object *gui_create();
void gui_destroy(gui_object **g);
char *get_message(gui_object *g);
void display_local_message(gui_object *g, char *msg);
void display_remote_message(gui_object *g, char *msg);

#endif
