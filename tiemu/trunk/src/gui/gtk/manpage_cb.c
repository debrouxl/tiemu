#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "manpage_cb.h"
#include "manpage_dbox.h"
#include "support.h"

#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "files.h"
#include "main.h"

gint display_manpage_dbox()
{
  GtkWidget *dbox;
  gpointer user_data;
  FILE *fd;
  gchar *buffer;
  GdkFont *fixed_font;
  GtkWidget *text;

  dbox = create_manpage_dbox();
  
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "text1");
  text = GTK_WIDGET(user_data);
  gtk_editable_delete_text(GTK_EDITABLE(text), 0, -1);
  
  /* Create the base filename */
  //fprintf(stderr, "before: <%s>\n", inst_paths.manpage_dir); 
  buffer = g_strconcat(inst_paths.manpage_dir, "manpage.txt", NULL);
  
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

  gtk_widget_show_all(dbox);
  return 0;
}


void
manpage_ok_button_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  unhalt();
}
