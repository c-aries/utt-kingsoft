#include <utt/artical.h>
#include <gtk/gtkbindings.h>

#define DEBUG

typedef struct {
  void *data;
} UttArticalPrivate;

#define UTT_ARTICAL_GET_PRIVATE(obj)	G_TYPE_INSTANCE_GET_PRIVATE((obj), UTT_TYPE_ARTICAL, UttArticalPrivate)

G_DEFINE_TYPE(UttArtical, utt_artical, GTK_TYPE_WIDGET);

/* when gtk_widget_set_has_window () set TRUE, should implement this function */
/* learn from gtkeventbox.c */
static void
utt_artical_realize (GtkWidget *widget)
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
utt_artical_unrealize (GtkWidget *widget)
{
  GTK_WIDGET_CLASS(utt_artical_parent_class)->unrealize (widget);
}

static void
utt_artical_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
  requisition->width = requisition->height = 0;
}

static void
utt_artical_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
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
utt_artical_map (GtkWidget *widget)
{
  gtk_widget_set_mapped (widget, TRUE);
  if (gtk_widget_get_has_window (widget)) {
    gdk_window_show (widget->window);
  }
}

static void
utt_artical_unmap (GtkWidget *widget)
{
  GTK_WIDGET_CLASS(utt_artical_parent_class)->unmap (widget);
}

static gboolean
utt_artical_expose (GtkWidget *widget, GdkEventExpose *event)
{
  cairo_t *cairo;
  GdkColor color;
  gint utt_artical_width, utt_artical_height;

  if (gtk_widget_is_drawable (widget)) {
    utt_artical_width = widget->allocation.width;
    utt_artical_height = widget->allocation.height;

    cairo = gdk_cairo_create (widget->window);

    gdk_color_parse ("yellow", &color);
    gdk_cairo_set_source_color (cairo, &color);
    cairo_rectangle (cairo, 6, 6, utt_artical_width - 12, utt_artical_height - 12);
    cairo_fill (cairo);

    cairo_destroy (cairo);
  }
  return FALSE;
}

static gint
utt_artical_key_press (GtkWidget *widget,
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
    rel_x += te.x_advance + (te.x_bearing < 0 ? -te.x_bearing : 0);
    if (te.x_bearing < 0) {
#ifdef DEBUG
      g_debug ("xbear %lf", te.x_bearing);
#endif
    }

    if (!oops_flag && rel_x > 100) {
      cairo_text_extents (cairo, oops, &te);
      cairo_move_to (cairo, rel_x, 40);
      cairo_show_text (cairo, oops);
      rel_x += te.x_advance + (te.x_bearing < 0 ? -te.x_bearing : 0);
      oops_flag = TRUE;
    }

#ifdef DEBUG
    g_debug ("%d, %d", width, height);
#endif

    cairo_destroy (cairo);
  }

  if (GTK_WIDGET_CLASS (utt_artical_parent_class)->key_press_event (widget, event)) {
    return TRUE;
  }
  return FALSE;
}

static gint
utt_artical_key_release (GtkWidget *widget,
			 GdkEventKey *event)
{
  return GTK_WIDGET_CLASS(utt_artical_parent_class)->key_release_event (widget, event);
}

static void
utt_artical_grab_focus (GtkWidget *widget)
{
#ifdef DEBUG
  /* c-aries */
  g_debug (G_STRFUNC);
#endif

  GTK_WIDGET_CLASS (utt_artical_parent_class)->grab_focus (widget);
}

static gint
utt_artical_focus_in (GtkWidget *widget,
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
utt_artical_focus_out (GtkWidget *widget,
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
utt_artical_class_init (UttArticalClass *class)
{
/*   GObjectClass *gobject_class = G_OBJECT_CLASS(class); */
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);
/*   GtkBindingSet *binding_set; */

  widget_class->realize = utt_artical_realize;
  widget_class->unrealize = utt_artical_unrealize;
  widget_class->size_request = utt_artical_size_request;
  widget_class->size_allocate = utt_artical_size_allocate;
  widget_class->map = utt_artical_map;
  widget_class->unmap = utt_artical_unmap;
  widget_class->expose_event = utt_artical_expose;
  widget_class->key_press_event = utt_artical_key_press; /* set gtk_widget_set_can_focus in utt_artical_init, learn from gtk_entry_init */
  widget_class->key_release_event = utt_artical_key_release;
  widget_class->grab_focus = utt_artical_grab_focus;
  widget_class->focus_in_event = utt_artical_focus_in;
  widget_class->focus_out_event = utt_artical_focus_out;

/*   binding_set = gtk_binding_set_by_class (class); */

  g_type_class_add_private(class, sizeof(UttArticalPrivate));
}

static void
utt_artical_init (UttArtical *artical)
{
  UttArticalPrivate *priv;

  gtk_widget_set_has_window (GTK_WIDGET (artical), TRUE);
  gtk_widget_set_can_focus (GTK_WIDGET (artical), TRUE);

  priv = UTT_ARTICAL_GET_PRIVATE (artical);
  priv->data = NULL;
}

GtkWidget *
utt_artical_new ()
{
  UttArtical *artical;

  artical = g_object_new (UTT_TYPE_ARTICAL,
			  NULL);
  return GTK_WIDGET(artical);
}
