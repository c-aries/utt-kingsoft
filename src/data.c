/* data */
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/ghash.h>
#define __DATA_C__
#include "data.h"
#include "process.h"		/* process data */

#define DEBUG 1

static gchar *icon_filename[ICON_NUM] = {	/* icon path prefix with MEDIADIR(in data.h) and filename */
  "en-kb.png",
  "dashboard.png",
  "hand.png",
};

struct _icon icon[ICON_NUM];

/* icons' size */
#define BUTTON_WIDTH 53
#define BUTTON_HEIGHT 52

struct _key key[] = {
  {"nil", 0, 0, 0, 0, 0, 0, 0, NONE_SHIFT, NULL},				/* eliminate the 0 index, beacause GUINT_TO_POINTER (0) equals to NULL in g_direct_hash */
  /*  */
  {"`", '`', GDK_quoteleft, 8, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, NONE_SHIFT, display_quoteleft},
  {"~", '~', GDK_asciitilde, 8, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, RIGHT_SHIFT, display_asciitilde},
  {"1", '1', GDK_1, 66, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, NONE_SHIFT, display_1},
  {"!", '!', GDK_exclam, 66, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, RIGHT_SHIFT, display_exclam},
  {"2", '2', GDK_2, 124, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_RING, NONE_SHIFT, display_2},
  {"@", '@', GDK_at, 124, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_RING, RIGHT_SHIFT, display_at},
  {"3", '3', GDK_3, 182, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_MIDDLE, NONE_SHIFT, display_3},
  {"#", '#', GDK_numbersign, 182, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_MIDDLE, RIGHT_SHIFT, display_numbersign},
  {"4", '4', GDK_4, 240, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, display_4},
  {"$", '$', GDK_dollar, 240, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, RIGHT_SHIFT, display_dollar},
  {"5", '5', GDK_5, 298, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, display_5},
  {"%", '%', GDK_percent, 298, 5, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, RIGHT_SHIFT, diaplay_percent},
  {"6", '6', GDK_6, 356, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, display_6},
  {"^", '^', GDK_asciicircum, 356, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, LEFT_SHIFT, display_asciicircum},
  {"7", '7', GDK_7, 414, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, display_7},
  {"&", '&', GDK_ampersand, 414, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, LEFT_SHIFT, display_ampersand},
  {"8", '8', GDK_8, 472, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_MIDDLE, NONE_SHIFT, display_8},
  {"*", '*', GDK_asterisk, 472, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_MIDDLE, LEFT_SHIFT, display_asterisk},
  {"9", '9', GDK_9, 530, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_RING, NONE_SHIFT, display_9},
  {"(", '(', GDK_parenleft, 530, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_RING, LEFT_SHIFT, display_parenleft},
  {"0", '0', GDK_0, 588, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_0},
  {")", ')', GDK_parenright, 588, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_parenright},
  {"-", '-', GDK_minus, 646, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_minus},
  {"_", '_', GDK_underscore, 646, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_underscore},
  {"=", '=', GDK_equal, 704, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_equal},
  {"+", '+', GDK_plus, 704, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_plus},
  {"\\", '\\', GDK_backslash, 762, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_backslash},
  {"|", '|', GDK_bar, 762, 5, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_bar},
  /*  */
  {"q", 'q', GDK_q, 92, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, NONE_SHIFT, NULL},
  {"Q", 'Q', GDK_Q, 92, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, NONE_SHIFT, NULL},
  {"w", 'w', GDK_w, 150, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_RING, NONE_SHIFT, NULL},
  {"W", 'W', GDK_W, 150, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_RING, NONE_SHIFT, NULL},
  {"e", 'e', GDK_e, 208, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_MIDDLE, NONE_SHIFT, NULL},
  {"E", 'E', GDK_E, 208, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_MIDDLE, NONE_SHIFT, NULL},
  {"r", 'r', GDK_r, 266, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"R", 'R', GDK_R, 266, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"t", 't', GDK_t, 324, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"T", 'T', GDK_T, 324, 60, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"y", 'y', GDK_y, 382, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"Y", 'Y', GDK_Y, 382, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"u", 'u', GDK_u, 440, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"U", 'U', GDK_U, 440, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"i", 'i', GDK_i, 498, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_MIDDLE, NONE_SHIFT, NULL},
  {"I", 'I', GDK_I, 498, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_MIDDLE, NONE_SHIFT, NULL},
  {"o", 'o', GDK_o, 556, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_RING, NONE_SHIFT, NULL},
  {"O", 'O', GDK_O, 556, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_RING, NONE_SHIFT, NULL},
  {"p", 'p', GDK_p, 614, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, NULL},
  {"P", 'P', GDK_P, 614, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, NULL},
  {"[", '[', GDK_bracketleft, 672, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_bracketleft},
  {"\{", '{', GDK_braceleft, 672, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_braceleft},
  {"]", ']', GDK_bracketright, 730, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_bracketright},
  {"}", '}', GDK_braceright, 730, 60, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_braceright},
  /*  */
  {"a", 'a', GDK_a, 119, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, NONE_SHIFT, NULL},
  {"A", 'A', GDK_A, 119, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, NONE_SHIFT, NULL},
  {"s", 's', GDK_s, 177, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_RING, NONE_SHIFT, NULL},
  {"S", 'S', GDK_S, 177, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_RING, NONE_SHIFT, NULL},
  {"d", 'd', GDK_d, 235, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_MIDDLE, NONE_SHIFT, NULL},
  {"D", 'D', GDK_D, 235, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_MIDDLE, NONE_SHIFT, NULL},
  {"f", 'f', GDK_f, 293, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"F", 'F', GDK_F, 293, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"g", 'g', GDK_g, 351, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"G", 'G', GDK_G, 351, 115, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"h", 'h', GDK_h, 409, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"H", 'H', GDK_H, 409, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"j", 'j', GDK_j, 467, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"J", 'J', GDK_J, 467, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"k", 'k', GDK_k, 525, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_MIDDLE, NONE_SHIFT, NULL},
  {"K", 'K', GDK_K, 525, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_MIDDLE, NONE_SHIFT, NULL},
  {"l", 'l', GDK_l, 583, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_RING, NONE_SHIFT, NULL},
  {"L", 'L', GDK_L, 583, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_RING, NONE_SHIFT, NULL},
  {";", ';', GDK_semicolon, 641, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_semicolon},
  {":", ':', GDK_colon, 641, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_colon},
  {"'", '\'', GDK_apostrophe, 699, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_apostrophe},
  {"""", '"', GDK_quotedbl, 699, 115, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_quotedbl},
  /*  */
  {"z", 'z', GDK_z, 153, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, NONE_SHIFT, NULL},
  {"Z", 'Z', GDK_Z, 153, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_LITTLE, NONE_SHIFT, NULL},
  {"x", 'x', GDK_x, 211, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_RING, NONE_SHIFT, NULL},
  {"X", 'X', GDK_X, 211, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_RING, NONE_SHIFT, NULL},
  {"c", 'c', GDK_c, 269, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_MIDDLE, NONE_SHIFT, NULL},
  {"C", 'C', GDK_C, 269, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_MIDDLE, NONE_SHIFT, NULL},
  {"v", 'v', GDK_v, 327, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"V", 'V', GDK_V, 327, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"b", 'b', GDK_b, 385, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"B", 'B', GDK_B, 385, 170, BUTTON_WIDTH, BUTTON_HEIGHT, LEFT_INDEX, NONE_SHIFT, NULL},
  {"n", 'n', GDK_n, 443, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"N", 'N', GDK_N, 443, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"m", 'm', GDK_m, 501, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {"M", 'M', GDK_M, 501, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_INDEX, NONE_SHIFT, NULL},
  {",", ',', GDK_comma, 559, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_MIDDLE, NONE_SHIFT, display_comma},
  {"<", '<', GDK_less, 559, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_MIDDLE, LEFT_SHIFT, display_less},
  {".", '.', GDK_period, 617, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_RING, NONE_SHIFT, display_period},
  {">", '>', GDK_greater, 617, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_RING, LEFT_SHIFT, display_greater},
  {"/", '/', GDK_slash, 675, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, NONE_SHIFT, display_slash},
  {"?", '?', GDK_question, 675, 170, BUTTON_WIDTH, BUTTON_HEIGHT, RIGHT_LITTLE, LEFT_SHIFT, display_question},
  /* control keys */
  {"Shift<L>", 0, GDK_Shift_L, 8, 170, 140, 52, LEFT_LITTLE, LEFT_SHIFT, NULL},
  {"Shift<R>", 0, GDK_Shift_R, 733, 170, 140, 52, RIGHT_LITTLE, RIGHT_SHIFT, NULL},
};

GHashTable *key_val_ht;		/* key vlaue hash table */
GHashTable *key_name_ht;	/* key name hash table */
GHashTable *key_char_ht;	/* key character table */

void
data_precheck_and_init()
{
  gchar *path;
  gint i;

  /* init and check icons' path */
  for (i = 0; i < ICON_NUM; i++) {
    path = icon[i].path = g_strjoin("", MEDIADIR, icon_filename[i], NULL);
    if (!g_file_test (path, G_FILE_TEST_EXISTS)) {
      g_error ("%s doesn't exists", path);
    }
  }

  /* check path */
  if (!g_file_test (UIFILE, G_FILE_TEST_EXISTS)) {
    g_error ("%s doesn't exists", UIFILE);
  }

  /* key hash table */
  key_val_ht = g_hash_table_new (g_direct_hash, g_direct_equal);
  key_name_ht = g_hash_table_new (g_str_hash, g_str_equal);
  key_char_ht = g_hash_table_new (g_direct_hash, g_direct_equal);
  for (i = 0; i < G_N_ELEMENTS (key); i++) {
    g_hash_table_insert (key_val_ht, GUINT_TO_POINTER (key[i].val), GINT_TO_POINTER (i));
    g_hash_table_insert (key_name_ht, key[i].name, GINT_TO_POINTER (i));
    g_hash_table_insert (key_char_ht, GUINT_TO_POINTER ((guint)key[i].ch), GINT_TO_POINTER (i));
  }
}

void
data_deinit()
{
  gint i;

  for (i = 0; i < ICON_NUM; i++) {
    g_free(icon[i].path);
  }
  g_hash_table_destroy (key_val_ht);
  g_hash_table_destroy (key_name_ht);
}

gboolean
finger_get_origin (enum _finger finger, gint *x, gint *y)
{
  gboolean ret = TRUE;

  g_return_val_if_fail (x && y, FALSE);

  switch (finger) {
  case LEFT_LITTLE:
    {
      *x = 36;
      *y = 82;
    }
    break;
  case LEFT_RING:
    {
      *x = 64;
      *y = 38;
    }
    break;
  case LEFT_MIDDLE:
    {
      *x = 89;
      *y = 21;
    }
    break;
  case LEFT_INDEX:
    {
      *x = 118;
      *y = 43;
    }
    break;
  case LEFT_THUMB:
    {
      *x = 160;
      *y = 125;
    }
    break;
  case RIGHT_THUMB:
    {
      *x = 371;
      *y = 125;
    }
    break;
  case RIGHT_INDEX:
    {
      *x = 413;
      *y = 43;
    }
    break;
  case RIGHT_MIDDLE:
    {
      *x = 442;
      *y = 21;
    }
    break;
  case RIGHT_RING:
    {
      *x = 468;
      *y = 38;
    }
    break;
  case RIGHT_LITTLE:
    {
      *x = 496;
      *y = 82;
    }
    break;
  default:
    ret = FALSE;
    break;
  }

  return ret;
}
