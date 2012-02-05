#include <gtk/gtk.h>
#include "main.h"

#define ENGLISH_KEYBOARD DATADIR "/media/en-kb.png"

void
english_ui_init (GtkBuilder *builder)
{
  GtkWidget *button;
  GtkImage *image;

  if (!g_file_test (ENGLISH_KEYBOARD, G_FILE_TEST_EXISTS)) { /* pre check */
    g_error ("%s doesn't exists", ENGLISH_KEYBOARD);
  }

  /* menu button */
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button1"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button2"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);
  button = GTK_WIDGET (gtk_builder_get_object (builder, "button5"));
  g_signal_connect (button, "button-press-event", G_CALLBACK (on_menu_press), NULL);

  image = GTK_IMAGE (gtk_builder_get_object (builder, "image1"));
  gtk_image_set_from_file (image, ENGLISH_KEYBOARD);
}
