
#ifndef USER_CB_H
#define USER_CB_H

#include <gtk/gtk.h>

gint user1_box(const char *titl, char *messag, const char *b1);
gint user2_box(const char *titl, char *messag, const char *b1, const char *b2);
gint user3_box(const char *titl, char *messag, 
	       const char *b1, const char *b2, const char *b3);


void
msg_ok_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
msg_ok_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
user1_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
user1_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
user1_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
user2_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
user1_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
user2_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
user3_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data);

#endif /* !USER_CB_H */
