#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>

#include "startup_cb.h"
#include "startup_dbox.h"
#include "support.h"

#include "tilibs.h"
#include "platform.h"
#include "struct.h"
#include "paths.h"
#include "version.h"
#include "rcfile.h"
#include "files.h"
#include "ticalc.h"
#include "main.h"

gint display_startup_dbox(void)
{
  gchar str[MAXCHARS];
  FILE *f;
  gchar *buffer;
  char *home_dir;

  /* Display the startup box */
#if defined(__LINUX__)
  get_home_path(&home_dir);
  buffer = g_strconcat(home_dir, DIR_SEPARATOR, ST_FILE, NULL);
  g_free(home_dir);
#elif defined(__WIN32__)
  buffer = g_strconcat(inst_paths.base_dir, DIR_SEPARATOR, ST_FILE, NULL);
#endif

  /* Try to access the file */
  if(access(buffer, F_OK) != 0 )
    {
      // file not found
      f = fopen(buffer, "wt");
      if(f == NULL)
	{
	  fprintf(stderr, "Unable to open/create this file: <%s>.\n", buffer);
	  g_free(buffer);
	  return -1;
	}
      fprintf(f, "%s\n", GTKTIEMU_VERSION);
      fclose(f);
      gtk_widget_show_all(create_startup_dbox());
    }
  else
    {
      // file found
      f = fopen(buffer, "rt");
      if(f == NULL)
	{
	  fprintf(stderr, "Unable to open/create this file: <%s>.\n", buffer);
	  return -1;
	}
      fgets(str, MAXCHARS, f);
      //fprintf(stderr, "<%s>\n", str);
      if(strcmp(GTKTIEMU_VERSION, str) > 0)
	{
	  fclose(f);
	  f = fopen(buffer, "wt");
	  if(f == NULL)
	    {
	      fprintf(stderr, "Unable to open/create this file: <%s>.\n", buffer);
	      g_free(buffer);
	      return -1;
	    }
	  fprintf(f, "%s\n", GTKTIEMU_VERSION);	
	  gtk_widget_show_all(create_startup_dbox());
	}
      fclose(f);
    }

  g_free(buffer);
  return 0;
}

void
on_startup_dbox_show                   (GtkWidget       *widget,
                                        gpointer         user_data)
{
#if 0 /* FUCKED */
  FILE *fd;
  gchar buffer[MAXCHARS];
  GdkFont *fixed_font;
  GtkWidget *text;

  text = GTK_WIDGET(user_data);
  gtk_text_freeze(GTK_TEXT (text));
  gtk_editable_delete_text(GTK_EDITABLE(text), 0, -1);

  /* Create the base filename */
  strcpy(buffer, inst_paths.base_dir);  // retrieve base path
  strcat(buffer, SHARE_DIR);          // 
  strcat(buffer, DIR_SEPARATOR);
#if defined(__LINUX__)
  strcat(buffer, "ChangeLog");
#elif defined(__WIN32__)
  strcat(buffer, "ChangeLog.txt");
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
  gtk_text_thaw(GTK_TEXT (text));
#endif /* 0 */
}


void
startup_ok_button_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
  unhalt();
}

