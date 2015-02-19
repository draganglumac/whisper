/*
 * =====================================================================================
 *
 *       Filename:  gui.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/19/2015 06:49:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "gui.h"
#include <pthread.h>
#define COL_LOGO   1
#define COL_LOCAL  2
#define COL_REMOTE 3

void init_colours() {
  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support colours.\n");
    exit(1);
  }
  start_color();
  init_pair(COL_LOGO, COLOR_WHITE, COLOR_BLUE);
  init_pair(COL_LOCAL, COLOR_WHITE, COLOR_BLACK);
  init_pair(COL_REMOTE, COLOR_GREEN, COLOR_BLACK);
}
void show_prompt(ui_t *ui) {
  wmove(ui->prompt, 1, 1);
  wclear(ui->prompt);
  mvwprintw(ui->prompt, 1, 1, "$> ");
  wrefresh(ui->prompt);
}
void display_logo() {
  attron(COLOR_PAIR(COL_LOGO) | A_BOLD);
  printw("%s", " Whisper Chat ");
  attroff(COLOR_PAIR(COL_LOGO) | A_BOLD);
  refresh();
}
gui_object *gui_create() {

  gui_object *g = malloc(sizeof(gui_object));
  pthread_mutex_init(&(*g).output_mutex,NULL);
  pthread_cond_init(&(*g).output_cond,NULL);

  ui_t *ui = malloc(sizeof(ui_t));
  initscr();
  init_colours();
  display_logo();
  ui->screen = newwin(LINES - 6, COLS - 1, 1, 1);
  scrollok(ui->screen, TRUE);
  box(ui->screen, 0, 0);
  ui->next_line = 1;
  wrefresh(ui->screen);
  ui->prompt = newwin(4, COLS - 1, LINES - 5, 1);
  show_prompt(ui);

  g->ui_handle = ui;
  return g;
}
void gui_destroy(gui_object **g) {
  delwin((*g)->ui_handle->screen);
  delwin((*g)->ui_handle->prompt);
  endwin();
  free(*g);
  *g = NULL;
}
char *get_message(gui_object *g) {
  char *msg = malloc(1024);
  wmove(g->ui_handle->prompt, 1, 4);
  wgetstr(g->ui_handle->prompt, msg);
  show_prompt(g->ui_handle);
  return msg;
}
void update_next_line(ui_t *ui) {
  int lines, cols;
  getmaxyx(ui->screen, lines, cols);
  if (ui->next_line >= --lines) {
    scroll(ui->screen);
    ui->next_line--;
  }
  else {
    getyx(ui->screen, lines, cols);
    ui->next_line = lines;
  }
}
void display_message(ui_t *ui, char *msg, int col_flag) {
  int row, col;
  getyx(ui->prompt, row, col);
  wattron(ui->screen, COLOR_PAIR(col_flag));
  mvwprintw(ui->screen, ui->next_line, 1, "%s\n", msg);
  wattroff(ui->screen, COLOR_PAIR(col_flag));
  update_next_line(ui);
  box(ui->screen, 0, 0);
  wrefresh(ui->screen);
  wmove(ui->prompt, row, col);
  wrefresh(ui->prompt);
}
void display_local_message(gui_object *g, char *msg) {
  display_message(g->ui_handle, msg, COL_LOCAL);
  free(msg);
}
void display_remote_message(gui_object *g, char *msg) {
  display_message(g->ui_handle, msg + 2, COL_REMOTE);
  free(msg);
}
