#include "stat.h"

void
stat_init (struct _stat *stat)
{
  stat->right = \
    stat->sum = \
    stat->total = \
    stat->elapse = 0;
}

struct _stat *
stat_new ()
{
  struct _stat *stat;

  stat = g_new (struct _stat, 1);
  stat_init (stat);
  return stat;
}

void
stat_destroy (struct _stat **stat)
{
  g_free (*stat);
  *stat = NULL;
}
