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

#include "./debugger/dbg_bkpts.h"
#include "./debugger/dbg_code.h"
#include "./debugger/dbg_cause.h"
#include "./debugger/dbg_data.h"
#include "./debugger/dbg_mem.h"
#include "./debugger/dbg_regs.h"
#include "./debugger/dbg_vectors.h"


int enter_gtk_debugger(void)
{
	ti68k_engine_halt();
    
	//display_dbgmem_window();
	//display_dbgregs_window();
	//display_dbgvectors_dbox();
	//display_dbgdata_dbox();
	//display_dbgbkpts_window();
    display_dbgcode_window();

	return 0;
}


