#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "data.h"

#ifndef DATADIR
#error "DATADIR doesn't exist"
#endif

#define MEDIADIR DATADIR "/media/"

struct icon icon[] = {
  { "en-kb.png", },
  { "key.png", },
};

struct key key[] = {
  { GDK_g, 0, 351, 115},
  { GDK_G, 0, 351, 115},
};

struct icon *
load_icons ()
{
  gchar *path;
  gint i;

  if (icon[0].pix) {
    return NULL;
  }

  for (i = 0; i < G_N_ELEMENTS (icon); i++) {
    path = g_strjoin ("", MEDIADIR, icon[i].filename, NULL);
    if (!g_file_test (path, G_FILE_TEST_EXISTS)) {
      g_error ("%s doesn't exists", path);
    }
    icon[i].pix = gdk_pixbuf_new_from_file (path, NULL);
    g_free (path);
    icon[i].w = gdk_pixbuf_get_width (icon[i].pix);
    icon[i].h = gdk_pixbuf_get_width (icon[i].pix);
  }

  return icon;
}

struct key *
load_keys ()
{
  return key;
}
