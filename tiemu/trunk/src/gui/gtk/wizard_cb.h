
#ifndef WIZARD_CB_H
#define WIZARD_CB_H

#include <gtk/gtk.h>

gint display_flash_fileselection(void);
gint display_romfile_fileselection(void);

void
on_flashfile_fileselection_destroy     (GtkObject       *object,
                                        gpointer         user_data);

void
on_flashfile_ok_button2_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_flashfile_cancel_button2_clicked    (GtkButton       *button,
                                        gpointer         user_data);

void
step1_on_radiobutton1_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
step2_on_radiobutton2_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
step3_on_radiobutton3_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
step1_b1_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
step1_b2_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
step1_b3_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
msg1_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
step3_b1_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
step3_b2_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
step3_b3_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
wait_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_romfile_fileselection_destroy       (GtkObject       *object,
                                        gpointer         user_data);

void
on_romfile_cancel_button2_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_romfile_ok_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_romfile_cancel_button2_clicked      (GtkButton       *button,
                                        gpointer         user_data);

#endif /* !WIZARD_CB_H */
