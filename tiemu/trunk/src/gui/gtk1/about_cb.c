#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>

#include "about_cb.h"
#include "about_dbox.h"
#include "support.h"

#include "platform.h"
#include "struct.h"
#include "version.h"
#include "img/fmt.h"
#include "ticalc.h"
#include "paths.h"
#include "files.h"
#include "tilibs.h"
#include "main.h"

Image img = { NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0 };

gint display_about_dbox()
{
  GtkWidget *dbox;
  gpointer user_data;
  FILE *fd;
  gchar buffer[MAXCHARS];
  GdkFont *fixed_font;
  GtkWidget *text;
  gchar tempstr[MAXCHARS];

  dbox = create_about_dbox();
  
  /* Set text */
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "text1");
  text = GTK_WIDGET(user_data);
  gtk_text_freeze(GTK_TEXT (text));
  gtk_editable_delete_text(GTK_EDITABLE(text), 0, -1);

  strcpy(buffer, inst_paths.base_dir);  // retrieve base path
  strcat(buffer, SHARE_DIR);          //
  strcat(buffer, DIR_SEPARATOR);
#if defined(__LINUX__)
  strcat(buffer, "COPYING");
#elif defined(__WIN32__)
  strcat(buffer, "Copying.txt");
#endif

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

  /* Set label */
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "label5");
#ifdef HAVE_LIBTIFFEP
  g_snprintf(tempstr, sizeof(tempstr), 
	     "* TILP version %s (cable=%s, tiffep=%s)", 
	     GTKTIEMU_VERSION,
	     ticable_get_version(),
	     tiffep_get_version());
#else
  g_snprintf(tempstr, sizeof(tempstr),
	     "* TILP version %s (%s %s)", 
	     GTKTIEMU_VERSION,
	     ticable_get_version(),
	     "NA");
#endif
  gtk_label_set_text(GTK_LABEL(user_data), tempstr);

  /* Set pixmap */
  user_data = gtk_object_get_data(GTK_OBJECT(dbox), "pixmap2");
  #if 0
  /* buggy code -- avoid segfault ... */
  GtkWidget *tmp_pixmapwid = GTK_WIDGET(user_data);
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle *style;
  FILE *f;
  gchar buffer[MAXCHARS];

  return;

  /* Create the base filename */
  strcpy(buffer, inst_paths.base_dir);  // retrieve base path
  strcat(buffer, SHARE_DIR);          //
  strcat(buffer, DIR_SEPARATOR);
#if defined(__LINUX__)
  strcat(buffer, "logo.jpg");
#elif defined(__WIN32__)
  strcat(buffer, "logo.jpg");
#endif
  DISPLAY("logo: <%s>\n", buffer);
  /* Try to access the file */
  if(access(buffer, F_OK) == 0 )
    {
      /* Open and load file */
      f = fopen(buffer, "rb");
      if(f == NULL)
	g_error("Unable to open the logo (logo.jpg).\n");
      img.depth = 3; // > 2 colors
      read_jpg_format(f, &img);
      fclose(f);

      convert_bytemap_to_pixmap(&img);

      style = gtk_widget_get_style(widget);
      pixmap = gdk_pixmap_create_from_xpm_d(widget->window, &mask,
					    &style->bg[GTK_STATE_NORMAL],
					    (gchar **)(img.pixmap));
      gtk_pixmap_set(GTK_PIXMAP(tmp_pixmapwid), pixmap, mask);      
    }
  else 
    g_warning("Unable to open the logo (logo.jpg).\n");
#endif

  gtk_widget_show_all(dbox);
  return 0;
}

void
about_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  //delete_image(&img);
  gtk_widget_destroy(GTK_WIDGET(user_data));
  unhalt();
}
