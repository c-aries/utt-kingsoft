#include <gtk/gtk.h>
#include "main.h"

#ifndef DATADIR
#error "DATADIR doesn't exist"
#endif

#define ENGLISH_KEYBOARD DATADIR "/media/en-kb.png"

static gboolean
on_key_press (GtkWidget *widget, GdkEventKey *key, gpointer data)
{
  g_print ("key press %08x\n", key->keyval);
  return FALSE;
}

static gint
on_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  cairo_t *cr;

  cr = gdk_cairo_create (event->window);

  cairo_destroy (cr);
  return FALSE;
}

void
english_ui_init (GtkBuilder *builder)
{
  GtkWidget *button;
  GtkWidget *kb_draw;

  if (!g_file_test (ENGLISH_KEYBOARD, G_FILE_TEST_EXISTS)) { /* pre check */
    g_error ("%s doesn't exists", ENGLISH_KEYBOARD);
  }

  ui.english_window = GTK_WIDGET (gtk_builder_get_object (builder, "english_window"));

  /* menu button */
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button1"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button2"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button5"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);

  kb_draw = GTK_WIDGET (gtk_builder_get_object (builder, "kb_draw"));
  g_signal_connect (kb_draw, "expose-event", G_CALLBACK (on_expose), NULL);
  gtk_widget_set_size_request (kb_draw, ui.icon[DATA_ENGLISH_KB].w, ui.icon[DATA_ENGLISH_KB].h);

  g_signal_connect (ui.english_window, "key-press-event", G_CALLBACK (on_key_press), NULL);
}
