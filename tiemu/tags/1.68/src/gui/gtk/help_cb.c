#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>

#include "help_cb.h"
#include "help_dbox.h"
#include "support.h"

#include "intl.h"

#include "platform.h"
#include "struct.h"
#include "ticalc.h"
#include "files.h"
#include "main.h"

gint display_help_dbox()
{
  GtkWidget *dbox;
  gpointer user_data;
  FILE *fd;
  gchar buffer[MAXCHARS];
  GdkFont *fixed_font;
  GtkWidget *text;

  dbox = create_help_dbox();
  
  /* Set text */
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "text1");
  text = GTK_WIDGET(user_data);
  gtk_text_freeze(GTK_TEXT (text));
  gtk_editable_delete_text(GTK_EDITABLE(text), 0, -1);

  /* Create the base filename */
  strcpy(buffer, inst_paths.help_dir);
  strcat(buffer, _("help_en_us"));
  
#ifdef __WIN32__
  //strcat(buffer, ".txt");
#endif

  /* Load the default help file if the previous can not be found */
  if( access(buffer, F_OK) != 0 )
    {
      strcpy(buffer, inst_paths.help_dir);
      strcat(buffer, "help_en_us");
#if defined(__WIN32__)
      strcat(buffer, ".txt");
#endif
    }
  
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
  gtk_text_thaw(GTK_TEXT (text));

  gtk_widget_show_all(dbox);
  return 0;
}

void
help_ok_button_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  unhalt();
}
