#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define UIFILE PKGDATADIR "/utt.glade"

struct _ui {
  GtkWidget *current_window;
  GtkWidget *menu_window;
  GtkWidget *english_window;
  GtkWidget *wubi_window;
} ui;

static gboolean
on_english_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide (ui.current_window);
  gtk_widget_show_all (ui.english_window);
  ui.current_window = ui.english_window;
  return FALSE;
}

static gboolean
on_wubi_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide (ui.current_window);
  gtk_widget_show_all (ui.wubi_window);
  ui.current_window = ui.wubi_window;
  return FALSE;
}

static gboolean
on_menu_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide (ui.current_window);
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

  ui.menu_window = ui.current_window = GTK_WIDGET (gtk_builder_get_object (builder, "menu_window"));
  ui.english_window = GTK_WIDGET (gtk_builder_get_object (builder, "english_window"));
  ui.wubi_window = GTK_WIDGET (gtk_builder_get_object (builder, "wubi_window"));

  button = GTK_WIDGET (gtk_builder_get_object (builder, "button3"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_english_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button4"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_wubi_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button1"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button2"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button5"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button10"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button12"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button14"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button16"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);

  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (G_OBJECT (builder));

  gtk_widget_show_all (ui.menu_window);
  gtk_main ();
  exit (EXIT_SUCCESS);
}
