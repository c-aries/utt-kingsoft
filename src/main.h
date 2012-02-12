#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>
#include "data.h"

struct _ui {
  GtkWidget *current_window;
  GtkWidget *menu_window;
  GtkWidget *english_window;
  GtkWidget *wubi_window;
  GtkWidget *english_notebook;
  GtkWidget *wubi_notebook;
  struct icon *icon;
  struct key *key;
};

extern struct _ui ui;

gboolean on_menu_press (GtkWidget *widget, GdkEventButton *event, gpointer data);

#endif

