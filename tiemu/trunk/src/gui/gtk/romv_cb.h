
#ifndef ROMV_CB_H
#define ROMV_CB_H

#include <gtk/gtk.h>

gint display_romversion_dbox();

void
on_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button7_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_romversion_dbox_destroy             (GtkObject       *object,
                                        gpointer         user_data);

void
on_romversion_dbox_destroy             (GtkObject       *object,
                                        gpointer         user_data);

gboolean
on_clist1_button_press_event           (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);


gboolean
on_romv_clist1_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

#endif /* !ROMV_CB_H */
