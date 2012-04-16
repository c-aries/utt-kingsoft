#ifndef __STAT_H__
#define __STAT_H__
/* statistics */

#include <glib.h>

struct _stat {		     /* statistics system */
  guint right;			/* right characaters */
  guint sum;			/* sum of characters you have input */
  guint total;			/* totoal of characters the class have */
  guint elapse;			/* time elapse */
};

void stat_init (struct _stat *stat);
struct _stat *stat_new ();
void stat_destroy (struct _stat **stat);

#endif
