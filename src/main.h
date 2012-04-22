#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>
#include "data.h"

/* FIXME: shouldn't be global */
gboolean on_menu_press (GtkWidget *widget, GdkEventButton *event, gpointer data);

#endif
