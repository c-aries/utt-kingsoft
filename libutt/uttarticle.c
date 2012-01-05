#include <utt/article.h>
#include <gtk/gtkbindings.h>
#include <gtk/gtkprivate.h>
#include <gconf/gconf-client.h>
#include <string.h>

#define DEBUG

typedef struct {
  gchar *text;
  void *data;
} UttArticlePrivate;

enum {
  PROP_0,
  PROP_BORDER,
};

#define UTT_ARTICLE_GET_PRIVATE(obj)	G_TYPE_INSTANCE_GET_PRIVATE((obj), UTT_TYPE_ARTICLE, UttArticlePrivate)

static void utt_article_get_property (GObject *object,
				      guint prop_id,
				      GValue *value,
				      GParamSpec *pspec);
static void utt_article_set_property (GObject *object,
				      guint prop_id,
				      const GValue *value,
				      GParamSpec *pspec);

G_DEFINE_TYPE(UttArticle, utt_article, GTK_TYPE_WIDGET);

void
utt_article_close_file (UttArticle *article)
{
  UttArticlePrivate *priv;

  g_return_if_fail (UTT_IS_ARTICLE (article));

  priv = UTT_ARTICLE_GET_PRIVATE (article);
  if (priv->text) {
    g_free (priv->text);
    priv->text = NULL;
  }
}

gboolean
utt_article_open_file (UttArticle *article, gchar *filename)
{
#define DEFAULT_MONOSPACE_FONT "Monospace 10"
  PangoFontDescription *font_desc;
  UttArticlePrivate *priv;
  gchar *contents;
  gsize length;
  GConfClient *conf;
  GConfValue *value;
  const gchar *font;

  g_return_val_if_fail (UTT_IS_ARTICLE (article), FALSE);

  priv = UTT_ARTICLE_GET_PRIVATE (article);
  if (priv->text) {
    g_free (priv->text);
    priv->text = NULL;
  }

  if (g_file_get_contents (filename, &contents, &length, NULL)) {
    priv->text = contents;
    gtk_widget_queue_draw (GTK_WIDGET (article));
  }

  conf = gconf_client_get_default ();
  value = gconf_client_get (conf, "/desktop/gnome/interface/monospace_font_name", NULL);
  if (value && value->type == GCONF_VALUE_STRING) {
    font = gconf_value_get_string (value);
    if (!font) {
      font = DEFAULT_MONOSPACE_FONT;
    }
    g_print ("%s\n", font);
    font_desc = pango_font_description_from_string (font);
    if (font_desc) {
      pango_font_description_free (font_desc);
      g_print ("%s\n", pango_font_description_get_family (font_desc));
    }
  }

  return TRUE;
#undef DEFAULT_MONOSPACE_FONT
}

/* when gtk_widget_set_has_window () set TRUE, should implement this function */
/* learn from gtkeventbox.c */
static void
utt_article_realize (GtkWidget *widget)
{
  GdkWindowAttr attr;
  gboolean has_window;
  gint attr_mask;

  gtk_widget_set_realized (widget, TRUE);

  attr.x = widget->allocation.x;
  attr.y = widget->allocation.y;
  attr.width = widget->allocation.width;
  attr.height = widget->allocation.height;
  attr.window_type = GDK_WINDOW_CHILD;
  attr.event_mask = gtk_widget_get_events (widget)
    | GDK_BUTTON_MOTION_MASK
    | GDK_BUTTON_PRESS_MASK
    | GDK_BUTTON_RELEASE_MASK
    | GDK_EXPOSURE_MASK
    | GDK_ENTER_NOTIFY_MASK
    | GDK_LEAVE_NOTIFY_MASK
    | GDK_KEY_PRESS_MASK
    | GDK_KEY_RELEASE_MASK;

  has_window = gtk_widget_get_has_window (widget);
  if (has_window) {
    attr.visual = gtk_widget_get_visual (widget);
    attr.colormap = gtk_widget_get_colormap (widget);
    attr.wclass = GDK_INPUT_OUTPUT;
    attr_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
    widget->window = gdk_window_new (gtk_widget_get_parent_window (widget),
				     &attr, attr_mask);
    gdk_window_set_user_data (widget->window, widget);
  }
  else {
    g_error("has no window");
  }

  widget->style = gtk_style_attach (widget->style, widget->window);
  if (has_window) {
    gtk_style_set_background (widget->style, widget->window, GTK_STATE_NORMAL);
  }
}

static void
utt_article_unrealize (GtkWidget *widget)
{
  GTK_WIDGET_CLASS(utt_article_parent_class)->unrealize (widget);
}

static void
utt_article_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
  requisition->width = requisition->height = 0;
}

static void
utt_article_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
  widget->allocation = *allocation;
  if (gtk_widget_get_realized (widget)) {
    gdk_window_move_resize (widget->window,
			    allocation->x,
			    allocation->y,
			    allocation->width,
			    allocation->height);
  }
}

static void
utt_article_map (GtkWidget *widget)
{
  gtk_widget_set_mapped (widget, TRUE);
  if (gtk_widget_get_has_window (widget)) {
    gdk_window_show (widget->window);
  }
}

static void
utt_article_unmap (GtkWidget *widget)
{
  GTK_WIDGET_CLASS(utt_article_parent_class)->unmap (widget);
}

static gboolean
utt_article_expose (GtkWidget *widget, GdkEventExpose *event)
{
  UttArticlePrivate *priv;
  cairo_t *cairo;
  GdkColor color;
  cairo_font_extents_t fe;
  cairo_text_extents_t te;
  gint utt_article_width, utt_article_height, len;
  gchar word[4], *p;
  gint rel_x = 20;

  g_return_val_if_fail (UTT_IS_ARTICLE (widget), FALSE);
#ifdef DEBUG
  g_debug (G_STRFUNC);
#endif

  priv = UTT_ARTICLE_GET_PRIVATE (widget);
  if (gtk_widget_is_drawable (widget)) {
    utt_article_width = widget->allocation.width;
    utt_article_height = widget->allocation.height;

    cairo = gdk_cairo_create (widget->window);

    gdk_color_parse ("yellow", &color);
    gdk_cairo_set_source_color (cairo, &color);
    cairo_rectangle (cairo, 6, 6, utt_article_width - 12, utt_article_height - 12);
    cairo_fill (cairo);

    if (priv->text) {
      gdk_color_parse ("black", &color);
      gdk_cairo_set_source_color (cairo, &color);
      cairo_set_line_width (cairo, 16.0);
      cairo_set_font_size (cairo, 16.0);
      cairo_select_font_face (cairo, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
      cairo_font_extents (cairo, &fe);
      p = priv->text;
      
      do {
	g_utf8_strncpy (word, p, 1);
	if (!(len = strlen (word))) {
	  break;
	}
	p += len;
	cairo_text_extents (cairo, word, &te);
	cairo_move_to (cairo, rel_x, 40);
	cairo_show_text (cairo, word);
	rel_x += te.x_advance;
	g_print ("advance %lf\n", te.x_advance);
      } while (len);
    }

    cairo_destroy (cairo);
  }
  return FALSE;
}

static gint
utt_article_key_press (GtkWidget *widget,
		       GdkEventKey *event)
{
#ifdef DEBUG
  g_debug (G_STRFUNC);
  g_debug ("%08x", event->keyval);
#endif

  if (gtk_widget_is_drawable (widget)) {
    cairo_t *cairo;
    GdkColor color;
    gint width = widget->allocation.width;
    gint height = widget->allocation.height;
    gchar word[2] = {};
    gchar oops[4] = {0xe8, 0xb6, 0x8a};
    cairo_font_extents_t fe;
    cairo_text_extents_t te;
    static gint rel_x = 20;
    static gint oops_flag = FALSE;

    word[0] = event->keyval;

    cairo = gdk_cairo_create (widget->window);
    gdk_color_parse ("black", &color);
    gdk_cairo_set_source_color (cairo, &color);
    cairo_set_line_width (cairo, 16.0);
    cairo_set_font_size (cairo, 16.0);
    cairo_select_font_face (cairo, "Georgia", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_font_extents (cairo, &fe);
    cairo_text_extents (cairo, word, &te);
#ifdef DEBUG
    g_debug ("%lf, %lf, %lf", te.width, te.x_bearing, te.x_advance);
#endif
    cairo_move_to (cairo, rel_x, 40);
    cairo_show_text (cairo, word);
#if 1
    rel_x += te.x_advance;
#else
    rel_x += te.x_advance + (te.x_bearing < 0 ? -te.x_bearing : 0);
#endif
    if (te.x_bearing < 0) {
#ifdef DEBUG
      g_debug ("xbear %lf", te.x_bearing);
#endif
    }

    if (!oops_flag && rel_x > 100) {
      cairo_text_extents (cairo, oops, &te);
      cairo_move_to (cairo, rel_x, 40);
      cairo_show_text (cairo, oops);
#if 1
      rel_x += te.x_advance;
#else
      rel_x += te.x_advance + (te.x_bearing < 0 ? -te.x_bearing : 0);
#endif
      oops_flag = TRUE;
    }

#ifdef DEBUG
    g_debug ("%d, %d", width, height);
#endif

    cairo_destroy (cairo);
  }

  if (GTK_WIDGET_CLASS (utt_article_parent_class)->key_press_event (widget, event)) {
    return TRUE;
  }
  return FALSE;
}

static gint
utt_article_key_release (GtkWidget *widget,
			 GdkEventKey *event)
{
  return GTK_WIDGET_CLASS(utt_article_parent_class)->key_release_event (widget, event);
}

static void
utt_article_grab_focus (GtkWidget *widget)
{
#ifdef DEBUG
  /* c-aries */
  g_debug (G_STRFUNC);
#endif

  GTK_WIDGET_CLASS (utt_article_parent_class)->grab_focus (widget);
}

static gint
utt_article_focus_in (GtkWidget *widget,
		      GdkEventFocus *event)
{
#ifdef DEBUG
  /* c-aries */
  g_debug (G_STRFUNC);
#endif

  gtk_widget_queue_draw (widget);
  return FALSE;
}

static gint
utt_article_focus_out (GtkWidget *widget,
		       GdkEventFocus *event)
{
#ifdef DEBUG
  /* c-aries */
  g_debug (G_STRFUNC);
#endif

  gtk_widget_queue_draw (widget);
  return FALSE;
}

static void
utt_article_get_property (GObject *object,
			  guint prop_id,
			  GValue *value,
			  GParamSpec *pspec)
{
  UttArticle *article = UTT_ARTICLE (object);

  switch (prop_id) {
  case PROP_BORDER:
    g_value_set_uint (value, article->border);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
utt_article_set_property (GObject *object,
			  guint prop_id,
			  const GValue *value,
			  GParamSpec *pspec)
{
  UttArticle *article = UTT_ARTICLE (object);

  switch (prop_id) {
  case PROP_BORDER:
    article->border = g_value_get_uint (value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
utt_article_class_init (UttArticleClass *class)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(class);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);
/*   GtkBindingSet *binding_set; */

  gobject_class->get_property = utt_article_get_property;
  gobject_class->set_property = utt_article_set_property;
  g_object_class_install_property (gobject_class,
				   PROP_BORDER,
				   g_param_spec_uint ("border",
						      "Border",
						      "The border of article widget",
						      1,
						      65535,
						      2,
						      GTK_PARAM_READWRITE));

  widget_class->realize = utt_article_realize;
  widget_class->unrealize = utt_article_unrealize;
  widget_class->size_request = utt_article_size_request;
  widget_class->size_allocate = utt_article_size_allocate;
  widget_class->map = utt_article_map;
  widget_class->unmap = utt_article_unmap;
  widget_class->expose_event = utt_article_expose;
  widget_class->key_press_event = utt_article_key_press; /* set gtk_widget_set_can_focus in utt_article_init, learn from gtk_entry_init */
  widget_class->key_release_event = utt_article_key_release;
  widget_class->grab_focus = utt_article_grab_focus;
  widget_class->focus_in_event = utt_article_focus_in;
  widget_class->focus_out_event = utt_article_focus_out;

/*   binding_set = gtk_binding_set_by_class (class); */

  g_type_class_add_private(class, sizeof(UttArticlePrivate));
}

static void
utt_article_init (UttArticle *article)
{
  UttArticlePrivate *priv;

  gtk_widget_set_has_window (GTK_WIDGET (article), TRUE);
  gtk_widget_set_can_focus (GTK_WIDGET (article), TRUE);

  article->border = 2;

  priv = UTT_ARTICLE_GET_PRIVATE (article);
  priv->text = NULL;
  priv->data = NULL;
}

GtkWidget *
utt_article_new ()
{
  UttArticle *article;

  article = g_object_new (UTT_TYPE_ARTICLE,
			  NULL);
  return GTK_WIDGET(article);
}
