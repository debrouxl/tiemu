#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "user_cb.h"
#include "user_dbox.h"
#include "support.h"

static gint ret_val = 0;

/* Dialog box, type 1 */
gint user1_box(const char *title, char *message, const char *button1)
{
  GtkWidget *dbox;
  gpointer data;
  ret_val = 0;

  dbox = create_user1_dbox();
  
  data = gtk_object_get_data(GTK_OBJECT(dbox), "user1_dbox");
  gtk_window_set_title (GTK_WINDOW(data), title);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label2");
  gtk_label_set_text(GTK_LABEL(data), message);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label3");
  gtk_label_set_text(GTK_LABEL(data), button1);
  
  gtk_widget_show_all(dbox);

  while(!ret_val)
    {
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }
  gtk_widget_destroy(dbox);
  
  return ret_val;  
}

/* This a general function used to display a message with 2 buttons */
gint user2_box(const char *title, char *message, 
	       const char *button1, const char *button2)
{
  GtkWidget *dbox;
  gpointer data;
  ret_val = 0;

  dbox = create_user2_dbox();
  
  data = gtk_object_get_data(GTK_OBJECT(dbox), "user2_dbox");
  gtk_window_set_title (GTK_WINDOW(data), title);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label3");
  gtk_label_set_text(GTK_LABEL(data), message);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label4");
  gtk_label_set_text(GTK_LABEL(data), button1);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label5");
  gtk_label_set_text(GTK_LABEL(data), button2);
  
  gtk_widget_show_all(dbox);

  while(!ret_val)
    {
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }
  gtk_widget_destroy(dbox);
  
  return ret_val;  
}

/* This a general function used to display a message with 3 buttons */
gint user3_box(const char *title, char *message, 
	       const char *button1, const char *button2, const char *button3)
{
  GtkWidget *dbox;
  gpointer data;
  ret_val = 0;

  dbox = create_user3_dbox();
  
  data = gtk_object_get_data(GTK_OBJECT(dbox), "user3_dbox");
  gtk_window_set_title (GTK_WINDOW(data), title);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label6");
  gtk_label_set_text(GTK_LABEL(data), message);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label7");
  gtk_label_set_text(GTK_LABEL(data), button1);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label8");
  gtk_label_set_text(GTK_LABEL(data), button2);

  data = gtk_object_get_data(GTK_OBJECT(dbox), "label9");
  gtk_label_set_text(GTK_LABEL(data), button3);
  
  gtk_widget_show_all(dbox);

  while(!ret_val)
    {
      while( gtk_events_pending() ) { gtk_main_iteration(); }
    }
  gtk_widget_destroy(dbox);
  
  return ret_val;  
}

/* First button clicked */
void
user1_ok_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  ret_val = 1;
  //gtk_widget_destroy(GTK_WIDGET(user_data));
}



void
user2_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  ret_val = 2;
  //gtk_widget_destroy(GTK_WIDGET(user_data));
}

void
user3_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  ret_val = 3;
  //gtk_widget_destroy(GTK_WIDGET(user_data));
}
