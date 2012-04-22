/* instance */
#include "class.h"

struct random_private {
  gint index;
};

static void
random_init (struct class *class)
{
  struct random_private *priv;

  priv = class->private = g_new (struct random_private, 1);
  priv->index = 0;
}

static void
random_deinit (struct class *class)
{
  g_free (class->private);
}

static gint
random_read (struct class *class, void *buf, gint count)
{
  return 0;
}

static gchar
random_next_char (struct class *class)
{
  return 0;
}

struct class_strategy strategy_random = {
  .init = random_init,
  .deinit = random_deinit,
  .read = random_read,
  .next_char = random_next_char,
};
