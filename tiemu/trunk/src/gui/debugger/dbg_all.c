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

#include "ti68k_int.h"
#include "struct.h"
#include "dbg_all.h"
#include "support.h"
#include "paths.h"

DbgOptions options3;
DbgWidgets dbgw = { 0 };
DbgStates dbgs = { 0 };

int dbg_on = 0;

/* Functions applicable to the whole debugger */

void gtk_debugger_preload(void)
{
	//create_dbgregs_window();
	//create_dbgcode_window();
	//create_dbgmem_window();
	//create_dbgbkpts_window();
	//create_dbgpclog_window();
    //create_dbgstack_window();
}

int gtk_debugger_enter(int context)
{
	gchar *path;

    // show breakpoint source (modal)
    switch(context)
    {
    case DBG_TRACE:
        break;
    case DBG_BREAK:
        //dbgcause1_display_dbox();
        display_dbgcause_dbox();
        break;
    }

	// load symbols
	path = g_strconcat(inst_paths.base_dir, "romcalls.txt", NULL);
	ti68k_debug_load_symbols(path);
	g_free(path);

    // open debugger windows (if not already opened)
	dbgw.regs = dbgregs_display_window();
	dbgw.mem  = dbgmem_display_window();
	dbgw.bkpts = dbgbkpts_display_window();
    dbgw.pclog = dbgpclog_display_window();
    dbgw.stack = dbgstack_display_window();
	dbgw.code = dbgcode_display_window();	// the last has focus

    dbg_on = !0;

	return 0;
}

void gtk_debugger_refresh(void)
{
	if(dbgs.regs)
		dbgregs_refresh_window();
	if(dbgs.mem)
		dbgmem_refresh_window();
	if(dbgs.bkpts)
		dbgbkpts_refresh_window();
	if(dbgs.pclog)
		dbgpclog_refresh_window();
	if(dbgs.code)
		dbgcode_refresh_window();
    if(dbgs.stack)
		dbgstack_refresh_window();
}

void set_other_windows_sensitivity(int state)
{
    if(dbgs.regs)
        gtk_widget_set_sensitive(dbgw.regs, state);
    if(dbgs.bkpts)
        gtk_widget_set_sensitive(dbgw.bkpts, state);
    if(dbgs.mem)
        gtk_widget_set_sensitive(dbgw.mem, state);
    if(dbgs.pclog)
        gtk_widget_set_sensitive(dbgw.pclog, state);
    if(dbgs.stack)
        gtk_widget_set_sensitive(dbgw.stack, state);
}

void gtk_debugger_minimize_all(void)
{
    if(dbgs.regs)
        gtk_window_iconify(GTK_WINDOW(dbgw.regs));
    if(dbgs.bkpts)
        gtk_window_iconify(GTK_WINDOW(dbgw.bkpts));
    if(dbgs.mem)
        gtk_window_iconify(GTK_WINDOW(dbgw.mem));
    if(dbgs.pclog)
        gtk_window_iconify(GTK_WINDOW(dbgw.pclog));
    if(dbgs.code)
        gtk_window_iconify(GTK_WINDOW(dbgw.code));
    if(dbgs.stack)
        gtk_window_iconify(GTK_WINDOW(dbgw.stack));
}

void gtk_debugger_deminimize_all(void)
{
    if(dbgs.regs)
        gtk_window_deiconify(GTK_WINDOW(dbgw.regs));
    if(dbgs.bkpts)
        gtk_window_deiconify(GTK_WINDOW(dbgw.bkpts));
    if(dbgs.mem)
        gtk_window_deiconify(GTK_WINDOW(dbgw.mem));
    if(dbgs.pclog)
        gtk_window_deiconify(GTK_WINDOW(dbgw.pclog));
    if(dbgs.code)
        gtk_window_deiconify(GTK_WINDOW(dbgw.code));
    if(dbgs.stack)
        gtk_window_deiconify(GTK_WINDOW(dbgw.stack));
}

void gtk_debugger_show_all(void)
{
    if(!dbg_on)
        return;

    if(!dbgs.regs)
        gtk_widget_show(dbgw.regs);
    if(!dbgs.bkpts)
        gtk_widget_show(dbgw.bkpts);
    if(!dbgs.mem)
        gtk_widget_show(dbgw.mem);
    if(!dbgs.pclog)
        gtk_widget_show(dbgw.pclog);
    if(!dbgs.code)
        gtk_widget_show(dbgw.code);
    if(!dbgs.stack)
        gtk_widget_show(dbgw.stack);
}

void gtk_debugger_hide_all(void)
{
    if(!dbg_on)
        return;

    if(dbgs.regs)
        gtk_widget_hide(dbgw.regs);
    if(dbgs.bkpts)
        gtk_widget_hide(dbgw.bkpts);
    if(dbgs.mem)
        gtk_widget_hide(dbgw.mem);
    if(dbgs.pclog)
        gtk_widget_hide(dbgw.pclog);
    if(dbgs.code)
        gtk_widget_hide(dbgw.code);
    if(dbgs.stack)
        gtk_widget_hide(dbgw.stack);
}

/* Callbacks */

// callbacks from dbg_code.c (window menu)

GLADE_CB void
on_registers1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    //if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
    if(dbgs.regs)
        gtk_widget_hide(dbgw.regs);
  	else
        dbgregs_display_window();
}


GLADE_CB void
on_breakpoints1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.bkpts);
  	else
        dbgbkpts_display_window();
}


GLADE_CB void
on_memory1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.mem);
  	else
        dbgmem_display_window();
}

GLADE_CB void
on_pc_log1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.pclog);
  	else
        dbgpclog_display_window();
}

GLADE_CB void
on_stack_frame1_activate                    (GtkMenuItem     *menuitem,
                                             gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.stack);
  	else
        dbgstack_display_window();
}


GLADE_CB void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_widget_hide(dbgw.regs);
    gtk_widget_hide(dbgw.bkpts);
    gtk_widget_hide(dbgw.mem);
    gtk_widget_hide(dbgw.pclog);
    gtk_widget_hide(dbgw.stack);
    gtk_widget_hide(dbgw.code);

    while(gtk_events_pending()) gtk_main_iteration();

    dbg_on = 0;

    // Closing the debugger starts the emulator
    bkpts.mode = bkpts.type = bkpts.id = 0;
    ti68k_engine_start();

}

GLADE_CB void
on_minimize_all1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_widget_hide(dbgw.regs);
    gtk_widget_hide(dbgw.bkpts);
    gtk_widget_hide(dbgw.mem);
    gtk_widget_hide(dbgw.pclog);
    gtk_widget_hide(dbgw.stack);
}


GLADE_CB void
on_maximize_all1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_widget_show(dbgw.regs);
    gtk_widget_show(dbgw.bkpts);
    gtk_widget_show(dbgw.mem);
    gtk_widget_show(dbgw.pclog);
    gtk_widget_show(dbgw.stack);
}

void update_submenu(GtkWidget *widget, gpointer user_data)
{
    GtkMenuShell *shell = GTK_MENU_SHELL(widget);
    GList *list = shell->children;
    GList *elt;
    GtkCheckMenuItem *item;

    // regs
    elt = g_list_nth(list, 0);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_registers1_activate, NULL);
    gtk_check_menu_item_set_active(item, dbgs.regs);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_registers1_activate, NULL);

    // bkpts
    elt = g_list_nth(list, 1);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_breakpoints1_activate, NULL);
    gtk_check_menu_item_set_active(item, dbgs.bkpts);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_breakpoints1_activate, NULL);

    // mem
    elt = g_list_nth(list, 2);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_memory1_activate, NULL);
    gtk_check_menu_item_set_active(item, dbgs.mem);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_memory1_activate, NULL);

    // pclog
    elt = g_list_nth(list, 3);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_pc_log1_activate, NULL);
    gtk_check_menu_item_set_active(item, dbgs.pclog);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_pc_log1_activate, NULL);

    // stack
    elt = g_list_nth(list, 4);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_stack_frame1_activate, NULL);
    gtk_check_menu_item_set_active(item, dbgs.stack);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_stack_frame1_activate, NULL);
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
    on_quit1_activate(NULL, NULL);    
    return TRUE;
}

GLADE_CB void
on_dbgcode_window_hide                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.code = 0;
    gtk_window_get_size(GTK_WINDOW(widget), &options3.code.w, &options3.code.h);
    gtk_window_get_position(GTK_WINDOW(widget), &options3.code.x, &options3.code.y);
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

// callbacks from dbg_stack.c
GLADE_CB gboolean
on_dbgstack_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);    
    return TRUE;
}

GLADE_CB void
on_dbgstack_window_hide                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.stack = 0;
    gtk_window_get_size(GTK_WINDOW(widget), &options3.stack.w, &options3.stack.h);
    gtk_window_get_position(GTK_WINDOW(widget), &options3.stack.x, &options3.stack.y);
}

GLADE_CB void
on_dbgstack_window_show                (GtkWidget       *widget,
                                        gpointer         user_data)
{
    dbgs.stack = !0;
}

// misc
