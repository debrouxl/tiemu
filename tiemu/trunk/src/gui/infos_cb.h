
#ifndef INFOS_CB_H
#define INFOS_CB_H

#include <gtk/gtk.h>

gint display_infos_dbox(void);


void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_infos_dbox_destroy                  (GtkObject       *object,
                                        gpointer         user_data);

#endif /* !INFOS_CB_H */
