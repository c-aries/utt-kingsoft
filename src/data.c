#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/ghash.h>
#include "data.h"

static gchar *icon_filename[ICON_NUM] = {	/* icon path prefix with MEDIADIR(in data.h) and filename */
  "en-kb.png",
};

struct _icon icon[ICON_NUM];

struct _key {			/* place it to data.c ? */
  gchar *name;
  gint value;
  /* fill area infromation, need to improve, in order to satisfy ENTER button, etc. */
  int startx, starty;
  int width, height;
};

static struct _key key[] = {
  {"a", 0x061, 119, 115, 53, 52},
  {"s", 0x073, 177, 115, 53, 52},
};

/* struct key key[] = { */
/*   { GDK_g, 0, 351, 115}, */
/*   { GDK_G, 0, 351, 115}, */
/* }; */

static GHashTable *ht;		/* key's hash table */

void
data_precheck_and_init()
{
  gchar *path;
  gint i;

  /* init and check icons' path */
  for (i = 0; i < ICON_NUM; i++) {
    path = icon[i].path = g_strjoin("", MEDIADIR, icon_filename[i], NULL);
    if (!g_file_test (path, G_FILE_TEST_EXISTS)) {
      g_error ("%s doesn't exists", path);
    }
  }

  /* check path */
  if (!g_file_test (UIFILE, G_FILE_TEST_EXISTS)) {
    g_error ("%s doesn't exists", UIFILE);
  }

  ht = g_hash_table_new (g_str_hash, g_str_equal);
  for (i = 0; i < G_N_ELEMENTS (key); i++) {
    g_hash_table_insert (ht, key[i].name, GINT_TO_POINTER (i));
  }
}

void
data_deinit()
{
  gint i;

  for (i = 0; i < ICON_NUM; i++) {
    g_free(icon[i].path);
  }
  g_hash_table_destroy (ht);
}

/* struct icon * */
/* load_icons () */
/* { */
/*   gchar *path; */
/*   gint i; */

/*   if (icon[0].pix) { */
/*     return NULL; */
/*   } */

/*   for (i = 0; i < G_N_ELEMENTS (icon); i++) { */
/*     path = g_strjoin ("", MEDIADIR, icon[i].filename, NULL); */
/*     if (!g_file_test (path, G_FILE_TEST_EXISTS)) { */
/*       g_error ("%s doesn't exists", path); */
/*     } */
/*     icon[i].pix = gdk_pixbuf_new_from_file (path, NULL); */
/*     g_free (path); */
/*     icon[i].w = gdk_pixbuf_get_width (icon[i].pix); */
/*     icon[i].h = gdk_pixbuf_get_height (icon[i].pix); */
/*   } */

/*   return icon; */
/* } */

/* struct key * */
/* load_keys () */
/* { */
/*   return key; */
/* } */
