#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>

#include "help_cb.h"
#include "help_dbox.h"
#include "support.h"
#include "utils.h"

#include "intl.h"

#include "struct.h"
#include "hid.h"
#include "files.h"

gint display_help_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *text;
  gchar *file;

  dbox = create_help_dbox();
  text = lookup_widget(dbox, "text1");

  file = g_strconcat(inst_paths.help_dir, _("help_en_us"), NULL);

  load_text(text, file);

  g_free(file);

  gtk_widget_show_all(dbox);

  return 0;
}

void
help_ok_button_clicked                     (GtkButton       *button,
					    gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "help_dbox"));
  unhalt();
}
