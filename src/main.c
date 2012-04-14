#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "global.h"
#include "main.h"
#include "data.h"
#include "englishui.h"

struct _ui ui;

static gboolean
on_wubi_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide_all (current_window);
  current_window = global.wubi_window;
  gtk_notebook_set_current_page (GTK_NOTEBOOK (global.wubi_notebook), 0);
  gtk_widget_show_all (current_window);
  return FALSE;
}

/* FIXME: shouldn't be global */
gboolean
on_menu_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide_all (current_window);
  current_window = global.menu_window;
  gtk_widget_show_all (current_window);
  return FALSE;
}

int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *button;

  gtk_init (&argc, &argv);
  data_precheck_and_init();

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, UIFILE, NULL); /* UIFILE from data.h */

  global.menu_window = GTK_WIDGET (gtk_builder_get_object (builder, "menu_window"));
  current_window = global.menu_window;
  global.wubi_window = GTK_WIDGET (gtk_builder_get_object (builder, "wubi_window"));

  button = GTK_WIDGET (gtk_builder_get_object (builder, "button3")); /* main_menu->english_typing */
  g_signal_connect (button, "clicked", G_CALLBACK (on_englishui_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button4")); /* main_menu->wubi_typing */
  g_signal_connect (button, "clicked", G_CALLBACK (on_wubi_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button10")); /* wubi_window->base_menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button12")); /* wubi_window->character_menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button14")); /* wubi_window->word_menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button16")); /* wubi_window->article_menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);

  global.english_notebook = GTK_WIDGET (gtk_builder_get_object (builder, "english_notebook"));
  global.wubi_notebook = GTK_WIDGET (gtk_builder_get_object (builder, "wubi_notebook"));

  englishui_init (builder);		/* english.c */

  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (G_OBJECT (builder));

  gtk_widget_show_all (current_window);
  gtk_main ();
  englishui_deinit();
  data_deinit();
  exit (EXIT_SUCCESS);
}
