#include <stdlib.h>
#include <gtk/gtk.h>

static GdkPixbuf *background, *frame, *key, *cur_key;
static GtkWidget *draw;
static gint bg_width, bg_height, key_width, key_height;

static gint
on_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  cairo_t *cr;

  cr = gdk_cairo_create (event->window);

  gdk_pixbuf_copy_area (background, 0, 0, bg_width, bg_height, frame, 0, 0);

  if (cur_key) {
    gdk_pixbuf_composite (cur_key, frame,
			  351, 115, key_width, key_height,
			  351, 115, 1, 1, GDK_INTERP_BILINEAR, 128);
    /* G 350,352 */
  }

  gdk_cairo_set_source_pixbuf (cr, frame, 0, 0);
  gdk_cairo_rectangle (cr, &event->area);
  cairo_fill (cr);
  g_print ("%d %d %d %d\n", event->area.x, event->area.y, event->area.width, event->area.height);
  cairo_destroy (cr);

  return FALSE;
}

static gboolean
on_press (GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  g_print ("%x\n", event->keyval);
  cur_key = key;
  gtk_widget_queue_draw (draw);
  return FALSE;
}

static gint
timeout (gpointer data)
{
  GDK_THREADS_ENTER ();
  gtk_widget_queue_draw (draw);
  GDK_THREADS_LEAVE ();
  return TRUE;
}

static guint timeout_id;

static void
on_destroy (GtkObject *object, gpointer data)
{
  g_source_remove (timeout_id);
  timeout_id = 0;
}

int
main(int argc, char *argv[])
{
  GtkWidget *window, *vbox, *label;

  gtk_init(&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy", G_CALLBACK (on_destroy), NULL);
  g_signal_connect (window, "destroy", gtk_main_quit, NULL);
  g_signal_connect (window, "key-press-event", G_CALLBACK (on_press), NULL);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);

  vbox = gtk_vbox_new (FALSE, 10);

  gtk_container_add (GTK_CONTAINER (window), vbox);
  label = gtk_label_new ("label0");
  gtk_box_pack_start (GTK_BOX (vbox), label, TRUE, TRUE, 0);

  draw = gtk_drawing_area_new ();
  gtk_box_pack_start (GTK_BOX (vbox), draw, TRUE, TRUE, 0);

  background = gdk_pixbuf_new_from_file ("/home/c-aries/git/utt/data/media/en-kb.png", NULL);
  if (!background) {
    g_error ("background fail\n");
  }
  bg_width = gdk_pixbuf_get_width (background);
  bg_height = gdk_pixbuf_get_height (background);
  gtk_widget_set_size_request (draw, bg_width, bg_height);
  frame = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, bg_width, bg_height);
  key = gdk_pixbuf_new_from_file ("/home/c-aries/git/utt/data/media/key.png", NULL);
  key_width = gdk_pixbuf_get_width (key);
  key_height = gdk_pixbuf_get_height (key);
  g_signal_connect (draw, "expose-event", G_CALLBACK (on_expose), NULL);

  timeout_id = g_timeout_add (1000, timeout, NULL);

  gtk_widget_show_all (window);
  gtk_main();
  exit(EXIT_SUCCESS);
}
