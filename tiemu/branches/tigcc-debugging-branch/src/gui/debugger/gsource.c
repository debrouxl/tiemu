/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 573 2004-07-16 09:00:43Z roms $ */

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

/*
	This unit add a new event to the 'Main Event Loop' of GTK (which is build on
	the GLib loop in fact).	This event comes from a source and can triggers a callback.

	The idea is to launch the GTK debugger when the M68K engine thread encounters a
	breakpoint while separating things because GTK is thread safe but not thread aware.

	The source is a variable set by the emulation engine. This source is parsed by the GTK
	event loop, triggers a callback when needed which call our GTK debugger.

	By this mean, we have a clean and not CPU intensive way to work.

	See <http://developer.gnome.org/doc/API/2.0/glib/glib-The-Main-Event-Loop.html#GSource>
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>

G_LOCK_EXTERN(debugger);		// defined in
extern volatile int debugger;	// engine.c

static gboolean
gs_prepare( GSource     *source,
            gint        *timeout )
{
    return debugger ? TRUE : FALSE;
}

static gboolean
gs_check( GSource     *source )
{
	return debugger ? TRUE : FALSE;
}

static gboolean
gs_dispatch( GSource     *source,
             GSourceFunc  callback,
             gpointer     user_data )
{
	G_LOCK(debugger);
	user_data = GINT_TO_POINTER(debugger);	// save data
	debugger = 0;							// ack debugger
	G_UNLOCK(debugger);

	return callback(user_data);				// raise callback
}

static GSourceFuncs gs_funcs = 
{
    gs_prepare,
    gs_check,
    gs_dispatch,
    NULL,
    NULL,
    NULL
};

GSource* attach_dbg_gsource(void)
{
	GSource *source;
	GMainContext *context;

	source = g_source_new(&gs_funcs, sizeof(GSource));
	context = g_main_context_default();	
	g_source_attach(source, context);

	return source;
}
