#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <gtk/gtk.h>
#include "data.h"

/* not so frequecely use */
struct _global {
  GtkWidget *menu_window;
  GtkWidget *english_window;
  GtkWidget *wubi_window;
  GtkWidget *english_notebook;
  GtkWidget *wubi_notebook;
  void *padding;
} global;

/* frequencely use variablies */
extern struct _icon icon[ICON_NUM]; /* data.c */
extern GtkWidget *current_window;   /* global.c */

#endif

