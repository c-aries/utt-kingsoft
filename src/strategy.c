/* instance */
#include <glib/gstdio.h>
#include <stdio.h>
#include "class.h"

struct random_private {
  gchar *text;
};

static void
random_init (struct class *class)
{
  struct random_private *priv;

  priv = class->private = g_new (struct random_private, 1);
}

static void
random_deinit (struct class *class)
{
  g_free (class->private);
}

static void
random_load (struct class *class, void *data)
{
  struct random_private *priv = class->private;

  priv->text = data;
}

static gint
random_read (struct class *class, void *buf, gint count)
{
  struct random_private *priv = class->private;
  glong len = g_utf8_strlen (priv->text, -1);
  gint i, j;

  g_print ("len %ld\n", len);
  for (i = 0; i < count; i++) {
    j = g_random_int_range (0, len);
    *((gchar *)buf + i) = priv->text[j];
  }
  return count;
}

struct class_strategy strategy_random = {
  .init = random_init,		/* set text or file */
  .deinit = random_deinit,
  .load = random_load,
  .read = random_read,
};

struct file_private {
  FILE *fp;
};

static void
file_init (struct class *class)
{
  struct file_private *priv;

  priv = class->private = g_new (struct file_private, 1);
  priv->fp = g_fopen ("/tmp/class0", "r");
}

static void
file_deinit (struct class *class)
{
  struct file_private *priv;

  priv = class->private;
  fclose (priv->fp);
  g_free (class->private);
}

static void
file_load (struct class *class, void *data)
{
  struct file_private *priv = class->private;

  priv->fp = g_fopen (data, "r");
}

static gint
file_read (struct class *class, void *buf, gint count)
{
  struct file_private *priv = class->private;
  FILE *fp = priv->fp;

  return fread (buf, sizeof (gchar), count, fp);
}

struct class_strategy strategy_file = {
  .init = file_init,
  .deinit = file_deinit,
  .load = file_load,
  .read = file_read,
};
