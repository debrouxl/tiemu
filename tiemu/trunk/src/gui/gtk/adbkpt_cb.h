
#ifndef ADBKPT_CB_H
#define ADBKPT_CB_H

#include <gtk/gtk.h>

int display_add_data_bkpt_dbox();


void
on_radiobutton19_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton20_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton21_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton22_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton23_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_entry4_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_combo_entry1_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry3_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry2_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button_ok_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_cancel_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry2_activate                     (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_add_data_bkpt_dbox_destroy          (GtkObject       *object,
                                        gpointer         user_data);

#endif /* !ADBKPT_CB_H */
