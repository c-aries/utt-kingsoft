#include <stdlib.h>
#include <gtk/gtk.h>
#include <utt/article.h>
#include "common.h"

#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define UIFILE PKGDATADIR "/article-ui.xml"

int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *window;

  utt_set_locale ();

  g_print ("Universal Typing Training %s Module\n", _("Artical"));
  gtk_init (&argc, &argv);

  g_debug("PKGDATADIR %s", PKGDATADIR);

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, UIFILE, NULL);

  window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
  if (!window) {
    g_error ("Can't get %s from %s", "window1", UIFILE);
  }

  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (G_OBJECT (builder));

  gtk_widget_show_all (window);
  gtk_main ();
  exit (EXIT_SUCCESS);
}
