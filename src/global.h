#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <gtk/gtk.h>
#include "data.h"		/* ICON_NUM etc. */
#include "stat.h"		/* statistics */

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
extern GHashTable *key_val_ht, *key_name_ht, *key_char_ht;		/* key's hash table */
extern struct _key key[];		/* keys array */
extern GtkWidget *current_window;   /* global.c */
extern struct _stat stat;

#endif
