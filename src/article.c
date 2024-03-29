#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <utt/article.h>
#include "common.h"

#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define UIFILE PKGDATADIR "/article-ui.xml"

static GtkWidget *article;

static void
on_open_item_activate (GtkMenuItem *menuitem, gpointer user_data)
{
  GtkWindow *window = user_data;
  GtkWidget *msg, *chooser;
  gchar *filename = NULL;
  gboolean ret = TRUE;

  chooser = gtk_file_chooser_dialog_new (_("Choose a file"),
					 window,
					 GTK_FILE_CHOOSER_ACTION_OPEN,
					 GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					 NULL);
  if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));
    g_print ("open %s\n", filename);
    ret = utt_article_open_file (UTT_ARTICLE (article), filename);
  }
  gtk_widget_destroy (chooser);
  if (!ret) {
    msg = gtk_message_dialog_new (GTK_WINDOW (window),
				  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
				  GTK_MESSAGE_INFO,
				  GTK_BUTTONS_OK,
				  _("%s is not a validate UTF-8 file."), filename);
    gtk_dialog_run (GTK_DIALOG (msg));
    gtk_widget_destroy (msg);
  }
  if (filename) {
    g_free (filename);
  }
}

int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *window, *open_item;
  gint border = 0;

  utt_set_locale ();

  gtk_init (&argc, &argv);

  g_debug("PKGDATADIR %s", PKGDATADIR);

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, UIFILE, NULL);

  window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
  if (!window) {
    g_error ("Can't get %s from %s", "window1", UIFILE);
  }

/*   gtk_widget_set_can_default (window, TRUE); */

  open_item = GTK_WIDGET (gtk_builder_get_object (builder, "imagemenuitem2"));
  g_signal_connect (open_item, "activate", G_CALLBACK (on_open_item_activate), window);

  article = GTK_WIDGET (gtk_builder_get_object (builder, "article1"));
  g_object_get (G_OBJECT (article),
		"border", &border,
		NULL);
  g_print ("border %d\n", border);

  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (G_OBJECT (builder));

  gtk_widget_show_all (window);
  gtk_main ();
  exit (EXIT_SUCCESS);
}
