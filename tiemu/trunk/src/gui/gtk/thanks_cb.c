#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "thanks_cb.h"
#include "thanks_dbox.h"
#include "support.h"
#include "utils.h"

#include "tilibs.h"
//#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "paths.h"
#include "files.h"
#include "main.h"

gint display_thanks_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *text;
  gchar *file;

  dbox = create_thanks_dbox();
  text = lookup_widget(dbox, "text1");

#ifdef __LINUX__
  file = g_strconcat(inst_paths.base_dir, SHARE_DIR, DIR_SEPARATOR,
		     "THANKS", NULL);
#elif defined(__WIN32__)
  file = g_strconcat(inst_paths.base_dir, SHARE_DIR, DIR_SEPARATOR,
		     "Thanks.txt", NULL);
#endif  

  load_text(text, file);

  g_free(file);

  gtk_widget_show_all(dbox);

  return 0;
}


void
thanks_ok_button_clicked                     (GtkButton       *button,
					      gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "thanks_dbox"));
  unhalt();
}
