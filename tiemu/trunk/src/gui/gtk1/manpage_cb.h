#include <gtk/gtk.h>

gint display_manpage_dbox();

void
on_manpage_dbox_show                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
manpage_ok_button_clicked              (GtkButton       *button,
                                        gpointer         user_data);
