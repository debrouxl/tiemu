#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "dbgopt_cb.h"
#include "dbgopt_dbox.h"
#include "support.h"

#include "platform.h"
#include "struct.h"


static gint tmp_stack_lines;
static gint tmp_mem_lines;
static gint tmp_code_lines;

int display_dbgopt_dbox()
{
  GtkWidget *dbox;
  gpointer user_data;

  dbox = create_dbgopt_dbox();
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "spinbutton1");
  tmp_code_lines = options.code_lines;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(user_data), options.stack_lines);
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "spinbutton2");
  tmp_mem_lines = options.mem_lines;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(user_data), options.mem_lines);
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "spinbutton3");
  tmp_stack_lines = options.stack_lines;
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(user_data), options.stack_lines);

  gtk_widget_show_all(dbox);
  return 0;
}


void
on_dbgopt_ok_button_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  options.code_lines = tmp_code_lines;
  options.mem_lines = tmp_mem_lines;
  options.stack_lines = tmp_stack_lines;

  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_dbgopt_cancel_button_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_dbgopt_spinbutton1_changed          (GtkEditable     *editable,
                                        gpointer         user_data)
{
  tmp_code_lines = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (user_data));
}


void
on_dbgopt_spinbutton2_changed          (GtkEditable     *editable,
                                        gpointer         user_data)
{
  tmp_mem_lines = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (user_data));
}


void
on_dbgopt_spinbutton3_changed          (GtkEditable     *editable,
                                        gpointer         user_data)
{
  tmp_stack_lines = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (user_data));
}

void
on_dbgopt_dbox_destroy                 (GtkObject       *object,
                                        gpointer         user_data)
{
  //unhalt();
}

/* */
