#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "msg_cb.h"
#include "msg_dbox.h"
#include "support.h"

//#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "main.h"

static gboolean b = TRUE;

/* Msg box with halting */
void msg_box(const gchar *title, gchar *message)
{
  GtkWidget *dbox;
  GtkWidget *w;

  b = TRUE;
  dbox = create_msg_dbox();

  gtk_window_set_title (GTK_WINDOW(dbox), title);

  w = lookup_widget(dbox, "label2");
  gtk_label_set_text(GTK_LABEL(w), message);

  gtk_widget_show_all(dbox);
}

/* Msg box without halting */
void msg_box2(const gchar *title, gchar *message)
{
  GtkWidget *dbox;
  GtkWidget *w;

  b = FALSE;
  dbox = create_msg_dbox();

  gtk_window_set_title (GTK_WINDOW(dbox), title);

  w = lookup_widget(dbox, "label2");
  gtk_label_set_text(GTK_LABEL(w), message);

  gtk_widget_show_all(dbox);
}

void
msg_ok_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "msg_dbox"));
}

void
on_msg_dbox_destroy                    (GtkObject       *object,
                                        gpointer         user_data)
{
  if(b)
    unhalt(); // free running
}
