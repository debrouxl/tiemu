
#ifndef DBGOPT_CB_H
#define DBGOPT_CB_H

#include <gtk/gtk.h>

int display_dbgopt_dbox();


void
on_timeout_spinbutton1_changed         (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_delay_spinbutton2_changed           (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_dbgopt_ok_button_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_dbgopt_cancel_button_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_dbgopt_spinbutton1_changed          (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_dbgopt_spinbutton2_changed          (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_dbgopt_spinbutton3_changed          (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_dbgopt_dbox_destroy                 (GtkObject       *object,
                                        gpointer         user_data);

#endif /* !DBGOPT_CB_H */
