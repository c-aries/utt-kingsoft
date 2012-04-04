/* data path macros */

#ifndef __DATA_H__
#define __DATA_H__

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
  ICON_NUM,			/* icons number */
};

struct _icon {
  gchar *path;
};

/* struct key { */
/*   guint keyval, state; */
/*   gint x, y; */
/* }; */

void data_precheck_and_init();
void data_deinit();
/* struct icon *load_icons (); */
/* struct key *load_keys (); */

#endif
