
#ifndef ROMV_CB_H
#define ROMV_CB_H

#include <gtk/gtk.h>

gint display_romversion_dbox(void);

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

void 
on_romv_clist_selection_changed (GtkTreeSelection *sel, 
				 gpointer user_data);

#endif /* !ROMV_CB_H */
