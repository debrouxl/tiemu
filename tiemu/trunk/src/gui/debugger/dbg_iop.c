/* Hey EMACS -*- linux-c -*- */
/* $Id: dbg_regs.c 1615 2005-08-03 12:44:13Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "intl.h"
#include "paths.h"
#include "support.h"
#include "ti68k_int.h"
#include "struct.h"
#include "dbg_all.h"

static GladeXML *xml = NULL;
static GtkWidget *wnd = NULL;

/*
	Display registers window
*/
GtkWidget* dbgiop_create_window(void)
{
	GtkWidget *dbox;
    GtkWidget *data;	
	
	xml = glade_xml_new
		(tilp_paths_build_glade("dbg_ioports-2.glade"), "dbgioports_window",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "dbgioports_window");
#ifdef WND_TRANSIENT
	gtk_window_set_transient_for(GTK_WINDOW(dbox), GTK_WINDOW(main_wnd));
#endif

	data = glade_xml_get_widget(xml, "treeview1");
    //store = ctree_create(data);
	//ctree_populate(store);

	gtk_tree_view_expand_all(GTK_TREE_VIEW(data));

	return wnd = dbox;
}

GtkWidget* dbgiop_display_window(void)
{
#ifdef WND_STATE
	if(!options3.iop.minimized)
	{
		gtk_window_resize(GTK_WINDOW(wnd), options3.iop.rect.w, options3.iop.rect.h);
		gtk_window_move(GTK_WINDOW(wnd), options3.iop.rect.x, options3.iop.rect.y);
	}
	else
		gtk_window_iconify(GTK_WINDOW(wnd));
#endif
    
	//ctree_refresh(store);

	if(!GTK_WIDGET_VISIBLE(dbgw.regs) && !options3.iop.closed)
		gtk_widget_show(wnd);

	return wnd;
}

void dbgiop_refresh_window(void)
{
	if(!options3.iop.closed)
	{
		//ctree_refresh(store);
	}
}
