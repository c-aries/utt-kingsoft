#ifndef __CLASS_H__
#define __CLASS_H__

#include <glib.h>

struct class {
  const gchar *name;
  struct class_strategy *strategy;
  gchar *text;
  void *private;
};

struct class_strategy {
  void (*init) (struct class *class);
  void (*deinit) (struct class *class);
  void (*load) (struct class *class, void *data);
  gint (*read) (struct class *class, void *buf, gint count);
};

#endif
