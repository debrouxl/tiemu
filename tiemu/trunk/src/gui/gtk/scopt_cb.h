
#ifndef SCOPT_CB_H
#define SCOPT_CB_H

#include <gtk/gtk.h>

gint display_scopt_dbox(void);


void
on_scopt_radiobutton18_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_scopt_radiobutton19_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_scopt_radiobutton20_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_scopt_radiobutton22_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_scopt_radiobutton23_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_scopt_radiobutton24_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_scopt_radiobutton25_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_scopt_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_scopt_cancel_button_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_scopt_dbox_destroy                  (GtkObject       *object,
                                        gpointer         user_data);

void
on_scopt_radiobutton21_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);


#endif /* !SCOPT_CB_H */
