#include <gtk/gtk.h>

gint display_skinlist_dbox(void);

void 
on_skin_clist_selection_changed (GtkTreeSelection *sel, 
				 gpointer user_data);

void
on_skin_dbox_destroy                   (GtkObject       *object,
                                        gpointer         user_data);

void
on_skin_button6_clicked                     (GtkButton       *button,
					     gpointer         user_data);

void
on_skin_button7_clicked                     (GtkButton       *button,
					     gpointer         user_data);
