#ifndef __COMMONUI_H__
#define __COMMONUI_H__

#include <gtk/gtk.h>
#include "stat.h"

void progress_bar_update (GtkWidget *bar, struct _stat *stat);

#endif

