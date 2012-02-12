#ifndef __DATA_H__
#define __DATA_H__

#include <gtk/gtk.h>

enum {
  DATA_ENGLISH_KB,
  DATA_KEY,
};

struct icon {
  gchar *filename;
  GdkPixbuf *pix;
  gint w, h;
};

struct key {
  guint keyval, state;
  gint x, y;
};

struct icon *load_icons ();
struct key *load_keys ();

#endif
