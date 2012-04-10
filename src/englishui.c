#include <gtk/gtk.h>
#include "global.h"		/* icon */
#include "data.h"		/* ICON_KB_EN */
#include "main.h"

#define DEBUG 1

static cairo_surface_t *kb_surface;	/* keyboard surface */
static cairo_surface_t *dash_surface;	/* dashboard surface */
static gint key_index = 0;
static GtkWidget *choose_dialog;
static GtkWidget *choose_treeview;
static GtkWidget *layout_label;

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
on_keyboard_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
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

static gint
on_dashboard_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  cairo_t *cr;

  cr = gdk_cairo_create (event->window);
  cairo_set_source_surface (cr, dash_surface, 0, 0);
  cairo_paint (cr);
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
  GtkWidget *content;
  GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW (choose_treeview));
  GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (choose_treeview));
  GtkTreeIter iter;
  GtkTreePath *path;
  gint ret, choose;

  if (!gtk_tree_selection_get_selected (sel, NULL, &iter)) {
    gtk_tree_model_get_iter_first (model, &iter);
    gtk_tree_selection_select_iter (sel, &iter);
  }
  content = gtk_dialog_get_content_area (GTK_DIALOG (choose_dialog));
  gtk_widget_show_all (choose_dialog);
  ret = gtk_dialog_run (GTK_DIALOG (choose_dialog)); /* ret GTK_RESPONSE_OK -5, GTK_RESPONSE_CANCEL -6, GTK_RESPONSE_DELETE_EVENT -4 */
  if (ret == GTK_RESPONSE_OK) {
    gtk_tree_selection_get_selected (sel, NULL, &iter);
    path = gtk_tree_model_get_path (model, &iter);
    choose = gtk_tree_path_get_indices (path)[0];
    gtk_label_set_text (GTK_LABEL (layout_label), class[choose].name);
    g_print ("choose index %d '%s'\n", choose, class[choose].name);
  }
  gtk_widget_hide_all (choose_dialog);
  return FALSE;
}

void
englishui_init (GtkBuilder *builder)			/* english ui init */
{
  GtkWidget *english_window;
  gint kb_width, kb_height, dash_width, dash_height, i;
  GtkWidget *button;
  GtkWidget *kb_draw, *dashboard;
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
  /* choose dialog */
  choose_dialog = GTK_WIDGET (gtk_builder_get_object (builder, "choose_dialog1")); /* english->layout->choose dialog */
  gtk_widget_set_parent_window (choose_dialog, gtk_widget_get_window (english_window)); /* FIXME: I don't know how to set parent window in glade. */
  gtk_window_set_transient_for (GTK_WINDOW (choose_dialog), GTK_WINDOW (english_window));
  choose_store = GTK_LIST_STORE (gtk_builder_get_object (builder, "liststore1"));
  choose_treeview = GTK_WIDGET (gtk_builder_get_object (builder, "treeview2"));
  /* others */
  layout_label = GTK_WIDGET (gtk_builder_get_object (builder, "label6"));
  gtk_label_set_text (GTK_LABEL (layout_label), class[0].name);

  kb_surface = cairo_image_surface_create_from_png (icon[ICON_KB_EN].path); /* english keyboard image */
  kb_width = cairo_image_surface_get_width (kb_surface);
  kb_height = cairo_image_surface_get_height (kb_surface);
  kb_draw = GTK_WIDGET (gtk_builder_get_object (builder, "kb_draw1"));
  g_signal_connect (kb_draw, "expose-event", G_CALLBACK (on_keyboard_expose), NULL);
  gtk_widget_set_size_request (kb_draw, kb_width, kb_height);

  dash_surface = cairo_image_surface_create_from_png (icon[ICON_DASHBOARD].path);
  dash_width = cairo_image_surface_get_width (dash_surface);
  dash_height = cairo_image_surface_get_height (dash_surface);
  dashboard = GTK_WIDGET (gtk_builder_get_object (builder, "dashboard1"));
  g_signal_connect (dashboard, "expose-event", G_CALLBACK (on_dashboard_expose), NULL);
  gtk_widget_set_size_request (dashboard, dash_width, dash_height);

  g_signal_connect (english_window, "key-press-event", G_CALLBACK (on_key_press), NULL);

  /* choose layout class, assume them to be local variables first */
  enum {
    COL_CLASS_NAME,		/* string */
    NUM_CLASS_COLS,
  };
  for (i = 0; i < G_N_ELEMENTS (class); i++) {
    gtk_list_store_append (choose_store, &iter);
    gtk_list_store_set (choose_store, &iter,
			COL_CLASS_NAME, class[i].name,
			-1);
  }
  gtk_tree_view_set_search_column (GTK_TREE_VIEW (choose_treeview), COL_CLASS_NAME);
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (choose_treeview), FALSE);
  g_object_unref (choose_store);
  renderer = gtk_cell_renderer_text_new ();
  col = gtk_tree_view_column_new_with_attributes ("class name",
						  renderer,
						  "text",
						  COL_CLASS_NAME,
						  NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (choose_treeview), col);
}

void englishui_deinit()
{
  if (kb_surface) {
    cairo_surface_destroy(kb_surface);
  }
}
