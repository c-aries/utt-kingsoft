#include <gtk/gtk.h>
#include "global.h"		/* icon */
#include "data.h"		/* ICON_KB_EN */
#include "main.h"

#define DEBUG 1

static cairo_surface_t *kb_surface;	/* keyboard surface */
static gint key_index = 0;
static GtkWidget *choose_treeview; /* class choose list */

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
on_choose_press (GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;
  GtkWidget *content;

  dialog = gtk_dialog_new_with_buttons ("choose class",
					GTK_WINDOW (global.english_window),
					GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_STOCK_OK, GTK_RESPONSE_OK,
					GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
					NULL);
  content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  gtk_container_add (GTK_CONTAINER (content), choose_treeview);
  gtk_widget_show_all (dialog);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
  return FALSE;
}

void
englishui_init (GtkBuilder *builder)			/* english ui init */
{
  GtkWidget *english_window;
  gint kb_width, kb_height, i;
  GtkWidget *button;
  GtkWidget *kb_draw;
  /* choose list widgets */
  GtkListStore *choose_store;
  GtkTreeIter iter;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *col;

  english_window = global.english_window = GTK_WIDGET (gtk_builder_get_object (builder, "english_window"));

  /* menu button */
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button1")); /* change to english->layout menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button2")); /* change to english->character menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button5")); /* change to english->article menu */
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button6")); /* english->layout->choose */
  g_signal_connect (button, "clicked", G_CALLBACK (on_choose_press), english_window);

  kb_surface = cairo_image_surface_create_from_png(icon[ICON_KB_EN].path); /* english keyboard image */
  kb_width = cairo_image_surface_get_width(kb_surface);
  kb_height = cairo_image_surface_get_height(kb_surface);

  kb_draw = GTK_WIDGET (gtk_builder_get_object (builder, "kb_draw"));
  g_signal_connect (kb_draw, "expose-event", G_CALLBACK (on_expose), NULL);
  gtk_widget_set_size_request (kb_draw, kb_width, kb_height);

  g_signal_connect (english_window, "key-press-event", G_CALLBACK (on_key_press), NULL);

  /* choose layout class, assume them to be local variables first */
  enum {
    COL_CLASS_NAME,		/* string */
    NUM_CLASS_COLS,
  };
  struct _class {
    const gchar *name;
  };
  struct _class class[] = {
    {"Keyboard Layout"},
    {"Finger One by One"},
    {"Wrong?"},
    {"Class 0: asdfjkl;"},
    {"Class 1: ei"},
    {"Class 2: ru"},
    {"Class 3: gh"},
    {"Class 4: C,"},
    {"Class 5: yt"},
    {"Class 6: mv"},
    {"Class 7: bn"},
    {"Class 8: ow"},
    {"Class 9: pqz"},
    {"Class 10: x."},
    {"Class 11: 0-9"},
    {"Class 12: punctuation"},
  };
  choose_store = gtk_list_store_new (NUM_CLASS_COLS,
				     G_TYPE_STRING);
  for (i = 0; i < G_N_ELEMENTS (class); i++) {
    g_print ("%s\n", class[i].name);
    gtk_list_store_append (choose_store, &iter);
    gtk_list_store_set (choose_store, &iter,
			COL_CLASS_NAME, class[i].name,
			-1);
  }
  choose_treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (choose_store));
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (choose_treeview), TRUE);
  gtk_tree_view_set_search_column (GTK_TREE_VIEW (choose_treeview), COL_CLASS_NAME);
  g_object_unref (choose_store);
  renderer = gtk_cell_renderer_text_new ();
  col = gtk_tree_view_column_new_with_attributes ("class name",
						  renderer,
						  "text",
						  COL_CLASS_NAME,
						  NULL);
  gtk_tree_view_column_set_sort_column_id (col, COL_CLASS_NAME);
  gtk_tree_view_append_column (GTK_TREE_VIEW (choose_treeview), col);
}

void englishui_deinit()
{
  if (kb_surface) {
    cairo_surface_destroy(kb_surface);
  }
}
