
#ifndef DUMP_CB_H
#define DUMP_CB_H

#include <gtk/gtk.h>

gint display_ramdump_fileselection();
gint display_romdump_fileselection();
gint display_wait_conversion_dbox();

void
on_ramdump_fileselection_destroy       (GtkObject       *object,
                                        gpointer         user_data);

void
on_ramd_ok_button2_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_ramd_cancel_button2_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_romd_fileselection_destroy          (GtkObject       *object,
                                        gpointer         user_data);

void
on_romd_ok_button2_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_romd_cancel_button2_clicked         (GtkButton       *button,
                                        gpointer         user_data);

#endif /* !DUMP_CB_H */
