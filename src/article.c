#include <stdlib.h>
#include <gtk/gtk.h>
#include <utt/article.h>

#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define UIFILE PKGDATADIR "/article-ui.xml"

int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *window;

  g_print ("Universal Typing Training Artical Module\n");

  gtk_init (&argc, &argv);

  g_debug("PKGDATADIR %s", PKGDATADIR);

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, UIFILE, NULL);

  window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
  if (!window) {
    g_error ("Can't get %s from %s", "window1", UIFILE);
  }

/*   window = gtk_window_new (GTK_WINDOW_TOPLEVEL); */
/*   gtk_widget_set_size_request (window, 481, 273); */
/*   gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER); */
/*   gtk_window_set_title (GTK_WINDOW (window), "artical typing training"); */
/*   g_signal_connect (window, "destroy", gtk_main_quit, NULL); */

/*   article = utt_article_new (); */
/*   gtk_container_add (GTK_CONTAINER (window), article); */

  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (G_OBJECT (builder));

  gtk_widget_show_all (window);
  gtk_main ();
  exit (EXIT_SUCCESS);
}
