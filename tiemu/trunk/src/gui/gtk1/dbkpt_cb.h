#include <gtk/gtk.h>

gint display_data_bkpts_dbox();

void
on_data_bkpts_dbox_show                (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_data_bkpts_dbox_show2               (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_clist4_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist4_unselect_row                 (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_button_add_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_del_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_ok39_clicked                 (GtkButton       *button,
                                        gpointer         user_data);
void
on_data_bkpts_dbox_destroy             (GtkWidget       *widget,
                                        gpointer         user_data);
