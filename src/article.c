#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <utt/article.h>
#include "common.h"

#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define UIFILE PKGDATADIR "/article-ui.xml"

static GtkTextBuffer *buffer;
static GtkWidget *article;

static void
test2 (gchar *filename)
{
  GtkTextIter iter;
  gchar *contents;
  gsize length;

  if (g_file_get_contents (filename, &contents, &length, NULL)) {
    gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
    gtk_text_buffer_insert (buffer, &iter, contents, -1);
    g_free (contents);
  }
}

static void
on_open_item_activate (GtkMenuItem *menuitem, gpointer user_data)
{
  GtkWidget *chooser = user_data;
  gchar *filename;

  if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (chooser));
    test2 (filename);
    g_print ("open %s\n", filename);
    utt_article_open_file (UTT_ARTICLE (article), filename);
    g_free (filename);
  }
  gtk_widget_hide (chooser);
}

int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *window, *open_item, *chooser;
  GtkTextView *textview;
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

  chooser = GTK_WIDGET (gtk_builder_get_object (builder, "filechooserdialog1"));
  gtk_widget_set_can_default (window, TRUE);

  open_item = GTK_WIDGET (gtk_builder_get_object (builder, "imagemenuitem2"));
  g_signal_connect (open_item, "activate", G_CALLBACK (on_open_item_activate), chooser);

  textview = GTK_TEXT_VIEW (gtk_builder_get_object (builder, "textview1"));
  buffer = gtk_text_view_get_buffer (textview);

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
