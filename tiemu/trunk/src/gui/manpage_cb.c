#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <gtk/gtk.h>

#include "manpage_cb.h"
#include "manpage_dbox.h"
#include "support.h"
#include "utils.h"

#include "struct.h"
#include "hid.h"
#include "files.h"

gint display_manpage_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *text;
  gchar *file;

  dbox = create_manpage_dbox();
  
  text = lookup_widget(dbox, "text1");
  file = g_strconcat(inst_paths.manpage_dir, "manpage.txt", NULL);

  load_text(text, file);

  g_free(file);

  gtk_widget_show_all(dbox);

  return 0;
}


void
manpage_ok_button_clicked                     (GtkButton       *button,
					       gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "manpage_dbox"));
  unhalt();
}
