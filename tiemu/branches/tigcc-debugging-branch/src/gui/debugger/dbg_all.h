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

#ifndef __DBG_ALL_H__
#define __DBG_ALL_H__

#include "dbg_bkpts.h"
#include "dbg_code.h"
#include "dbg_cause.h"
#include "dbg_data.h"
#include "dbg_mem.h"
#include "dbg_regs.h"
#include "dbg_vectors.h"
#include "dbg_pclog.h"
#include "dbg_stack.h"
#include "dbg_heap.h"
#include "dbg_entry.h"

#define WND_STATE

/* Definitions */

typedef struct {
    GtkWidget *mem;
    GtkWidget *regs;
    GtkWidget *bkpts;
    GtkWidget *code;
    GtkWidget *pclog;
    GtkWidget *stack;
	GtkWidget *heap;
} DbgWidgets;
extern DbgWidgets dbgw;

/* Functions */

void gtk_debugger_preload(void);
int  gtk_debugger_enter(int context);
void gtk_debugger_refresh(void);

void set_other_windows_sensitivity(int state);

void gtk_debugger_minimize_all(int all);
void gtk_debugger_unminimize_all(int all);

void gtk_debugger_show_all(int all);
void gtk_debugger_hide_all(int all);

void gtk_window_minimize(GtkWindow *window, gboolean action);

/* Variables */

extern int dbg_on;
extern gchar *symfile;

// convenient place (calc.h)
extern GtkWidget *main_wnd;

/* Macros */

#define glade_get(s)		glade_xml_get_widget(xml, (s))

#endif
