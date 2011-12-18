#ifndef __UTT_ARTICAL_H__
#define __UTT_ARTICAL_H__

#include <gtk/gtkwidget.h>

G_BEGIN_DECLS

#define UTT_TYPE_ARTICAL	(utt_artical_get_type ())
#define UTT_ARTICAL(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), UTT_TYPE_ARTICAL, GttArtical))
#define UTT_ARTICAL_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), UTT_TYPE_ARTICAL, UttArticalClass))
#define UTT_IS_ARTICAL(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), UTT_TYPE_ARTICAL))
#define UTT_IS_ARTICAL_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), UTT_TYPE_ARTICAL))
#define UTT_ARTICAL_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), UTT_TYPE_ARTICAL, UttArticalClass))

typedef struct _UttArtical UttArtical;
typedef struct _UttArticalClass UttArticalClass;

struct _UttArtical {
  GtkWidget widget;
};

struct _UttArticalClass {
  GtkWidgetClass parent_class;
};

GType utt_artical_get_type() G_GNUC_CONST;
GtkWidget *utt_artical_new();

G_END_DECLS

#endif
