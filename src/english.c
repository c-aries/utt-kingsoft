#include <gtk/gtk.h>
#include "main.h"

#ifndef DATADIR
#error "DATADIR doesn't exist"
#endif

#define ENGLISH_KEYBOARD DATADIR "/media/en-kb.png"

static GdkPixbuf *frame;

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
  gdk_pixbuf_copy_area (ui.icon[DATA_ENGLISH_KB].pix, 0, 0, ui.icon[DATA_ENGLISH_KB].w, ui.icon[DATA_ENGLISH_KB].h, frame, 0, 0);
  gdk_cairo_set_source_pixbuf (cr, frame, 0, 0);
  gdk_cairo_rectangle (cr, &event->area);
  cairo_fill (cr);

  cairo_destroy (cr);
  return FALSE;
}

void
english_ui_init (GtkBuilder *builder)
{
  GtkWidget *button;
  GtkWidget *kb_draw;
  gint width, height;

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

  width = ui.icon[DATA_ENGLISH_KB].w;
  height = ui.icon[DATA_ENGLISH_KB].h;
  frame = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, width, height);

  kb_draw = GTK_WIDGET (gtk_builder_get_object (builder, "kb_draw"));
  g_signal_connect (kb_draw, "expose-event", G_CALLBACK (on_expose), NULL);
  gtk_widget_set_size_request (kb_draw, width, height);

  g_signal_connect (ui.english_window, "key-press-event", G_CALLBACK (on_key_press), NULL);
}
