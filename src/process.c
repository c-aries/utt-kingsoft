#include <glib.h>
#define __PROCESS_C__
#include "process.h"

static void
display_helper (cairo_t *cr, gint r, gint g, gint b, gint x, gint y, gint w, gint h)
{
  cairo_set_source_rgb (cr, r/255., g/255., b/255.);
  cairo_rectangle (cr, x, y, w, h);
  cairo_fill (cr);
}

void
display_quoteleft (cairo_t *cr)
{
  display_helper (cr, 230, 230, 223, 14, 8, 20, 12);
}

void
display_asciitilde (cairo_t *cr)
{
  display_helper (cr, 216, 215, 220, 18, 28, 12, 10);
}

void
display_1 (cairo_t *cr)
{
  display_helper (cr, 232, 232, 234, 23, 6, 6, 16);
}

void
display_exclam (cairo_t *cr)
{
  display_helper (cr, 214, 213, 219, 20, 29, 8, 17);
}

void
display_2 (cairo_t *cr)
{
  display_helper (cr, 228, 228, 230, 14, 5, 22, 22);
}

void
display_at (cairo_t *cr)
{
  display_helper (cr, 214, 213, 219, 20, 29, 12, 17);
}

void
display_3 (cairo_t *cr)
{
  display_helper (cr, 230, 230, 233, 18, 5, 15, 17);
}

void
display_numbersign (cairo_t *cr)
{
  display_helper (cr, 214, 214, 219, 22, 29, 12, 18);
}

void
display_4 (cairo_t *cr)
{
  display_helper (cr, 230, 230, 233, 18, 5, 12, 19);
}

void
display_dollar (cairo_t *cr)
{
  display_helper (cr, 213, 213, 218, 19, 29, 12, 17);
}

void
display_5 (cairo_t *cr)
{
  display_helper (cr, 231, 231, 234, 19, 5, 17, 17);
}

void
diaplay_percent (cairo_t *cr)
{
  display_helper (cr, 214, 214, 219, 23, 29, 12, 18);
}

void
display_6 (cairo_t *cr)
{
  display_helper (cr, 232, 232, 235, 22, 5, 10, 11);
}

void
display_asciicircum (cairo_t *cr)
{
  display_helper (cr, 214, 214, 219, 20, 29, 12, 18);
}

void
display_7 (cairo_t *cr)
{
  display_helper (cr, 230, 230, 233, 22, 5, 14, 17);
}

void
display_ampersand (cairo_t *cr)
{
  display_helper (cr, 213, 213, 219, 22, 30, 11, 16);
}

void
display_8 (cairo_t *cr)
{
  display_helper (cr, 234, 234, 235, 26, 5, 9, 9);
}

void
display_asterisk (cairo_t *cr)
{
  display_helper (cr, 214, 214, 219, 25, 30, 11, 16);
}

void
display_9 (cairo_t *cr)
{
  display_helper (cr, 228, 228, 231, 21, 5, 8, 20);
}

void
display_parenleft (cairo_t *cr)
{
  display_helper (cr, 216, 216, 222, 22, 30, 11, 16);
}

void
display_0 (cairo_t *cr)
{
  display_helper (cr, 230, 230, 233, 21, 5, 10, 20);
}

void
display_parenright (cairo_t *cr)
{
  display_helper (cr, 213, 213, 218, 24, 30, 11, 16);
}

void
display_minus (cairo_t *cr)
{
  display_helper (cr, 223, 223, 227, 18, 20, 14, 6);
}

void
display_underscore (cairo_t *cr)
{
  display_helper (cr, 213, 213, 218, 22, 36, 11, 6);
}

void
display_equal (cairo_t *cr)
{
  display_helper (cr, 230, 230, 233, 22, 8, 12, 12);
}

void
display_plus (cairo_t *cr)
{
  display_helper (cr, 214, 214, 219, 22, 32, 11, 10);
}

void
display_backslash (cairo_t *cr)
{
  display_helper (cr, 229, 229, 232, 20, 6, 6, 20);
}

void
display_bar (cairo_t *cr)
{
  display_helper (cr, 214, 214, 219, 19, 29, 12, 19);
}

void
display_bracketleft (cairo_t *cr)
{}

void
display_braceleft (cairo_t *cr)
{}

void
display_bracketright (cairo_t *cr)
{}

void
display_braceright (cairo_t *cr)
{}

void
display_semicolon (cairo_t *cr)
{}

void
display_colon (cairo_t *cr)
{}

void
display_apostrophe (cairo_t *cr)
{}

void
display_quotedbl (cairo_t *cr)
{}

void
display_comma (cairo_t *cr)
{}

void
display_less (cairo_t *cr)
{}

void
display_period (cairo_t *cr)
{}

void
display_greater (cairo_t *cr)
{}

void
display_slash (cairo_t *cr)
{}

void
display_question (cairo_t *cr)
{}
