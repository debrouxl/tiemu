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

/*
    Run engine from GTK main loop at regular interval.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>

#include "intl.h"
#include "ti68k_int.h"
#include "m68k.h"
#include "engine.h"
#include "dbg_all.h"

/* 
   The TI92/89 should approximately execute NB_INSTRUCTIONS_PER_LOOP in 
   TIME_LIMIT milliseconds (1.500.000 instructions/s).
   If you think this values are a bit too big, you can slow down 
   the emulator by changing them 
*/
#define NB_INSTRUCTIONS_PER_LOOP 50000	// 50000 inst
#define TIME_LIMIT               30	    // 30 ms

static int cpu_instr = NB_INSTRUCTIONS_PER_LOOP;

static guint tid = 0;
static gint  res = 0;

// function called when the function below is exited
static void engine_notify(gint *data)
{
	// call debugger here
	if(*data)
		gtk_debugger_enter(GPOINTER_TO_INT(*data));

	// and reset source id
	tid = 0;
}

// function called by g_timeout_add_full/g_idle_add_full
static gboolean engine_func(gint *data)
{
	GTimer *tmr;
    gulong us;

	// set instruction rate
    if(params.cpu_rate != -1)
        cpu_instr = params.cpu_rate;

	// start measurement of time
	tmr = g_timer_new();
	g_timer_start(tmr);

	// run emulation core
	*data = hw_m68k_run(cpu_instr);

	// a bkpt has been encountered ? If yes, stop engine
	if(*data)
		return FALSE;

	// get time needed to execute 'cpu_instr' instructions
	g_timer_elapsed(tmr, &us);

	return TRUE;
}

// start emulation engine
void engine_start(void) 
{
	if(params.restricted)
		tid = g_timeout_add_full(G_PRIORITY_DEFAULT, TIME_LIMIT, engine_func, &res, engine_notify);
	else
		tid = g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, engine_func, &res, engine_notify);
}

// stop it
void engine_stop(void) 
{
	if(tid)
		g_source_remove(tid);
}

// state of engine
int engine_is_stopped() 
{
	return !tid;
}

int engine_is_running(void)
{
	return tid;
}

/*
	Called at startup to know needed time for exec'ing hw_m68k_run.
	This allow to make exec'ing more precise.
*/
void engine_calibrate(void)
{

}