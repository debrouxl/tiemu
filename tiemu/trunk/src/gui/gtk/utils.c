/*  tilp - link program for TI calculators
 *  Copyright (C) 1999-2001  Romain Lievin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <gtk/gtk.h>

#include "platform.h"
#include "struct.h"
#include "paths.h"

/* Open the pixmap stored on the file 'filename' and put it in the 'pixmap' 
variable according to the 'mask' variable */
void open_xpm (char *filename, GtkWidget *parent, 
	       GdkPixmap **pixmap, GdkBitmap **mask)
{
  gchar *s;
  GtkStyle *style;

  style = gtk_widget_get_style (parent);
#if defined(__LINUX__)
  s = g_strconcat(inst_paths.base_dir, SHARE_DIR "/pixmaps/", filename, NULL);
#elif defined(__WIN32__)
  s = g_strconcat(inst_paths.base_dir, SHARE_DIR "\\pixmaps\\", filename, NULL);
#endif
  
  if (access (s, F_OK) != 0) 
    {
      g_error("Error: unable to access this file: '%s'\n", s);
      g_free(s);
      exit (1);
    }
  *pixmap = gdk_pixmap_create_from_xpm (parent->window, mask, 
					&style->bg[GTK_STATE_NORMAL], s);
  if (*pixmap == NULL) 
    {
      fprintf(stderr, "Error: unable to load this file into a pixmap: '%s'\n", s);
      g_free(s);
      exit (1);
    }
  g_free(s);
}


/* Load a text file into a TextView */
void
load_text (GtkWidget *text, gchar *file)
{
  struct stat stbuf;
  FILE *fp;
  int len;
  gchar *buffer;
  GtkTextBuffer *txtbuf;

  if (access(file, F_OK) == 0)
    {
      if (stat(file, &stbuf) != -1)
	{
	  len = stbuf.st_size;
	  len -= 2;
	  
	  if ((fp = fopen(file, "r")) != NULL)
	    {
	      buffer = (gchar *) malloc(len * sizeof(gchar));
	      memset(buffer, 0, len);
	      len = fread(buffer, 1, len, fp);
	      fclose(fp);
	      
	      txtbuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
	      gtk_text_buffer_set_text(txtbuf, buffer, len);
	      
	      free(buffer);  
	    }
	}
    }
}


