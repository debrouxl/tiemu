#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "msg_cb.h"
#include "msg_dbox.h"
#include "support.h"

#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "main.h"

static gboolean b = TRUE;

/* Msg box with halting */
void msg_box(const gchar *title, gchar *message)
{
  GtkWidget *dbox;
  gpointer data;

  b = TRUE;
  dbox = create_msg_dbox();

  data = gtk_object_get_data(GTK_OBJECT(dbox), "msg_dbox");
  gtk_window_set_title (GTK_WINDOW(data), title);
  data = gtk_object_get_data(GTK_OBJECT(dbox), "label2");
  gtk_label_set_text(GTK_LABEL(data), message);

  gtk_widget_show_all(dbox);
}

/* Msg box without halting */
void msg_box2(const gchar *title, gchar *message)
{
  GtkWidget *dbox;
  gpointer data;

  b = FALSE;
  dbox = create_msg_dbox();

  data = gtk_object_get_data(GTK_OBJECT(dbox), "msg_dbox");
  gtk_window_set_title (GTK_WINDOW(data), title);
  data = gtk_object_get_data(GTK_OBJECT(dbox), "label2");
  gtk_label_set_text(GTK_LABEL(data), message);

  gtk_widget_show_all(dbox);
}

void
msg_ok_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}

void
on_msg_dbox_destroy                    (GtkObject       *object,
                                        gpointer         user_data)
{
  if(b)
    unhalt(); // free running
}
