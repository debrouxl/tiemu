
#ifndef ABOUT_CB_H
#define ABOUT_CB_H

#include <gtk/gtk.h>

gint display_about_dbox(void);


void
about_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

#endif /* !ABOUT_CB_H */

