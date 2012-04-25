#include "class_random.h"

#define DEUBUG

struct class_random *
class_random_new ()
{
  struct class_random *class;

  class = g_new0 (struct class_random, 1);
  return class;
}

void
class_random_destroy (struct class_random *class)
{
  g_free (class);
}

void
class_random_set_text (struct class_random *class, gchar *text)
{
  class->text = text;
}

gint
class_random_read (struct class_random *class, void *buf, gint count)
{
  glong len = g_utf8_strlen (class->text, -1);
  gint i, j;

#ifdef DEBUG
  g_print ("len %ld\n", len);
#endif
  for (i = 0; i < count; i++) {
    j = g_random_int_range (0, len);
    *((gchar *)buf + i) = class->text[j];
  }
  *((gchar *)buf + i) = '\0';
  return count;
}
