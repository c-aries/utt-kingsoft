#ifndef __CLASS_RANDOM_H__
#define __CLASS_RANDOM_H__

#include <glib.h>

struct class_random {
  const gchar *text;
};

struct class_random *class_random_new ();
void class_random_destroy (struct class_random *class);
void class_random_set_text (struct class_random *class, gchar *text);
gint class_random_read (struct class_random *class, void *buf, gint count);

#endif

