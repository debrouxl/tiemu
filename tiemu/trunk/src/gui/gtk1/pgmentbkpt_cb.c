#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "pgmentbkpt_cb.h"
#include "pgmentbkpt_dbox.h"
#include "support.h"

#include "ticalc.h"
#include "main.h"

gint display_pgmentrybkpt_dbox (void)
{
  gtk_widget_show_all(create_pgmentrybkpt_dbox());
  return 0;
}

void
on_pgmentrybkpt_dbox_destroy           (GtkObject       *object,
                                        gpointer         user_data)
{
  unhalt();
}


void
on_button21_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(user_data);
}


void
on_button22_enter                      (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(user_data);
}


