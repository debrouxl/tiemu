/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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

int dbg_on = 0;

/* Functions applicable to the whole debugger */

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

int gtk_debugger_enter(int context)
{
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
	dbgregs_display_window();
	dbgmem_display_window();
	dbgbkpts_display_window();
    dbgpclog_display_window();
    dbgstack_display_window();
	dbgheap_display_window();
	dbgcode_display_window();	// the last has focus

	return 0;
}

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

void set_other_windows_sensitivity(int state)
{
    if(options3.regs.visible)
        gtk_widget_set_sensitive(dbgw.regs, state);
    if(options3.bkpts.visible)
        gtk_widget_set_sensitive(dbgw.bkpts, state);
    if(options3.mem.visible)
        gtk_widget_set_sensitive(dbgw.mem, state);
    if(options3.pclog.visible)
        gtk_widget_set_sensitive(dbgw.pclog, state);
    if(options3.stack.visible)
        gtk_widget_set_sensitive(dbgw.stack, state);
	if(options3.heap.visible)
        gtk_widget_set_sensitive(dbgw.heap, state);
}

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
	gtk_debugger_hide_all(!0);
    //while(gtk_events_pending()) gtk_main_iteration();

    dbg_on = 0;

    // Closing the debugger starts the emulator
	ti68k_bkpt_set_cause(0, 0, 0);
    engine_start();
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

	//???
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

		gtk_window_get_size(GTK_WINDOW(widget), &options3.regs.rect.w, &options3.regs.rect.h);
		gdk_window_get_position(widget->window, &options3.regs.rect.x, &options3.regs.rect.y);
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

		gtk_window_get_size(GTK_WINDOW(widget), &options3.pclog.rect.w, &options3.pclog.rect.h);
		gdk_window_get_position(widget->window, &options3.pclog.rect.x, &options3.pclog.rect.y);
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

		gtk_window_get_size(GTK_WINDOW(widget), &options3.mem.rect.w, &options3.mem.rect.h);
		gdk_window_get_position(widget->window, &options3.mem.rect.x, &options3.mem.rect.y);
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

		gtk_window_get_size(GTK_WINDOW(widget), &options3.code.rect.w, &options3.code.rect.h);
		gdk_window_get_position(widget->window, &options3.code.rect.x, &options3.code.rect.y);
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

		gtk_window_get_size(GTK_WINDOW(widget), &options3.bkpts.rect.w, &options3.bkpts.rect.h);
		gdk_window_get_position(widget->window, &options3.bkpts.rect.x, &options3.bkpts.rect.y);
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

		gtk_window_get_size(GTK_WINDOW(widget), &options3.stack.rect.w, &options3.stack.rect.h);
		gdk_window_get_position(widget->window, &options3.stack.rect.x, &options3.stack.rect.y);
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

		gtk_window_get_size(GTK_WINDOW(widget), &options3.heap.rect.w, &options3.heap.rect.h);
		gdk_window_get_position(widget->window, &options3.heap.rect.x, &options3.heap.rect.y);
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
