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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "thanks_cb.h"
#include "thanks_dbox.h"
#include "support.h"
#include "utils.h"

#include "tilibs.h"
#include "struct.h"
#include "hid.h"
#include "paths.h"
#include "files.h"

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
  ti68k_unhalt();
}
