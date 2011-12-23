#include <stdlib.h>
#include <locale.h>
#include <glib.h>
#include "common.h"

void
utt_set_locale ()
{
  gchar *locale, *localedir;

  locale = getenv ("LANG");
  if (!locale) {
    locale = "C";
  }

  setlocale (LC_ALL, locale);
#ifdef LOCALEDIR
  localedir = LOCALEDIR;
#else
  localedir = "/usr/share/locale";
#endif
  bindtextdomain ("utt", localedir);
  textdomain ("utt");
}
