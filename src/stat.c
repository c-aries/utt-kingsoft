/* library */
#include "stat.h"

/* don't define struct _stat stat; here. this file should be a module */

void
stat_reset (struct _stat *stat)
{
  stat->right = \
    stat->sum = \
    stat->pass = \
    stat->elapse = 0;
}

void
stat_init (struct _stat *stat, guint total)
{
  stat->total = total;
  stat_reset (stat);
}

struct _stat *
stat_new (guint total)
{
  struct _stat *stat;

  stat = g_new (struct _stat, 1);
  stat_init (stat, total);
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

  if (stat->sum == 0) {
    return 0;
  }
  ret = stat->right * 100.0 / stat->sum;
  return ret;
}

void
stat_show (struct _stat *stat)
{
  g_print ("right %u, pass %u, sum %u, total %u, elapse %u\n", stat->right,
	   stat->pass, stat->sum, stat->total, stat->elapse);
  g_print ("correct: %.2lf%%\n", stat_correct (stat));
  g_print ("finish: %.2lf%%\n", stat_finish (stat));
  g_print ("speed: %.2lf/m, %.2lf/m\n", stat_pass_speed (stat), stat_correct_speed (stat));
}
