
#ifndef STATE_CB_H
#define STATE_CB_H

#include <gtk/gtk.h>

gint display_fs_load_state(void);
gint display_fs_save_state(void);

void
load_state_on_ok_button1_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
load_state_on_cancel_button1_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
save_state_on_ok_button2_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
save_state_on_cancel_button2_pressed   (GtkButton       *button,
                                        gpointer         user_data);

void
load_state_on_ok_button1_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
load_state_on_cancel_button1_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_fs_load_state_destroy               (GtkObject       *object,
                                        gpointer         user_data);

void
on_fs_save_state_destroy               (GtkObject       *object,
                                        gpointer         user_data);

#endif /* !STATE_CB_H */
