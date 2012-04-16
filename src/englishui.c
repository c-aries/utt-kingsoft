#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gprintf.h>
#include "global.h"		/* icon */
#include "data.h"		/* ICON_KB_EN */
#include "main.h"

#define DEBUG 1

static gint current_pageid;	      /* current notebook page num */
static cairo_surface_t *kb_surface;	/* keyboard surface */
static GdkPixbuf *kb_pixbuf;		/* keyboard pixbuf */
static GtkWidget *dashboard;		/* dashboard, timeout need to update it */
static cairo_surface_t *dash_surface;	/* dashboard surface */
/* static gint key_index = 0; */
static GtkWidget *key_draw[6];	/* english ui, basic keyboard layout class */
static gint keydraw_index = 0;	/* blue keydraw index */
static GtkWidget *choose_dialog;
static GtkWidget *choose_treeview;
static GtkWidget *layout_label;
static gint class_index = 0;

static gchar *text = "asdfg";
static gchar gentext[7];	/* the last character is NUL */

static guint elapse;
static guint timeout_id;
static gboolean class_begin_flag = FALSE;

struct _stat {		     /* statistics system */
  guint right;			/* right characaters */
  guint sum;			/* sum of characters you have input */
  guint total;			/* totoal of characters the class have */
  guint elapse;			/* time elapse */
};
/* speed = right units / minutes */
/* percent to finish = right units / class units */
/* correntness = right units / total units */

struct _class {
  const gchar *name;
};
static struct _class class[] = {
  {"Random"},
  {"asdfg"},
  {"Keyboard Layout"},
  {"Finger One by One"},
  {"Wrong?"},
  {"Test 'a', 's'"},
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
on_timeout (gpointer data)
{
  elapse++;
/*   g_print ("%u\n", elapse); */
  gtk_widget_queue_draw (dashboard);
  return TRUE;
}

static gboolean
on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  gpointer ret, found;
  gint i, texti, keyi;
  gchar ch = gentext[keydraw_index];

  g_print ("key press %08x\n", event->keyval);
  if (event->keyval == GDK_Pause) { /* deal with pause key */
    if (timeout_id) {
      g_source_remove (timeout_id);
      timeout_id = 0;
    }
    else {
      timeout_id = g_timeout_add_seconds (1, on_timeout, NULL);
    }
    return FALSE;
  }
  ret = g_hash_table_lookup (key_val_ht, GUINT_TO_POINTER (event->keyval));
  if (ret) {
    keyi = GPOINTER_TO_INT (ret);
    g_print ("key %s\n", key[keyi].name);
    if (!class_begin_flag) {
      class_begin_flag = TRUE;
      if (!timeout_id) {
	timeout_id = g_timeout_add_seconds (1, on_timeout, NULL);
      }
    }
    if (ch && key[keyi].ch != ch) {
      return FALSE;
    }
    keydraw_index = (keydraw_index + 1) % 6;
    if (keydraw_index == 0) {	/* next turn */
      g_print ("gen text\n");
      for (i = 0; i < 6; i++) {	/* i is gentext index */
	texti = g_random_int_range (0, 5);
	found = g_hash_table_lookup (key_char_ht, GUINT_TO_POINTER ((guint)text[texti]));
	if (found) {
	  keyi = GPOINTER_TO_INT (found);
	  g_print ("%d %c, %d, %s\n", texti, text[texti], keyi, key[keyi].name);
	  gentext[i] = text[texti];
	}
	else {
	  g_print ("not found\n");
	}
      }
      g_print ("gentext %s\n", gentext);
    }
    gtk_widget_queue_draw (widget);
  }
  return FALSE;
}

static gint
on_keyboard_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  cairo_t *cr;
  struct _key *keyp;
  gchar ch = gentext[keydraw_index];
  gpointer found;
  gint keyi;

  cr = gdk_cairo_create (event->window);
  cairo_set_source_surface (cr, kb_surface, 0, 0);
  cairo_paint (cr);

  if (ch) {
    /* FIXME: we can cache here */
    found = g_hash_table_lookup (key_char_ht, GUINT_TO_POINTER ((guint)ch));
    if (found) {
      keyi = GPOINTER_TO_INT (found);
      keyp = &key[keyi];
      cairo_set_line_width (cr, 1);
      cairo_set_source_rgba (cr, 0, 0, 1, 0.3);
      cairo_rectangle (cr, keyp->x, keyp->y, keyp->width, keyp->height);
      cairo_fill (cr);
    }
  }

  cairo_destroy (cr);
  return FALSE;
}

static gint
on_dashboard_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  cairo_t *cr;
  cairo_text_extents_t te;
  gchar timestamp[9];		/* format:"00:00:00" */
  gint sec, min, hour, min_left;

  sec = elapse % 60;
  min_left = elapse / 60;
  min = min_left % 60;
  hour = min_left / 60;
  g_sprintf (timestamp, "%02d:%02d:%02d", hour, min, sec);
  g_print ("%s\n", timestamp);

  cr = gdk_cairo_create (event->window);
  cairo_set_source_surface (cr, dash_surface, 0, 0);
  cairo_paint (cr);

  /* FIXME: maybe we can cache here */
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_select_font_face (cr, "Monospace",
			  CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size (cr, 18.0);
  cairo_text_extents (cr, timestamp, &te);
  g_print ("width %lf, height %lf, x %lf, y %lf\n", te.width, te.height, te.x_bearing, te.y_bearing);
#define DEBUG_X 80
#define DEBUG_Y 43
  cairo_move_to (cr, DEBUG_X, DEBUG_Y);
  cairo_show_text (cr, timestamp);
#if 0
  cairo_set_source_rgb (cr, 1, 0, 0);
  cairo_arc (cr, DEBUG_X, DEBUG_Y, 2, 0, 2 * G_PI);
  cairo_fill (cr);
#endif
#undef DEBUG_X
#undef DEBUG_Y

  cairo_destroy (cr);
  return TRUE;
}

gboolean
on_englishui_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  gtk_widget_hide_all(current_window);
  current_window = global.english_window;
  gtk_widget_show_all(current_window);
  /* GtkNotebook hide, we should set page by ourself after show */
  gtk_notebook_set_current_page(GTK_NOTEBOOK(global.english_notebook), current_pageid);
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
    class_index = choose = gtk_tree_path_get_indices (path)[0];
    gtk_label_set_text (GTK_LABEL (layout_label), class[choose].name);
    g_print ("choose index %d '%s'\n", choose, class[choose].name);
  }
  if (class_index == 1) {	/* "asdfg" */
  }
  gtk_widget_hide_all (choose_dialog);
/*   gtk_widget_grab_focus (title_label); /\* focus title label, if not, when you press enter, it will press choose button again *\/ */
  return FALSE;
}

static gboolean
on_keydraw_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  GdkPixbuf *dest = NULL;
  cairo_t *cr;
  gint index = GPOINTER_TO_INT (data); /* current common keydraw index */
  struct _key *keyp = NULL;
  gint keyi;
  gpointer found;
  gchar ch;

  ch = gentext[index];
  if (ch) {
    /* FIXME: we can cache here */
    found = g_hash_table_lookup (key_char_ht, GUINT_TO_POINTER ((guint)ch));
    if (found) {
      keyi = GPOINTER_TO_INT (found);
      keyp = &key[keyi];
    }
  }

  if (keyp) {
    dest = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, 8, keyp->width, keyp->height);
    gdk_pixbuf_copy_area (kb_pixbuf, keyp->x, keyp->y, keyp->width, keyp->height,
			  dest, 0, 0); /* got it from mei meng maid cafe */
  }

  cr = gdk_cairo_create (event->window);
  if (keyp) {
    gdk_cairo_set_source_pixbuf (cr, dest, 0, 0);
  }
  cairo_paint (cr);
  if (key_draw[keydraw_index] == widget) {
    cairo_set_source_rgba (cr, 0, 0, 1, 0.3);
    cairo_paint (cr);
  }

  cairo_destroy (cr);
  if (keyp) {
    g_object_unref (dest);
  }
  return FALSE;
}

static gboolean
on_english_window_focus_in (GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
  g_print ("%s\n", __func__);
  if (class_begin_flag && !timeout_id) {
    timeout_id = g_timeout_add_seconds (1, on_timeout, NULL);
  }
  return TRUE;
}

static gboolean
on_english_window_focus_out (GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
  g_print ("%s\n", __func__);
  if (class_begin_flag && timeout_id) {
    g_source_remove (timeout_id);
    timeout_id = 0;
  }
  return TRUE;
}

static void
on_note_switch (GtkNotebook *note, GtkNotebookPage *page, guint page_num, gpointer data)
{
  g_print ("%s: index %d\n", __func__, page_num);
}

static void
on_note_hide (GtkWidget *widget, gpointer data)
{
  g_print ("%s\n", __func__);
}

static void
on_note_pageadd (GtkNotebook *notebook, GtkWidget *child, guint page_num, gpointer user_data)
{
  g_print ("%s: pageadd %d\n", __func__, page_num);
}

static gboolean
englishui_on_menu_press (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  current_pageid = gtk_notebook_get_current_page (GTK_NOTEBOOK (global.english_notebook));
  gtk_widget_hide_all (current_window);
  current_window = global.menu_window;
  gtk_widget_show_all (current_window);
  return FALSE;
}

void
englishui_init (GtkBuilder *builder)			/* english ui init */
{
  GtkWidget *english_window;
  gint kb_width, kb_height, dash_width, dash_height, i;
  GtkWidget *button;
  GtkWidget *kb_draw;
  GtkNotebook *notebook;
  /* choose list widgets */
  GtkListStore *choose_store;
  GtkTreeIter iter;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *col;
  gchar *tempstr;
  /* gentext */
  gint texti, keyi;
  gpointer found;

  english_window = global.english_window = GTK_WIDGET (gtk_builder_get_object (builder, "english_window"));
  g_signal_connect (english_window, "focus-out-event", G_CALLBACK (on_english_window_focus_out), NULL);
  g_signal_connect (english_window, "focus-in-event", G_CALLBACK (on_english_window_focus_in), NULL);

  /* title label */
  /* menu button */
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button1")); /* change to english->layout menu */
  g_signal_connect (button, "clicked", G_CALLBACK (englishui_on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button2")); /* change to english->character menu */
  g_signal_connect (button, "clicked", G_CALLBACK (englishui_on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button5")); /* change to english->article menu */
  g_signal_connect (button, "clicked", G_CALLBACK (englishui_on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button6")); /* english->layout->choose */
  g_signal_connect (button, "clicked", G_CALLBACK (on_choose_press), english_window);
  /* choose dialog */
  choose_dialog = GTK_WIDGET (gtk_builder_get_object (builder, "choose_dialog1")); /* english->layout->choose dialog */
  gtk_widget_set_parent_window (choose_dialog, gtk_widget_get_window (english_window)); /* FIXME: I don't know how to set parent window in glade. */
  gtk_window_set_transient_for (GTK_WINDOW (choose_dialog), GTK_WINDOW (english_window));
  choose_store = GTK_LIST_STORE (gtk_builder_get_object (builder, "liststore1"));
  choose_treeview = GTK_WIDGET (gtk_builder_get_object (builder, "treeview2"));
  notebook = GTK_NOTEBOOK (gtk_builder_get_object (builder, "english_notebook"));
  g_signal_connect (notebook, "page-added", G_CALLBACK (on_note_pageadd), NULL);
  g_signal_connect (notebook, "switch-page", G_CALLBACK (on_note_switch), NULL);
  g_signal_connect_after (notebook, "hide", G_CALLBACK (on_note_hide), NULL);
  /* others */
  layout_label = GTK_WIDGET (gtk_builder_get_object (builder, "label6"));
  gtk_label_set_text (GTK_LABEL (layout_label), class[0].name);

  kb_pixbuf = gdk_pixbuf_new_from_file (icon[ICON_KB_EN].path, NULL);	/* english keyboard image */
  kb_surface = cairo_image_surface_create_from_png (icon[ICON_KB_EN].path);
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

  /* keys */
  for (i = 0; i < 6; i++) {
    tempstr = g_strdup_printf ("key_draw%d", i + 1);
    key_draw[i] = GTK_WIDGET (gtk_builder_get_object (builder, tempstr));
    g_free (tempstr);
    gtk_widget_set_size_request (key_draw[i], key[1].width, key[1].height);
    g_signal_connect (key_draw[i], "expose-event", G_CALLBACK (on_keydraw_expose), GINT_TO_POINTER (i));
  }

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
/*   g_object_unref (choose_store); */
  renderer = gtk_cell_renderer_text_new ();
  col = gtk_tree_view_column_new_with_attributes ("class name",
						  renderer,
						  "text",
						  COL_CLASS_NAME,
						  NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (choose_treeview), col);

  /* initialize gentext */
  for (i = 0; i < 6; i++) {	/* i is gentext index */
    texti = g_random_int_range (0, 5);
    found = g_hash_table_lookup (key_char_ht, GUINT_TO_POINTER ((guint)text[texti]));
    if (found) {
      keyi = GPOINTER_TO_INT (found);
      g_print ("%d %c, %d, %s\n", texti, text[texti], keyi, key[keyi].name);
      gentext[i] = text[texti];
    }
    else {
      g_print ("not found\n");
    }
  }
}

void englishui_deinit()
{
  if (kb_surface) {
    cairo_surface_destroy (kb_surface);
  }
  if (kb_pixbuf) {
    g_object_unref (kb_pixbuf);
  }
  if (timeout_id) {
    g_source_remove (timeout_id);
    timeout_id = 0;
    elapse = 0;			/* end of the session */
    class_begin_flag = FALSE;
  }
}
