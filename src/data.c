#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/ghash.h>
#include "data.h"

#define DEBUG 1

static gchar *icon_filename[ICON_NUM] = {	/* icon path prefix with MEDIADIR(in data.h) and filename */
  "en-kb.png",
  "dashboard.png",
};

struct _icon icon[ICON_NUM];

/* icons' size */
#define BUTTON_WIDTH 53
#define BUTTON_HEIGHT 52

struct _key key[] = {
  {"nil", 0, 0, 0, 0, 0, 0},				/* eliminate the 0 index, beacause GUINT_TO_POINTER (0) equals to NULL in g_direct_hash */
  /*  */
  {"`", '`', GDK_quoteleft, 8, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"~", '~', GDK_asciitilde, 8, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"1", '1', GDK_1, 66, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"!", '!', GDK_exclam, 66, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"2", '2', GDK_2, 124, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"@", '@', GDK_at, 124, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"3", '3', GDK_3, 182, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"#", '#', GDK_numbersign, 182, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"4", '4', GDK_4, 240, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"$", '$', GDK_dollar, 240, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"5", '5', GDK_5, 298, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"%", '%', GDK_percent, 298, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"6", '6', GDK_6, 356, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"^", '^', GDK_asciicircum, 356, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"7", '7', GDK_7, 414, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"&", '&', GDK_ampersand, 414, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"8", '8', GDK_8, 472, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"*", '*', GDK_asterisk, 472, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"9", '9', GDK_9, 530, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"(", '(', GDK_parenleft, 530, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"0", '0', GDK_0, 588, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {")", ')', GDK_parenright, 588, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"-", '-', GDK_minus, 646, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"_", '_', GDK_underscore, 646, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"=", '=', GDK_equal, 704, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"+", '+', GDK_plus, 704, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"\\", '\\', GDK_backslash, 762, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"|", '|', GDK_bar, 762, 5, BUTTON_WIDTH, BUTTON_HEIGHT},
  /*  */
  {"q", 'q', GDK_q, 92, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"Q", 'Q', GDK_Q, 92, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"w", 'w', GDK_w, 150, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"W", 'W', GDK_W, 150, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"e", 'e', GDK_e, 208, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"E", 'E', GDK_E, 208, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"r", 'r', GDK_r, 266, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"R", 'R', GDK_R, 266, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"t", 't', GDK_t, 324, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"T", 'T', GDK_T, 324, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"y", 'y', GDK_y, 382, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"Y", 'Y', GDK_Y, 382, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"u", 'u', GDK_u, 440, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"U", 'U', GDK_U, 440, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"i", 'i', GDK_i, 498, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"I", 'I', GDK_I, 498, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"o", 'o', GDK_o, 556, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"O", 'O', GDK_O, 556, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"p", 'p', GDK_p, 614, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"P", 'P', GDK_P, 614, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"[", '[', GDK_bracketleft, 672, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"\{", '{', GDK_braceleft, 672, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"]", ']', GDK_bracketright, 730, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"}", '}', GDK_braceright, 730, 60, BUTTON_WIDTH, BUTTON_HEIGHT},
  /*  */
  {"a", 'a', GDK_a, 119, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"A", 'A', GDK_A, 119, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"s", 's', GDK_s, 177, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"S", 'S', GDK_S, 177, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"d", 'd', GDK_d, 235, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"D", 'D', GDK_D, 235, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"f", 'f', GDK_f, 293, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"F", 'F', GDK_F, 293, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"g", 'g', GDK_g, 351, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"G", 'G', GDK_G, 351, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"h", 'h', GDK_h, 409, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"H", 'H', GDK_H, 409, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"j", 'j', GDK_j, 467, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"J", 'J', GDK_J, 467, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"k", 'k', GDK_k, 525, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"K", 'K', GDK_K, 525, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"l", 'l', GDK_l, 583, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"L", 'L', GDK_L, 583, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {";", ';', GDK_semicolon, 641, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {":", ':', GDK_colon, 641, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"'", '\'', GDK_apostrophe, 699, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"""", '"', GDK_quotedbl, 699, 115, BUTTON_WIDTH, BUTTON_HEIGHT},
  /*  */
  {"z", 'z', GDK_z, 153, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"Z", 'Z', GDK_Z, 153, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"x", 'x', GDK_x, 211, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"X", 'X', GDK_X, 211, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"c", 'c', GDK_c, 269, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"C", 'C', GDK_C, 269, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"v", 'v', GDK_v, 327, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"V", 'V', GDK_V, 327, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"b", 'b', GDK_b, 385, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"B", 'B', GDK_B, 385, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"n", 'n', GDK_n, 443, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"N", 'N', GDK_N, 443, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"m", 'm', GDK_m, 501, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"M", 'M', GDK_M, 501, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {",", ',', GDK_comma, 559, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"<", '<', GDK_less, 559, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {".", '.', GDK_period, 617, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {">", '>', GDK_greater, 617, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"/", '/', GDK_slash, 675, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
  {"?", '?', GDK_question, 675, 170, BUTTON_WIDTH, BUTTON_HEIGHT},
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
