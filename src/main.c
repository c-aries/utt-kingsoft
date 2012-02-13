#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "main.h"
#include "english.h"

#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define UIFILE PKGDATADIR "/utt.glade"

#ifndef DATADIR
#error "DATADIR doesn't exist"
#endif

struct _ui ui;

static gboolean
on_english_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide_all (ui.current_window);
  gtk_widget_show_all (ui.english_window);
  ui.current_window = ui.english_window;
  gtk_notebook_set_current_page (GTK_NOTEBOOK (ui.english_notebook), 0);
/*   gtk_window_fullscreen (GTK_WINDOW (ui.current_window)); */
  return FALSE;
}

static gboolean
on_wubi_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide_all (ui.current_window);
  gtk_widget_show_all (ui.wubi_window);
  ui.current_window = ui.wubi_window;
  gtk_notebook_set_current_page (GTK_NOTEBOOK (ui.wubi_notebook), 0);
  return FALSE;
}

gboolean
on_menu_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide_all (ui.current_window);
  gtk_widget_show_all (ui.menu_window);
  ui.current_window = ui.menu_window;
  return FALSE;
}

int
main (int argc, char *argv[])
{
  GtkBuilder *builder;
  GtkWidget *button;

  gtk_init (&argc, &argv);

  if (!g_file_test (UIFILE, G_FILE_TEST_EXISTS)) { /* pre check */
    g_error ("%s doesn't exists", UIFILE);
  }

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, UIFILE, NULL);

  ui.icon = load_icons ();
  ui.key = load_keys ();

  ui.menu_window = ui.current_window = GTK_WIDGET (gtk_builder_get_object (builder, "menu_window"));
  ui.wubi_window = GTK_WIDGET (gtk_builder_get_object (builder, "wubi_window"));

  button = GTK_WIDGET (gtk_builder_get_object (builder, "button3"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_english_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button4"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_wubi_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button10"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button12"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button14"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button16"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);

  ui.english_notebook = GTK_WIDGET (gtk_builder_get_object (builder, "english_notebook"));
  ui.wubi_notebook = GTK_WIDGET (gtk_builder_get_object (builder, "wubi_notebook"));

  english_ui_init (builder);

  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (G_OBJECT (builder));

  gtk_widget_show_all (ui.menu_window);
  gtk_main ();
  exit (EXIT_SUCCESS);
}
