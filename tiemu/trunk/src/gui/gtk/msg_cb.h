
#ifndef MSG_CB_H
#define MSG_CB_H

#include <gtk/gtk.h>

void msg_box (const gchar *titl, gchar *messag);
void msg_box2(const gchar *titl, gchar *messag);


void
msg_ok_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_msg_dbox_destroy                    (GtkObject       *object,
                                        gpointer         user_data);

#endif /* !MSG_CB_H */
