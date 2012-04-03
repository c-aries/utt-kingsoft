#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <gtk/gtk.h>
#include "data.h"

struct _global {
  GtkBuilder *builder;		/* plan to remove */
  GtkWidget *menu_window;
  GtkWidget *english_window;
  GtkWidget *wubi_window;
  GtkWidget *english_notebook;
  GtkWidget *wubi_notebook;
  void *padding;
} global;

extern struct _icon icon[ICON_NUM];
extern GtkWidget *current_window;

#endif

