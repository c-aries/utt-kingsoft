#ifndef __DATA_H__
#define __DATA_H__
/* data path macros */

#include <gtk/gtk.h>

/* command macros */
#ifndef DATADIR
#error "DATADIR doesn't exist"
#endif
#ifndef PKGDATADIR
#error "PKGDATADIR doesn't exist"
#endif

#define MEDIADIR DATADIR "/media/"
#define UIFILE PKGDATADIR "/utt.glade"

/* icons' path */
/* #define ICONPATH_KB_EN MEDIADIR "en-kb.png" /\* english keyboard *\/ */

enum {
  ICON_KB_EN,
  ICON_DASHBOARD,
  ICON_HAND,
  ICON_NUM,			/* icons number */
};

struct _icon {
  gchar *path;
};

enum _finger {				/* finger */
  LEFT_THUMB,
  LEFT_INDEX,
  LEFT_MIDDLE,
  LEFT_RING,
  LEFT_LITTLE,
  RIGHT_THUMB,
  RIGHT_INDEX,
  RIGHT_MIDDLE,
  RIGHT_RING,
  RIGHT_LITTLE,
};

struct _key {
  gchar *name;
  guchar ch;			/* printable character */
  guint val;			/* key value */
  /* fill area infromation, need to improve, in order to satisfy ENTER button, etc. */
  int x, y;			/* start x, start y */
  int width, height;
  enum _finger finger;
};

void data_precheck_and_init();
void data_deinit();
/* struct icon *load_icons (); */
/* struct key *load_keys (); */

#endif
