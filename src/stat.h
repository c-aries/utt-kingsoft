#ifndef __STAT_H__
#define __STAT_H__
/* statistics */

#include <glib.h>

struct _stat {		     /* statistics system */
  guint right;			/* right characaters */
  guint pass;			/* from 0 to total, when it equals total the class finish */
  guint sum;			/* sum of characters you have input */
  guint total;			/* totoal of characters the class have */
  guint elapse;			/* time elapse */
};

void stat_reset (struct _stat *stat);
struct _stat *stat_new ();
void stat_destroy (struct _stat **stat);
gdouble stat_pass_speed (struct _stat *stat);
gdouble stat_correct_speed (struct _stat *stat);
gdouble stat_finish (struct _stat *stat);
gdouble stat_correct (struct _stat *stat);
void stat_show (struct _stat *stat);

#endif
