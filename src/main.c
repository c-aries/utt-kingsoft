#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define UIFILE PKGDATADIR "/utt.glade"

int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *window;

  gtk_init (&argc, &argv);

  if (!g_file_test (UIFILE, G_FILE_TEST_EXISTS)) { /* pre check */
    g_error ("%s doesn't exists", UIFILE);
  }

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, UIFILE, NULL);

  window = GTK_WIDGET (gtk_builder_get_object (builder, "menu_window"));

  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (G_OBJECT (builder));

  gtk_widget_show_all (window);
  gtk_main ();
  exit (EXIT_SUCCESS);
}
