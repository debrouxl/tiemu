/* Hey EMACS -*- linux-c -*- */
/* $Id: calc.c 2279 2006-11-22 19:47:12Z kevinkofler $ */

/*  TiEmu - Tiemu Is an EMUlator
 *
 *  Copyright (c) 2006, Romain Liévin
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif				/*  */

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>

#include "support.h"
#include "tilibs.h"
#include "ti68k_int.h"
#include "struct.h"
#include "calc.h"
#include "engine.h"
#include "fs_misc.h"

void dnd_init(void)
{
	extern GtkWidget *main_wnd;

	gtk_drag_dest_set (main_wnd, 0, NULL, 0, 0);
}

GLADE_CB gboolean
on_calc_wnd_drag_drop              (GtkWidget       *widget,
                                        GdkDragContext  *context,
                                        gint             x,
                                        gint             y,
                                        guint            time,
                                        gpointer         user_data)
{
	if (context->targets)
    {
      gtk_drag_get_data (widget, context, 
			 GDK_POINTER_TO_ATOM (context->targets->data), 
			 time);
      return TRUE;
    }

	return FALSE;
}

GLADE_CB void
on_calc_wnd_drag_data_received     (GtkWidget       *widget,
                                        GdkDragContext  *context,
                                        gint             x,
                                        gint             y,
                                        GtkSelectionData *data,
                                        guint            info,
                                        guint            time,
                                        gpointer         user_data)
{
	if ((data->length >= 0) && (data->format == 8))
    {
		gchar *uri = data->data;
		//g_print ("Received \"%s\"\n", uri);

		// is this an URI?
		if (!g_ascii_strncasecmp(uri, "file://", 7)) 
		{
			GError *error = NULL;
			gchar *fn;
			gchar *tok;

			// convert URI to filename
			fn = g_filename_from_uri(uri, NULL, &error);
			if(fn == NULL)
			{
				fprintf(stderr, "DnD error: %s\n", error ? error->message : "g_filename_from_uri error");
				return;
			}

			// tails string
			if ((tok = strchr(fn, '\r')) || (tok = strchr(fn, '\n')))
				*tok = '\0';

			// debug
			printf("fn = <%s>\n", fn);

			// check for file type
			if(!strcmp(tifiles_fext_get(fn), "skn"))
			{
				// Load new skin (fs_misc.c)
				g_free(options.skin_file);
				options.skin_file = g_strdup(fn);
    
				hid_change_skin(options.skin_file);
			}
			else if(!strcmp(tifiles_fext_get(fn), "rom") || ti68k_is_a_rom_file(fn) || ti68k_is_a_tib_file(fn))
			{
				// Add rom to wizard
				engine_stop();
				engine_start();
			}
			else if(tifiles_file_is_ti(fn) && (tifiles_calc_is_ti9x(tifiles_file_get_model(fn)) ||
			tifiles_file_is_tigroup(fn))) 
			{
				engine_stop();
				send_file(fn);
				engine_start();
			}
		}

      gtk_drag_finish (context, TRUE, FALSE, time);
      return;
    }
  
	gtk_drag_finish (context, FALSE, FALSE, time);
}
