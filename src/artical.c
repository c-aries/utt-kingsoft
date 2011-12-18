#include <stdlib.h>
#include <gtk/gtk.h>
#include <utt/artical.h>

int
main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *artical;

  g_print ("Universal Typing Training Artical Module\n");

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request (window, 481, 273);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW (window), "artical typing training");
  g_signal_connect (window, "destroy", gtk_main_quit, NULL);

  artical = utt_artical_new ();
  gtk_container_add (GTK_CONTAINER (window), artical);

  gtk_widget_show_all (window);
  gtk_main ();
  exit (EXIT_SUCCESS);
}
