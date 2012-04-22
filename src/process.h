#ifndef __PROCESS_H__
#define __PROCESS_H__

#if !defined(__DATA_C__) && !defined(__PROCESS_C__)
#error "only use by data.c"
#endif

#include <cairo.h>

void display_asciitilde (cairo_t *cr);
void display_quoteleft (cairo_t *cr);
void display_1 (cairo_t *cr);
void display_exclam (cairo_t *cr);
void display_2 (cairo_t *cr);
void display_at (cairo_t *cr);
void display_3 (cairo_t *cr);
void display_numbersign (cairo_t *cr);
void display_4 (cairo_t *cr);
void display_dollar (cairo_t *cr);
void display_5 (cairo_t *cr);
void diaplay_percent (cairo_t *cr);
void display_6 (cairo_t *cr);
void display_asciicircum (cairo_t *cr);
void display_7 (cairo_t *cr);
void display_ampersand (cairo_t *cr);
void display_8 (cairo_t *cr);
void display_asterisk (cairo_t *cr);
void display_9 (cairo_t *cr);
void display_parenleft (cairo_t *cr);
void display_0 (cairo_t *cr);
void display_parenright (cairo_t *cr);
void display_minus (cairo_t *cr);
void display_underscore (cairo_t *cr);
void display_equal (cairo_t *cr);
void display_plus (cairo_t *cr);
void display_backslash (cairo_t *cr);
void display_bar (cairo_t *cr);
void display_bracketleft (cairo_t *cr);
void display_braceleft (cairo_t *cr);
void display_bracketright (cairo_t *cr);
void display_braceright (cairo_t *cr);
void display_semicolon (cairo_t *cr);
void display_colon (cairo_t *cr);
void display_apostrophe (cairo_t *cr);
void display_quotedbl (cairo_t *cr);
void display_comma (cairo_t *cr);
void display_less (cairo_t *cr);
void display_period (cairo_t *cr);
void display_greater (cairo_t *cr);
void display_slash (cairo_t *cr);
void display_question (cairo_t *cr);
/* void display_ (cairo_t *cr); */

#endif

