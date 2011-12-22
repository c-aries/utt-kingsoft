#ifndef __UTT_ARTICLE_H__
#define __UTT_ARTICLE_H__

#include <gtk/gtkwidget.h>

G_BEGIN_DECLS

#define UTT_TYPE_ARTICLE	(utt_article_get_type ())
#define UTT_ARTICLE(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), UTT_TYPE_ARTICLE, GttArticle))
#define UTT_ARTICLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), UTT_TYPE_ARTICLE, UttArticleClass))
#define UTT_IS_ARTICLE(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), UTT_TYPE_ARTICLE))
#define UTT_IS_ARTICLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), UTT_TYPE_ARTICLE))
#define UTT_ARTICLE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), UTT_TYPE_ARTICLE, UttArticleClass))

typedef struct _UttArticle UttArticle;
typedef struct _UttArticleClass UttArticleClass;

struct _UttArticle {
  GtkWidget widget;
};

struct _UttArticleClass {
  GtkWidgetClass parent_class;
};

GType utt_article_get_type() G_GNUC_CONST;
GtkWidget *utt_article_new();

G_END_DECLS

#endif
