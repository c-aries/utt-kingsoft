#include <glib/gprintf.h>
#include "commonui.h"

void
progress_bar_update (GtkWidget *bar, struct _stat *stat)
{
  gchar finishstamp[5];	/* format:"100%" */
  gdouble finish;

  finish = stat_finish (stat);
  g_sprintf (finishstamp, "%d%%", (gint)finish);
  finish /= 100;
  gtk_progress_bar_set_text (GTK_PROGRESS_BAR (bar), finishstamp);
  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (bar), finish);
}
