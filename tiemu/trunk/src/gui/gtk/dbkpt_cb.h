
#ifndef DBKPT_CB_H
#define DBKPT_CB_H

#include <gtk/gtk.h>

gint display_data_bkpts_dbox(void);


void
on_button_add_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_del_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_ok39_clicked                 (GtkButton       *button,
                                        gpointer         user_data);
void
on_data_bkpts_dbox_destroy             (GtkWidget       *widget,
                                        gpointer         user_data);

#endif /* !DBKPT_CB_H */
