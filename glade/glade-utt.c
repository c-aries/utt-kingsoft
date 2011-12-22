#include <gladeui/glade-base-editor.h>
#include <utt/article.h>

void
glade_utt_init (const gchar *name)
{
  g_debug (G_STRFUNC);
}

void
glade_utt_article_post_create (GladeWidgetAdaptor *adaptor,
			       GObject *object,
			       GladeCreateReason reason)
{
  g_debug (G_STRFUNC);

  g_return_if_fail (UTT_IS_ARTICLE (object));
}
