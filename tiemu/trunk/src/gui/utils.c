/* Hey EMACS -*- linux-c -*- */
/* $Id: hid.c 163 2004-05-13 06:40:57Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Liévin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include <unistd.h>

//#include "platform.h"
#include "struct.h"
#include "paths.h"

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


