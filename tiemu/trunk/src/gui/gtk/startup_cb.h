
#ifndef STARTUP_CB_H
#define STARTUP_CB_H

#include <gtk/gtk.h>

gint
display_startup_dbox (void);

void
startup_ok_button_clicked              (GtkButton       *button,
                                        gpointer         user_data);

#endif /* !STARTUP_CB_H */
