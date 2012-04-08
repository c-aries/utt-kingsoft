#include <gtk/gtk.h>
#include "global.h"		/* icon */
#include "data.h"		/* ICON_KB_EN */
#include "main.h"

#define DEBUG 1

static cairo_surface_t *kb_surface;	/* keyboard surface */
static gint key_index = 0;
static GtkWidget *choose_dialog;

static gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  gpointer ret;

  g_print ("key press %08x\n", event->keyval);
  ret = g_hash_table_lookup (key_ht, GUINT_TO_POINTER (event->keyval));
  if (ret) {
    key_index = GPOINTER_TO_INT (ret);
    g_print ("key %s\n", key[key_index].name);
  }
  else {
    key_index = 0;
  }
  gtk_widget_queue_draw (widget);
  return FALSE;
}

static gint
on_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  cairo_t *cr;
  struct _key *keyp;

  cr = gdk_cairo_create (event->window);
  cairo_set_source_surface (cr, kb_surface, 0, 0);
  cairo_paint (cr);

  if (key_index) {
    cairo_set_line_width (cr, 1);
    cairo_set_source_rgba (cr, 0, 0, 1, 0.3);
    keyp = &key[key_index];
    cairo_rectangle (cr, keyp->startx, keyp->starty, keyp->width, keyp->height);
    cairo_fill (cr);
  }

  cairo_destroy (cr);
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

static gboolean
on_choose_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gint ret;

  ret = gtk_dialog_run (GTK_DIALOG (choose_dialog));
  g_print ("ret %d\n", ret);
  return FALSE;
}

void
englishui_init (GtkBuilder *builder)			/* english ui init */
{
  GtkWidget *english_window;
  gint kb_width, kb_height;
  GtkWidget *button;
  GtkWidget *kb_draw;

  english_window = global.english_window = GTK_WIDGET (gtk_builder_get_object (builder, "english_window"));

  /* menu button */
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button1")); /* change to english->layout menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button2")); /* change to english->character menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button5")); /* change to english->article menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  choose_dialog = GTK_WIDGET (gtk_builder_get_object (builder, "choose_dialog1"));
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button6")); /* english->layout->choose */
  g_signal_connect (button, "clicked", G_CALLBACK (on_choose_press), NULL);

  kb_surface = cairo_image_surface_create_from_png(icon[ICON_KB_EN].path);
  kb_width = cairo_image_surface_get_width(kb_surface);
  kb_height = cairo_image_surface_get_height(kb_surface);

  kb_draw = GTK_WIDGET (gtk_builder_get_object (builder, "kb_draw"));
  g_signal_connect (kb_draw, "expose-event", G_CALLBACK (on_expose), NULL);
  gtk_widget_set_size_request (kb_draw, kb_width, kb_height);

  g_signal_connect (english_window, "key-press-event", G_CALLBACK (on_key_press), NULL);
}

void englishui_deinit()
{
  if (kb_surface) {
    cairo_surface_destroy(kb_surface);
  }
}
