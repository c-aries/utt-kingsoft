#include <gtk/gtk.h>
#include "global.h"		/* icon */
#include "data.h"		/* ICON_KB_EN */
#include "main.h"

/* static GdkPixbuf *frame; */

static gboolean
on_key_press (GtkWidget *widget, GdkEventKey *key, gpointer data)
{
  g_print ("key press %08x\n", key->keyval);
  return FALSE;
}

static gint
on_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
/*   cairo_t *cr; */

/*   cr = gdk_cairo_create (event->window); */
/*   gdk_pixbuf_copy_area (ui.icon[DATA_ENGLISH_KB].pix, 0, 0, ui.icon[DATA_ENGLISH_KB].w, ui.icon[DATA_ENGLISH_KB].h, frame, 0, 0); */
/*   gdk_cairo_set_source_pixbuf (cr, frame, 0, 0); */
/*   gdk_cairo_rectangle (cr, &event->area); */
/*   cairo_fill (cr); */

/*   cairo_destroy (cr); */
  return FALSE;
}

gboolean
on_englishui_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide_all(current_window);
  current_window = global.english_window;
  gtk_notebook_set_current_page(GTK_NOTEBOOK(global.english_notebook), 0);
  gtk_widget_show_all(current_window);
  return FALSE;
}

void
englishui_init (GtkBuilder *builder)			/* english ui init */
{
  GtkWidget *english_window;
  cairo_surface_t *kb_surface;	/* keyboard surface */
  gint kb_width, kb_height;
  GtkWidget *button;
  GtkWidget *kb_draw;
/*   gint width, height; */

  english_window = global.english_window = GTK_WIDGET (gtk_builder_get_object (builder, "english_window"));

  /* menu button */
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button1")); /* change to english->layout menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button2")); /* change to english->character menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button5")); /* change to english->article menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);

/*   width = ui.icon[DATA_ENGLISH_KB].w; */
/*   height = ui.icon[DATA_ENGLISH_KB].h; */
/*   frame = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, width, height); */

  kb_surface = cairo_image_surface_create_from_png(icon[ICON_KB_EN].path);
  kb_width = cairo_image_surface_get_width(kb_surface);
  kb_height = cairo_image_surface_get_height(kb_surface);

  kb_draw = GTK_WIDGET (gtk_builder_get_object (builder, "kb_draw"));
  g_signal_connect (kb_draw, "expose-event", G_CALLBACK (on_expose), NULL);
  gtk_widget_set_size_request (kb_draw, kb_width, kb_height);

  g_signal_connect (english_window, "key-press-event", G_CALLBACK (on_key_press), NULL);
}
