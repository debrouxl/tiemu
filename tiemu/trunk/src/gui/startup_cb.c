#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>
#include <unistd.h>

#include "startup_cb.h"
#include "startup_dbox.h"
#include "support.h"
#include "utils.h"

#include "tilibs.h"
#include "struct.h"
#include "paths.h"
#include "version.h"
#include "rcfile.h"
#include "files.h"
#include "hid.h"


static void
show_startup_dbox (void)
{
  GtkWidget *dbox;
  GtkWidget *text;
  gchar *file;

  dbox = create_startup_dbox();
  text = lookup_widget(dbox, "text1");

#ifdef __LINUX__
  file = g_strconcat(inst_paths.base_dir, SHARE_DIR, DIR_SEPARATOR, "ChangeLog", NULL);
#elif defined(__WIN32__)
  file = g_strconcat(inst_paths.base_dir, SHARE_DIR, DIR_SEPARATOR, "ChangeLog.txt", NULL);
#endif

  load_text(text, file);

  g_free(file);

  gtk_widget_show_all(dbox);
}


gint
display_startup_dbox (void)
{
  gchar str[MAXCHARS];
  FILE *f;
  gchar *buffer;
  char *home_dir;

  /* Display the startup box */
  
  buffer = g_strconcat(inst_paths.home_dir, DIR_SEPARATOR, ST_FILE, NULL);

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
      fprintf(f, "%s\n", TIEMU_VERSION);
      fclose(f);
      show_startup_dbox();
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
      if(strcmp(TIEMU_VERSION, str) > 0)
	{
	  fclose(f);
	  f = fopen(buffer, "wt");
	  if(f == NULL)
	    {
	      fprintf(stderr, "Unable to open/create this file: <%s>.\n", buffer);
	      g_free(buffer);
	      return -1;
	    }
	  fprintf(f, "%s\n", TIEMU_VERSION);	
	  show_startup_dbox();
	}
      fclose(f);
    }

  g_free(buffer);
  return 0;
}


void
startup_ok_button_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "startup_dbox"));
  unhalt();
}

