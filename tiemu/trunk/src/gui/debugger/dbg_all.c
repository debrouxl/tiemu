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
DbgStates dbgs = { 0 };

int dbg_on = 0;

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

    dbg_on = !0;

	return 0;
}

void set_other_windows_sensitivity(int state)
{
    gtk_widget_set_sensitive(dbgw.regs, state);
    gtk_widget_set_sensitive(dbgw.bkpts, state);
    gtk_widget_set_sensitive(dbgw.mem, state);
    gtk_widget_set_sensitive(dbgw.pclog, state);
}

// callbacks from dbg_code.c (window menu)

GLADE_CB void
on_registers1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    //if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    if(dbgs.regs)
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

    dbg_on = 0;
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

// callbacks from dbg_regs.c

GLADE_CB gboolean
on_dbgregs_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);
	return TRUE;
}

GLADE_CB void
on_dbgregs_window_hide                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.regs = 0;
    gtk_window_get_size(GTK_WINDOW(widget), &options3.regs.w, &options3.regs.h);
    gtk_window_get_position(GTK_WINDOW(widget), &options3.regs.x, &options3.regs.y);
}

GLADE_CB void
on_dbgregs_window_show                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.regs = !0;
}

// callbacks from dbg_pclog.c
GLADE_CB gboolean
on_dbgpclog_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);    
    return TRUE;
}

GLADE_CB void
on_dbgpclog_window_hide                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.pclog = 0;
    gtk_window_get_size(GTK_WINDOW(widget), &options3.pclog.w, &options3.pclog.h);
    gtk_window_get_position(GTK_WINDOW(widget), &options3.pclog.x, &options3.pclog.y);
}

GLADE_CB void
on_dbgpclog_window_show                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.pclog = !0;
}

// callbacks from dbg_mem.c

GLADE_CB gboolean
on_dbgmem_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);    
    return TRUE;
}

GLADE_CB void
on_dbgmem_window_hide                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.mem = 0;
    gtk_window_get_size(GTK_WINDOW(widget), &options3.mem.w, &options3.mem.h);
    gtk_window_get_position(GTK_WINDOW(widget), &options3.mem.x, &options3.mem.y);
}

GLADE_CB void
on_dbgmem_window_show                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.mem = !0;
}

// callbacks from dbg_code.c

GLADE_CB gboolean
on_dbgcode_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);
    return TRUE;
}

GLADE_CB void
on_dbgcode_window_hide                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.code = 0;
    gtk_window_get_size(GTK_WINDOW(widget), &options3.code.w, &options3.code.h);
    gtk_window_get_position(GTK_WINDOW(widget), &options3.code.x, &options3.code.y);
    
    // Closing the debugger starts the emulator
    on_quit1_activate(NULL, NULL);
    bkpts.mode = bkpts.type = bkpts.id = 0;
    ti68k_engine_unhalt();
}

GLADE_CB void
on_dbgcode_window_show                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.code = !0;
}

// callbacks from dbg_bkpts.c

GLADE_CB gboolean
on_dbgbkpts_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);
    return TRUE;
}

GLADE_CB void
on_dbgbkpts_window_hide                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.bkpts = 0;
    gtk_window_get_size(GTK_WINDOW(widget), &options3.bkpts.w, &options3.bkpts.h);
    gtk_window_get_position(GTK_WINDOW(widget), &options3.bkpts.x, &options3.bkpts.y);
}

GLADE_CB void
on_dbgbkpts_window_show                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.bkpts = !0;
}