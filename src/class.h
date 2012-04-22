#ifndef __CLASS_H__
#define __CLASS_H__

#include <glib.h>

struct class {
  void *private;
};

struct class_strategy {
  void (*init) (struct class *class);
  void (*deinit) (struct class *class);
  gint (*read) (struct class *class, void *buf, gint count);
  gchar (*next_char) (struct class *class);
};

#endif
