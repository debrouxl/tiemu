/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "dbg_bkpts.h"
#include "dbg_code.h"
#include "dbg_cause.h"
#include "dbg_data.h"
#include "dbg_mem.h"
#include "dbg_regs.h"
#include "dbg_vectors.h"
#include "dbg_pclog.h"
#include "ti68k_int.h"
#include "struct.h"
#include "dbg_all.h"
#include "support.h"

DbgOptions options3;
DbgWidgets dbgw = { 0 };

void preload_gtk_debugger(void)
{
	//create_dbgregs_window();
	//create_dbgcode_window();
	//create_dbgmem_window();
	//create_dbgbkpts_window();
	//create_dbgpclog_window();
}

int enter_gtk_debugger(int context)
{
    // show breakpoint source
    switch(context)
    {
    case DBG_TRACE:
        break;
    case DBG_BREAK:
        display_dbgcause_dbox();
        break;
    }

    // refresh debugger windows (open debugger, if not already opened)
	dbgw.regs = display_dbgregs_window();
	dbgw.mem  = display_dbgmem_window();
	dbgw.bkpts = display_dbgbkpts_window();
    dbgw.pclog = display_dbgpclog_window();
	dbgw.code = display_dbgcode_window();	// the last has focus

	return 0;
}

void set_other_windows_sensitivity(int state)
{
    gtk_widget_set_sensitive(dbgw.regs, state);
    gtk_widget_set_sensitive(dbgw.bkpts, state);
    gtk_widget_set_sensitive(dbgw.mem, state);
    gtk_widget_set_sensitive(dbgw.pclog, state);
}

// callbacks from dbg_code.c

GLADE_CB void
on_registers1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.regs);
  	else
        display_dbgregs_window();
}


GLADE_CB void
on_breakpoints1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.bkpts);
  	else
        display_dbgbkpts_window();
}


GLADE_CB void
on_memory1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.mem);
  	else
        display_dbgmem_window();
}

GLADE_CB void
on_pc_log1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.pclog);
  	else
        display_dbgpclog_window();
}


GLADE_CB void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(dbgw.regs)
        gtk_widget_hide(dbgw.regs);
    if(dbgw.bkpts)
        gtk_widget_hide(dbgw.bkpts);
    if(dbgw.mem)
        gtk_widget_hide(dbgw.mem);
	if(dbgw.pclog)
        gtk_widget_hide(dbgw.pclog);
	if(dbgw.code)
	    gtk_widget_hide(dbgw.code);
}

GLADE_CB void
on_minimize_all1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_window_iconify(GTK_WINDOW(dbgw.regs));
    gtk_window_iconify(GTK_WINDOW(dbgw.bkpts));
    gtk_window_iconify(GTK_WINDOW(dbgw.mem));
    gtk_window_iconify(GTK_WINDOW(dbgw.pclog));
}


GLADE_CB void
on_maximize_all1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_window_deiconify(GTK_WINDOW(dbgw.regs));
    gtk_window_deiconify(GTK_WINDOW(dbgw.bkpts));
    gtk_window_deiconify(GTK_WINDOW(dbgw.mem));
    gtk_window_deiconify(GTK_WINDOW(dbgw.pclog));
}

