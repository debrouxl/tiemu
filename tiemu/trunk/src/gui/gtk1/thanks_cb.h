#include <gtk/gtk.h>

gint display_thanks_dbox();

void
on_thanks_dbox_show                    (GtkWidget       *widget,
                                        gpointer         user_data);

void
thanks_ok_button_clicked               (GtkButton       *button,
                                        gpointer         user_data);
