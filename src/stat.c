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

/* per minute */
gdouble
stat_pass_speed (struct _stat *stat)
{
  gdouble ret;

  if (stat->elapse == 0) {
    return 0;
  }
  ret = stat->pass * 60.0 / stat->elapse;
  return ret;
}

/* per minute */
gdouble
stat_correct_speed (struct _stat *stat)
{
  gdouble ret;

  if (stat->elapse == 0) {
    return 0;
  }
  ret = stat->right * 60.0 / stat->elapse;
  return ret;
}

/* per minute */
gdouble
stat_finish (struct _stat *stat)
{
  gdouble ret;

  if (stat->total == 0) {
    return 0;
  }
  ret = stat->pass * 100.0 / stat->total;
  return ret;
}

/* per minute */
gdouble
stat_correct (struct _stat *stat)
{
  gdouble ret;

  if (stat->total == 0) {
    return 0;
  }
  ret = stat->right * 100.0 / stat->sum;
  return ret;
}
