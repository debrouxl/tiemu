
#ifndef ITICK_CB_H
#define ITICK_CB_H

#include <gtk/gtk.h>

gint display_tickrate_dbox(void);
gint display_cyclerate_dbox(void);
gint display_itick_dbox(void);


void
on_entry7_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button33_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button34_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry8_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button35_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button36_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry9_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button41_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button42_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_tickrate_dbox_destroy               (GtkObject       *object,
                                        gpointer         user_data);

void
on_cyclerate_dbox_destroy              (GtkObject       *object,
                                        gpointer         user_data);

void
on_itick_dbox_destroy                  (GtkObject       *object,
                                        gpointer         user_data);

#endif /* !ITICK_CB_H */
