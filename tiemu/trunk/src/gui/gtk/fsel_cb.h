#include <gtk/gtk.h>


gint display_rom_fileselection();
gint display_tifile_fileselection();
gint display_tib_fileselection();
gint display_ams_fileselection();


void
on_rom_fileselection_destroy           (GtkObject       *object,
                                        gpointer         user_data);

void
on_rom_fileselection_show              (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_rom_ok_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_rom_cancel_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_tifile_fileselection_destroy        (GtkObject       *object,
                                        gpointer         user_data);

void
on_tifile_fileselection_show           (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_tifile_ok_button2_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_tifile_cancel_button2_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_tib_fileselection_destroy           (GtkObject       *object,
                                        gpointer         user_data);

void
on_tib_fileselection_show              (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_tib_ok_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_tib_cancel_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_ams_fileselection_destroy           (GtkObject       *object,
                                        gpointer         user_data);

void
on_ams_fileselection_show              (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_ams_ok_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_ams_cancel_button2_clicked          (GtkButton       *button,
                                        gpointer         user_data);
