
#ifndef COLORS_CB_H
#define COLORS_CB_H

#include <gtk/gtk.h>

int display_ncolors_dbox();
int display_dlcolors_dbox();

void
on_radiobutton24_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton25_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton26_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton27_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button29_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button30_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry5_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry6_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button31_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button32_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_ncolors_dbox_destroy                (GtkObject       *object,
                                        gpointer         user_data);

void
on_dlcolors_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data);

#endif /* !COLORS_CB_H */
