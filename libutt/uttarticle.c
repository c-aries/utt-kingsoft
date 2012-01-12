#include <utt/article.h>
#include <gtk/gtkbindings.h>
#include <gtk/gtkprivate.h>
#include <pango/pangocairo.h>
#include <gconf/gconf-client.h>
#include <string.h>

#define DEBUG
#define DEFAULT_MONOSPACE_FONT "Monospace 10"

typedef struct {
  PangoFontDescription *font_desc;
  gchar *text;
  GArray *array;
  gint cur, total;
  void *data;
} UttArticlePrivate;

struct _pos {
  gdouble x, y;
  gchar word[4];
};

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
  UttArticlePrivate *priv;
  gchar *contents;
  gsize length;
  const gchar *font;

  g_return_val_if_fail (UTT_IS_ARTICLE (article), FALSE);

  priv = UTT_ARTICLE_GET_PRIVATE (article);

  if (g_file_get_contents (filename, &contents, &length, NULL)) {
    if (!g_utf8_validate (contents, length, NULL)) {
      g_free (contents);
      return FALSE;
    }
    if (priv->text) {
      g_free (priv->text);
    }
    priv->text = contents;
    gtk_widget_queue_draw (GTK_WIDGET (article));
  }

  return TRUE;
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
  UttArticle *article;
  UttArticlePrivate *priv;
  PangoLayout *layout;
  cairo_t *cairo;
  GdkColor color;
  gint utt_article_width, utt_article_height, len, width, height;
  gchar word[4], *p;
  gdouble rel_x, rel_y, height_4, height_2, width_1;
  gint border, end_x, end_y, num = 0;
  struct _pos pos;

#ifdef DEBUG
  g_debug (G_STRFUNC);
#endif
  g_return_val_if_fail (UTT_IS_ARTICLE (widget), FALSE);

  article = UTT_ARTICLE (widget);
  border = article->border;
  priv = UTT_ARTICLE_GET_PRIVATE (widget);
  if (gtk_widget_is_drawable (widget)) {
    utt_article_width = widget->allocation.width;
    utt_article_height = widget->allocation.height;
    end_x = utt_article_width - border;
    end_y = utt_article_height - border;

    cairo = gdk_cairo_create (widget->window);

    gdk_color_parse ("yellow", &color);
    gdk_cairo_set_source_color (cairo, &color);
    cairo_rectangle (cairo, border, border, utt_article_width - border, utt_article_height - border);
    cairo_fill (cairo);

    if (priv->text) {
      layout = pango_cairo_create_layout (cairo);
      pango_layout_set_font_description (layout, priv->font_desc);
      cairo_set_source_rgb (cairo, 0.0, 0.0, 0.0);
      p = priv->text;
      rel_x = rel_y = border;
      pango_layout_set_text (layout, "六", -1);
      pango_layout_get_size (layout, NULL, &height);
      height_2 = 2 * (gdouble)height / PANGO_SCALE;
      height_4 = 4 * (gdouble)height / PANGO_SCALE;
      do {
	g_utf8_strncpy (word, p, 1);
	if (!(len = strlen (word))) {
	  break;
	}
	p += len;
	pango_layout_set_text (layout, word, -1);
	pango_layout_get_size (layout, &width, NULL);	
	width_1 = (gdouble)width / PANGO_SCALE;
	if (len == 1 && word[0] == '\n') {
	  if (rel_y + height_4 > end_y) {
	    break;
	  }
	  rel_y += height_2;
	  rel_x = border;
	  continue;
	}
	if (rel_x + width_1 > end_x)  {
	  rel_x = border;
	  if (rel_y + height_4 > end_y) {
	    break;
	  }
	  rel_y += height_2;
	}
	cairo_move_to (cairo, rel_x, rel_y);
	pos.x = rel_x;
	pos.y = rel_y + (gdouble)height / PANGO_SCALE;
	g_utf8_strncpy (pos.word, word, 1);
	g_array_insert_val (priv->array, num, pos);
	num++;
	rel_x += width_1;
	pango_cairo_show_layout (cairo, layout);
      } while (len);
      g_object_unref (layout);
    }
    priv->total = num;
    cairo_destroy (cairo);
  }
  return FALSE;
}

static gint
utt_article_key_press (GtkWidget *widget,
		       GdkEventKey *event)
{
  UttArticlePrivate *priv;
  PangoLayout *layout;
  struct _pos *pos;
  cairo_t *cairo;
  GdkColor color;
  gchar word[4] = {};
  gunichar unicode;
  gint ret;

#ifdef DEBUG
  g_debug (G_STRFUNC);
  g_debug ("%08x", event->keyval);
#endif

  g_return_val_if_fail (UTT_IS_ARTICLE (widget), TRUE);
  priv = UTT_ARTICLE_GET_PRIVATE (widget);

  if (event->keyval != 0xff09) {
    unicode = gdk_keyval_to_unicode (event->keyval);
  }
  else {
    unicode = 0x09;
  }
  ret = g_unichar_to_utf8 (unicode, word);
  word[ret] = '\0';
#if 0
  g_print ("%x %x, %x %x %x %x, %d\n", event->keyval, unicode, word[0], word[1], word[2], word[3], ret);
#endif
  unicode = g_utf8_get_char (word);

  if (priv->text && gtk_widget_is_drawable (widget) &&
      (g_unichar_isprint (unicode) || unicode == 0x09)) {

    cairo = gdk_cairo_create (widget->window);
    layout = pango_cairo_create_layout (cairo);
    pango_layout_set_font_description (layout, priv->font_desc);

    gdk_color_parse ("blue", &color);
    gdk_cairo_set_source_color (cairo, &color);
    pango_layout_set_text (layout, word, -1);

    pos = &g_array_index (priv->array ,struct _pos , priv->cur);
    cairo_move_to (cairo, pos->x, pos->y);
    priv->cur++;
    if (priv->cur == priv->total) {
      g_debug ("next page!");
    }

    pango_cairo_show_layout (cairo, layout);
    g_object_unref (layout);
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
utt_article_finalize (GObject *object)
{
  UttArticlePrivate *priv;

  g_return_if_fail (UTT_IS_ARTICLE (object));

  g_debug (G_STRFUNC);
  priv = UTT_ARTICLE_GET_PRIVATE (object);
  if (priv->text) {
    g_free (priv->text);
    priv->text = NULL;
  }
  if (priv->font_desc) {
    pango_font_description_free (priv->font_desc);
    priv->font_desc = NULL;
  }
  if (priv->array) {
    g_array_free (priv->array, TRUE);
  }

  G_OBJECT_CLASS (utt_article_parent_class)->finalize (object);
}

static void
utt_article_class_init (UttArticleClass *class)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS(class);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);
/*   GtkBindingSet *binding_set; */

  gobject_class->get_property = utt_article_get_property;
  gobject_class->set_property = utt_article_set_property;
  gobject_class->finalize = utt_article_finalize;
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
  GConfClient *conf;
  GConfValue *value;
  gchar *font;

  gtk_widget_set_has_window (GTK_WIDGET (article), TRUE);
  gtk_widget_set_can_focus (GTK_WIDGET (article), TRUE);

  article->border = 2;

  priv = UTT_ARTICLE_GET_PRIVATE (article);
  priv->text = NULL;
  priv->data = NULL;
  priv->font_desc = NULL;

  conf = gconf_client_get_default ();
  value = gconf_client_get (conf, "/desktop/gnome/interface/monospace_font_name", NULL);
  if (value && value->type == GCONF_VALUE_STRING) {
    font = gconf_value_get_string (value);
  }
  if (!font) {
    font = DEFAULT_MONOSPACE_FONT;
  }
  priv->font_desc = pango_font_description_from_string (font);
  pango_font_description_set_weight (priv->font_desc, PANGO_WEIGHT_BOLD);
  pango_font_description_set_absolute_size (priv->font_desc, 16 * PANGO_SCALE);
  g_object_unref (conf);

  priv->array = g_array_new (FALSE, TRUE, sizeof (struct _pos));
  priv->cur = 0;
}

GtkWidget *
utt_article_new ()
{
  UttArticle *article;

  g_debug (G_STRFUNC);

  article = g_object_new (UTT_TYPE_ARTICLE,
			  NULL);
  return GTK_WIDGET(article);
}
