#include <gtk/gtk.h>

gint display_skinlist_dbox();

void
on_skin_dbox_destroy                   (GtkObject       *object,
                                        gpointer         user_data);

void
on_skin_clist1_select_row              (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_skin_clist1_unselect_row            (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_skin_clist1_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_skin_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_skin_button7_clicked                     (GtkButton       *button,
                                        gpointer         user_data);
