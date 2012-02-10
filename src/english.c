#include <gtk/gtk.h>
#include "main.h"

#define ENGLISH_KEYBOARD DATADIR "/media/en-kb.png"

static gboolean
on_key_press (GtkWidget *widget, GdkEventKey *key, gpointer data)
{
  g_print ("key press %08x\n", key->keyval);
  return FALSE;
}

static gboolean
on_image_expose (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  g_print ("%s\n", G_STRLOC);
  return FALSE;
}

void
english_ui_init (GtkBuilder *builder)
{
  GtkWidget *button;
  GtkImage *image;

  if (!g_file_test (ENGLISH_KEYBOARD, G_FILE_TEST_EXISTS)) { /* pre check */
    g_error ("%s doesn't exists", ENGLISH_KEYBOARD);
  }

  ui.english_window = GTK_WIDGET (gtk_builder_get_object (builder, "english_window"));

  /* menu button */
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button1"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button2"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button5"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_menu_press), NULL);

  image = GTK_IMAGE (gtk_builder_get_object (builder, "image1"));
  gtk_image_set_from_file (image, ENGLISH_KEYBOARD);
  g_signal_connect_after (image, "expose-event", G_CALLBACK (on_image_expose), NULL);

  g_signal_connect (ui.english_window, "key-press-event", G_CALLBACK (on_key_press), NULL);
}
