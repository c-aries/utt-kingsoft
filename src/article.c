#include <stdlib.h>
#include <gtk/gtk.h>
#include <utt/article.h>
#include "common.h"

#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define UIFILE PKGDATADIR "/article-ui.xml"

static void
on_open_item_activate (GtkMenuItem *menuitem, gpointer user_data)
{
  GtkWidget *chooser = user_data;
  gchar *filename;

  if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));
    g_print ("open %s\n", filename);
    g_free (filename);
  }
  gtk_widget_hide (chooser);
}

int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *window, *open_item, *chooser;

  utt_set_locale ();

  gtk_init (&argc, &argv);

  g_debug("PKGDATADIR %s", PKGDATADIR);

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, UIFILE, NULL);

  window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
  if (!window) {
    g_error ("Can't get %s from %s", "window1", UIFILE);
  }

  chooser = GTK_WIDGET (gtk_builder_get_object (builder, "filechooserdialog1"));
  gtk_widget_set_can_default (window, TRUE);

  open_item = GTK_WIDGET (gtk_builder_get_object (builder, "imagemenuitem2"));
  g_signal_connect (open_item, "activate", G_CALLBACK (on_open_item_activate), chooser);


  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (G_OBJECT (builder));

  gtk_widget_show_all (window);
  gtk_main ();
  exit (EXIT_SUCCESS);
}
