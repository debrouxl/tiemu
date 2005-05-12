/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin, Kevin Kofler
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
#ifdef __WIN32__
#include <gdk/gdkwin32.h>
#endif
#ifdef __MINGW32__
#include <windows.h>
#endif

#include "ti68k_int.h"
#include "struct.h"
#include "dbg_all.h"
#include "support.h"
#include "paths.h"
#include "gdbcall.h"
void gdbtk_hide_insight(void);
void gdbtk_show_insight(void);

gchar *symfile;

DbgOptions options3;
DbgWidgets dbgw = { 0 };

int dbg_on = 0;

/* Functions applicable to the whole debugger */

// create windows but don't show them
void gtk_debugger_preload(void)
{
	// open debugger windows
	dbgw.regs  = dbgregs_create_window();
	dbgw.mem   = dbgmem_create_window();
	dbgw.bkpts = dbgbkpts_create_window();
	dbgw.pclog = dbgpclog_create_window();
    dbgw.stack = dbgstack_create_window();
	dbgw.heap  = dbgheap_create_window();
	dbgw.code  = dbgcode_create_window();
}

// show previously created window
int gtk_debugger_enter(int context)
{
	gint type, id, mode;

	// debugger is open
	dbg_on = !0;

    // show breakpoint source (modal)
    switch(context)
    {
    case DBG_TRACE:
        break;
    case DBG_BREAK:
        break;
    }

    // display debugger windows (if not)
	gdbtk_show_insight();
	dbgregs_display_window();
	dbgmem_display_window();
	dbgbkpts_display_window();
    dbgpclog_display_window();
    dbgstack_display_window();
	dbgheap_display_window();
	dbgcode_display_window();	// the last has focus

	// enable the debugger if GDB disabled it
	if (!GTK_WIDGET_SENSITIVE(dbgw.regs))
		set_other_windows_sensitivity(TRUE);

	// handle automatic debugging requests
	if (symfile)
	{
		// get context
		ti68k_bkpt_get_cause(&type, &mode, &id);

		if(type == BK_TYPE_PGMENTRY)
		{
			uint16_t handle;
			uint32_t pc;

			ti68k_bkpt_get_pgmentry(id, &handle);
			ti68k_bkpt_del_pgmentry(handle);
			if(options3.bkpts.visible)
				dbgbkpts_refresh_window();

			ti68k_register_get_pc(&pc);
			gdb_add_symbol_file(symfile, pc);
			g_free (symfile);
			symfile = NULL;

			ti68k_unprotect_64KB_range(pc);

			gdb_hbreak("__main");
		}
	}

	return 0;
}

// refresh content of all windows
void gtk_debugger_refresh(void)
{	
	if(options3.regs.visible)
		dbgregs_refresh_window();
	if(options3.mem.visible)
		dbgmem_refresh_window();
	if(options3.bkpts.visible)
		dbgbkpts_refresh_window();
	if(options3.pclog.visible)
		dbgpclog_refresh_window();
	if(options3.code.visible)
		dbgcode_refresh_window();
    if(options3.stack.visible)
		dbgstack_refresh_window();
	if(options3.heap.visible)
		dbgheap_refresh_window();
}

// make windows (un-)modifiable
void set_other_windows_sensitivity(int state)
{
    gtk_widget_set_sensitive(dbgw.regs, state);
    gtk_widget_set_sensitive(dbgw.bkpts, state);
    gtk_widget_set_sensitive(dbgw.mem, state);
    gtk_widget_set_sensitive(dbgw.pclog, state);
    gtk_widget_set_sensitive(dbgw.stack, state);
    gtk_widget_set_sensitive(dbgw.heap, state);
}

// minimize all windows
void gtk_debugger_minimize_all(int all)
{
    if(options3.regs.visible)
        gtk_window_iconify(GTK_WINDOW(dbgw.regs));
    if(options3.bkpts.visible)
        gtk_window_iconify(GTK_WINDOW(dbgw.bkpts));
    if(options3.mem.visible)
        gtk_window_iconify(GTK_WINDOW(dbgw.mem));
    if(options3.pclog.visible)
        gtk_window_iconify(GTK_WINDOW(dbgw.pclog));
    if(options3.code.visible & all)
        gtk_window_iconify(GTK_WINDOW(dbgw.code));
    if(options3.stack.visible)
        gtk_window_iconify(GTK_WINDOW(dbgw.stack));
	if(options3.heap.visible)
        gtk_window_iconify(GTK_WINDOW(dbgw.heap));
}

// unminimize all windows
void gtk_debugger_unminimize_all(int all)
{
    if(options3.regs.visible)
        gtk_window_deiconify(GTK_WINDOW(dbgw.regs));
    if(options3.bkpts.visible)
        gtk_window_deiconify(GTK_WINDOW(dbgw.bkpts));
    if(options3.mem.visible)
        gtk_window_deiconify(GTK_WINDOW(dbgw.mem));
    if(options3.pclog.visible)
        gtk_window_deiconify(GTK_WINDOW(dbgw.pclog));
    if(options3.code.visible & all)
        gtk_window_deiconify(GTK_WINDOW(dbgw.code));
    if(options3.stack.visible)
        gtk_window_deiconify(GTK_WINDOW(dbgw.stack));
	if(options3.heap.visible)
        gtk_window_deiconify(GTK_WINDOW(dbgw.heap));
}

// show all windows
void gtk_debugger_show_all(int all)
{
    if(!dbg_on)
        return;

    if(!options3.regs.visible)
        gtk_widget_show(dbgw.regs);
    if(!options3.bkpts.visible)
        gtk_widget_show(dbgw.bkpts);
    if(!options3.mem.visible)
        gtk_widget_show(dbgw.mem);
    if(!options3.pclog.visible)
        gtk_widget_show(dbgw.pclog);
    if(!options3.code.visible && all)
        gtk_widget_show(dbgw.code);
    if(!options3.stack.visible)
        gtk_widget_show(dbgw.stack);
	if(!options3.heap.visible)
        gtk_widget_show(dbgw.heap);
}

// or hide them
void gtk_debugger_hide_all(int all)
{
    if(!dbg_on)
        return;

    if(options3.regs.visible)
        gtk_widget_hide(dbgw.regs);
    if(options3.bkpts.visible)
        gtk_widget_hide(dbgw.bkpts);
    if(options3.mem.visible)
        gtk_widget_hide(dbgw.mem);
    if(options3.pclog.visible)
        gtk_widget_hide(dbgw.pclog);
    if(options3.code.visible && all)
        gtk_widget_hide(dbgw.code);
    if(options3.stack.visible)
        gtk_widget_hide(dbgw.stack);
	if(options3.heap.visible)
        gtk_widget_hide(dbgw.heap);
}

/* Callbacks */

// callbacks from dbg_code.c (window menu)
// used to show/hide or minimize/un-minimize windows

GLADE_CB void
on_registers1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
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
on_heap_frame1_activate                    (GtkMenuItem     *menuitem,
                                             gpointer         user_data)
{
    if(GTK_CHECK_MENU_ITEM(menuitem)->active != TRUE) 
        gtk_widget_hide(dbgw.heap);
  	else
        dbgheap_display_window();
}


GLADE_CB void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	// hide all windows
	gdbtk_hide_insight();
	gtk_debugger_hide_all(!0);
    dbg_on = 0;

    // and restarts the emulator
	ti68k_bkpt_set_cause(0, 0, 0);
    if (engine_is_stopped()) gdbcall_continue();
}

GLADE_CB void
on_show_all1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gtk_debugger_show_all(0);
}


GLADE_CB void
on_hide_all1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	gtk_debugger_hide_all(0);
}

GLADE_CB void
on_minimize_all1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_debugger_minimize_all(0);
}


GLADE_CB void
on_maximize_all1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gtk_debugger_unminimize_all(0);
}

extern void options3_set_default(void);

GLADE_CB void
on_restore_all1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
	// restore windows with default settings
	options3_set_default();
}

// reflects window state in menu
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
    gtk_check_menu_item_set_active(item, options3.regs.visible);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_registers1_activate, NULL);

    // bkpts
    elt = g_list_nth(list, 1);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_breakpoints1_activate, NULL);
    gtk_check_menu_item_set_active(item, options3.bkpts.visible);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_breakpoints1_activate, NULL);

    // mem
    elt = g_list_nth(list, 2);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_memory1_activate, NULL);
    gtk_check_menu_item_set_active(item, options3.mem.visible);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_memory1_activate, NULL);

    // pclog
    elt = g_list_nth(list, 3);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_pc_log1_activate, NULL);
    gtk_check_menu_item_set_active(item, options3.pclog.visible);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_pc_log1_activate, NULL);

    // stack
    elt = g_list_nth(list, 4);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_stack_frame1_activate, NULL);
    gtk_check_menu_item_set_active(item, options3.stack.visible);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_stack_frame1_activate, NULL);
	
	// heap
    elt = g_list_nth(list, 5);
    item = GTK_CHECK_MENU_ITEM(elt->data);
    g_signal_handlers_block_by_func(GTK_OBJECT(item), on_heap_frame1_activate, NULL);
    gtk_check_menu_item_set_active(item, options3.heap.visible);
    g_signal_handlers_unblock_by_func(GTK_OBJECT(item), on_heap_frame1_activate, NULL);
}

// callbacks from dbg_regs.c

/* 
	This function exists because GDK retrieves client coordinates, not window ones.
	(Kevin: GDK uses GetClientRect and ClientToScreen).
	We need that to save and restore windows position.
*/
void window_get_rect(GtkWidget *widget, GdkRect *rect)
{
	gtk_window_get_size(GTK_WINDOW(widget), &rect->w, &rect->h);
	gdk_window_get_position(widget->window, &rect->x, &rect->y);

#ifdef __WIN32__
	{
		BOOL bResult;
		HWND hWnd = GDK_WINDOW_HWND(widget->window);
		RECT lpRect;

		bResult = GetWindowRect(hWnd, &lpRect);

		rect->x = lpRect.left;
		rect->y = lpRect.top;
	}
#endif
}

GLADE_CB gboolean
on_dbgregs_window_delete_event         (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);
	return TRUE;
}

GLADE_CB void
on_dbgregs_window_state_event		   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GdkEventWindowState *wstate = (GdkEventWindowState *)event;
    GdkWindowState state = wstate->new_window_state;
    GdkWindowState mask = wstate->changed_mask;

	if(mask & GDK_WINDOW_STATE_WITHDRAWN)
	{
		options3.regs.visible = !(state & GDK_WINDOW_STATE_WITHDRAWN);
		window_get_rect(widget, &options3.regs.rect);
	}

	if(mask & GDK_WINDOW_STATE_ICONIFIED)
		options3.regs.minimized = state & GDK_WINDOW_STATE_ICONIFIED;
}

// callbacks from dbg_pclog.c
GLADE_CB gboolean
on_dbgpclog_window_delete_event        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);    
    return TRUE;
}

GLADE_CB void
on_dbgpclog_window_state_event		   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GdkEventWindowState *wstate = (GdkEventWindowState *)event;
    GdkWindowState state = wstate->new_window_state;
    GdkWindowState mask = wstate->changed_mask;

	if(mask & GDK_WINDOW_STATE_WITHDRAWN)
	{
		options3.pclog.visible = !(state & GDK_WINDOW_STATE_WITHDRAWN);
		window_get_rect(widget, &options3.pclog.rect);
	}

	if(mask & GDK_WINDOW_STATE_ICONIFIED)
		options3.pclog.minimized = state & GDK_WINDOW_STATE_ICONIFIED;
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
on_dbgmem_window_state_event		   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GdkEventWindowState *wstate = (GdkEventWindowState *)event;
    GdkWindowState state = wstate->new_window_state;
    GdkWindowState mask = wstate->changed_mask;

	if(mask & GDK_WINDOW_STATE_WITHDRAWN)
	{
		options3.mem.visible = !(state & GDK_WINDOW_STATE_WITHDRAWN);
		window_get_rect(widget, &options3.mem.rect);
	}

	if(mask & GDK_WINDOW_STATE_ICONIFIED)
		options3.mem.minimized = state & GDK_WINDOW_STATE_ICONIFIED;
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
on_dbgcode_window_state_event		   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GdkEventWindowState *wstate = (GdkEventWindowState *)event;
    GdkWindowState state = wstate->new_window_state;
    GdkWindowState mask = wstate->changed_mask;

	if(mask & GDK_WINDOW_STATE_WITHDRAWN)
	{
		options3.code.visible = !(state & GDK_WINDOW_STATE_WITHDRAWN);
		window_get_rect(widget, &options3.code.rect);
	}

	if(mask & GDK_WINDOW_STATE_ICONIFIED)
		options3.code.minimized = state & GDK_WINDOW_STATE_ICONIFIED;
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
on_dbgbkpts_window_state_event		   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GdkEventWindowState *wstate = (GdkEventWindowState *)event;
    GdkWindowState state = wstate->new_window_state;
    GdkWindowState mask = wstate->changed_mask;

	if(mask & GDK_WINDOW_STATE_WITHDRAWN)
	{
		options3.bkpts.visible = !(state & GDK_WINDOW_STATE_WITHDRAWN);
		window_get_rect(widget, &options3.bkpts.rect);
	}

	if(mask & GDK_WINDOW_STATE_ICONIFIED)
		options3.bkpts.minimized = state & GDK_WINDOW_STATE_ICONIFIED;
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
on_dbgstack_window_state_event		   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GdkEventWindowState *wstate = (GdkEventWindowState *)event;
    GdkWindowState state = wstate->new_window_state;
    GdkWindowState mask = wstate->changed_mask;

	if(mask & GDK_WINDOW_STATE_WITHDRAWN)
	{
		options3.stack.visible = !(state & GDK_WINDOW_STATE_WITHDRAWN);
		window_get_rect(widget, &options3.stack.rect);
	}

	if(mask & GDK_WINDOW_STATE_ICONIFIED)
		options3.stack.minimized = state & GDK_WINDOW_STATE_ICONIFIED;
}

// callbacks from dbg_heap.c
GLADE_CB gboolean
on_dbgheap_window_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gtk_widget_hide(widget);    
    return TRUE;
}

GLADE_CB void
on_dbgheap_window_state_event		   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GdkEventWindowState *wstate = (GdkEventWindowState *)event;
    GdkWindowState state = wstate->new_window_state;
    GdkWindowState mask = wstate->changed_mask;

	if(mask & GDK_WINDOW_STATE_WITHDRAWN)
	{
		options3.heap.visible = !(state & GDK_WINDOW_STATE_WITHDRAWN);
		window_get_rect(widget, &options3.heap.rect);
	}

	if(mask & GDK_WINDOW_STATE_ICONIFIED)
		options3.heap.minimized = state & GDK_WINDOW_STATE_ICONIFIED;
}

// misc

// Common remplacement for gtk_window_(de)iconify)
void gtk_window_minimize(GtkWindow *window, gboolean action)
{
	if(action)
		gtk_window_iconify(window);
	else
		gtk_window_deiconify(window);
}
