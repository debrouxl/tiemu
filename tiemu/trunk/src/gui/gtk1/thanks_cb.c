#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "thanks_cb.h"
#include "thanks_dbox.h"
#include "support.h"

#include "tilibs.h"
#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "paths.h"
#include "files.h"
#include "main.h"

gint display_thanks_dbox()
{
  GtkWidget *dbox;
  gpointer user_data;
  FILE *fd;
  gchar *buffer;
  GdkFont *fixed_font;
  GtkWidget *text;

  dbox = create_thanks_dbox();
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "text1");
  text = GTK_WIDGET(user_data);
  gtk_editable_delete_text(GTK_EDITABLE(text), 0, -1);

  /* Create the base filename */
#if defined(__LINUX__)
  buffer = g_strconcat(inst_paths.base_dir, SHARE_DIR, DIR_SEPARATOR,
		       "THANKS", NULL);
#elif defined(__WIN32__)
  buffer = g_strconcat(inst_paths.base_dir, SHARE_DIR, DIR_SEPARATOR,
		       "Thanks.txt", NULL);
#endif  

  /* Try to access the file */
  if(access(buffer, F_OK) == 0 )
    {
#if defined(__LINUX__)
      fixed_font = gdk_font_load ("-misc-clean-medium-r-*-*-*-140-*-*-*-*-*-*");
#elif defined(__WIN32__)
      fixed_font = gdk_font_load ("-adobe-courier-medium-r-normal--12-120-75-75-p-70-iso8859-1");
#endif
      if( (fd=fopen (buffer, "r")) != NULL)
	{
	  memset (buffer, 0, sizeof(buffer));
	  while(fread (buffer, 1, sizeof(buffer)-1, fd))
	    {
	      process_buffer(buffer);
	      gtk_text_insert (GTK_TEXT (text), fixed_font, NULL, NULL, buffer, strlen (buffer));
	      memset (buffer, 0, sizeof(buffer));
	    }
	  fclose (fd);
	}
    }
  
  g_free(buffer);
  gtk_widget_show_all(dbox);
  return 0;
}


void
thanks_ok_button_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  unhalt();
}
