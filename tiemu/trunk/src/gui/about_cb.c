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
#include <string.h>

#include "about_cb.h"
#include "about_dbox.h"
#include "support.h"
#include "utils.h"

#include "struct.h"
#include "version.h"
#include "hid.h"
#include "paths.h"
#include "files.h"
#include "tilibs.h"

gint display_about_dbox(void)
{
  GtkWidget *dbox;
  GtkWidget *text;
  GtkWidget *label;
  gchar *file;
  gchar buf[128];

  dbox = create_about_dbox();
  text = lookup_widget(dbox, "text1");

#ifdef __LINUX__
  file = g_strconcat(inst_paths.base_dir, SHARE_DIR, DIR_SEPARATOR, "COPYING", NULL);
#elif defined(__WIN32__)
  file = g_strconcat(inst_paths.base_dir, SHARE_DIR, DIR_SEPARATOR, "Copying.txt", NULL);
#endif

  load_text(text, file);

  g_free(file);


  /* Set label */
  label = lookup_widget(dbox, "label5");
  g_snprintf(buf, sizeof(buf),
	     "*TiEmu version %s (cable=%s, calcs=%s, files=%s)",
	     TIEMU_VERSION,
	     ticable_get_version(),
	     ticalc_get_version(),
	     tifiles_get_version());
  gtk_label_set_text(GTK_LABEL(label), buf);

  gtk_widget_show_all(dbox);

  return 0;
}

void
about_ok_button_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  //delete_image(&img);
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "about_dbox"));
  ti68k_unhalt();
}
