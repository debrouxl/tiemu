#include <gtk/gtk.h>

gint display_startup_dbox(void);

void
on_startup_dbox_show                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
startup_ok_button_clicked              (GtkButton       *button,
                                        gpointer         user_data);
