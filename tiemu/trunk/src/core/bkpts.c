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

/*
    Breakpoints management
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "intl.h"
#include "bkpts.h"
#include "tilibs.h"
#include "ti68k_err.h"
#include "ti68k_def.h"


int ti68k_bkpt_set_address(int address) 
{
    bkpts.code = g_list_append(bkpts.code, GINT_TO_POINTER(address));
    return g_list_length(bkpts.code);
}

int ti68k_bkpt_set_address_range(int addressMin, int addressMax, int mode) 
{
    if(mode & BK_READ) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

        bkpts.mem_rng_r = g_list_append(bkpts.mem_rng_r, s);
        return g_list_length(bkpts.mem_rng_r);
    }

    if(mode & BK_WRITE) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

        bkpts.mem_rng_w = g_list_append(bkpts.mem_rng_w, s);
        return g_list_length(bkpts.mem_rng_w);
    }
    
    return ERR_68K_INVALID_MODE;
}

int ti68k_bkpt_set_access(int address, int mode) 
{
    if(mode==BK_READ_BYTE) 
    {		
        bkpts.mem_rb = g_list_append(bkpts.mem_rb, GINT_TO_POINTER(address));
        return g_list_length(bkpts.mem_rb);
    }
    else if(mode==BK_READ_WORD) 
    {
      bkpts.mem_rw = g_list_append(bkpts.mem_rw, GINT_TO_POINTER(address));
      return g_list_length(bkpts.mem_rw);
    }
    else if(mode==BK_READ_LONG) 
    {
      bkpts.mem_rl = g_list_append(bkpts.mem_rl, GINT_TO_POINTER(address));
      return g_list_length(bkpts.mem_rl);
    }
    else if (mode==BK_WRITE_BYTE) 
    {
      bkpts.mem_wb = g_list_append(bkpts.mem_wb, GINT_TO_POINTER(address));
      return g_list_length(bkpts.mem_wb);
    }
    else if (mode==BK_WRITE_WORD) 
    {
      bkpts.mem_ww = g_list_append(bkpts.mem_ww, GINT_TO_POINTER(address));
      return g_list_length(bkpts.mem_ww);
    }
    else if (mode==BK_WRITE_LONG) 
    {
      bkpts.mem_wl = g_list_append(bkpts.mem_wl, GINT_TO_POINTER(address));
      return g_list_length(bkpts.mem_wl);
    }
    else
        return ERR_68K_INVALID_MODE;
}

int ti68k_bkpt_set_vector(int vector) 
{
    assert( (vector >= 2) && (vector < 16) );
    bkpts.vectors[vector] = !0;
    bkpts.n_vectors++;
    return (bkpts.n_vectors-1);
}

int ti68k_bkpt_set_autoint(int autoint)
{
    assert( (autoint >= 1) && (autoint < 8) );
    bkpts.autoints[autoint] = !0;
    bkpts.n_autoints++;
    return (bkpts.n_autoints-1);
}

int ti68k_bkpt_set_trap(int trap) 
{
    assert( (trap >= 0) && (trap < 16) );
    bkpts.traps[trap] = !0;
    bkpts.n_traps++;
    return (bkpts.n_traps-1);
}


void ti68k_bkpt_del_address(int i) 
{
    bkpts.code = g_list_delete_link(bkpts.code, g_list_nth(bkpts.code, i));
}

void ti68k_bkpt_del_access(int i, int mode) 
{
    if (mode==BK_READ_BYTE) 
    {		
        bkpts.mem_rb = g_list_delete_link(bkpts.mem_rb, g_list_nth(bkpts.mem_rb, i));
    }
    if (mode==BK_READ_WORD) 
    {
      bkpts.mem_rw = g_list_delete_link(bkpts.mem_rw, g_list_nth(bkpts.mem_rw, i));
    }
    if (mode==BK_READ_LONG) 
    {
      bkpts.mem_rl = g_list_delete_link(bkpts.mem_rl, g_list_nth(bkpts.mem_rl, i));
    }
    if (mode==BK_WRITE_BYTE) 
    {
      bkpts.mem_wb = g_list_delete_link(bkpts.mem_wb, g_list_nth(bkpts.mem_wb, i));
    }
    if (mode==BK_WRITE_WORD) 
    {
      bkpts.mem_ww = g_list_delete_link(bkpts.mem_ww, g_list_nth(bkpts.mem_ww, i));
    }
    if (mode==BK_WRITE_LONG) 
    {
      bkpts.mem_wl = g_list_delete_link(bkpts.mem_wl, g_list_nth(bkpts.mem_wl, i));
    }
}

void ti68k_bkpt_delaccess_range(int i,int mode) 
{
    if (mode & BK_READ) 
    {
        GList *elt = g_list_nth(bkpts.mem_rng_r, i);

        g_free(elt->data);
        bkpts.mem_rng_r = g_list_delete_link(bkpts.mem_rng_r, elt);
    }
    else if (mode & BK_WRITE) 
    {
        GList *elt = g_list_nth(bkpts.mem_rng_w, i);

        g_free(elt->data);
        bkpts.mem_rng_w = g_list_delete_link(bkpts.mem_rng_w, elt);
    }
    else
    {
        DISPLAY(_("delBreakpointAccessRange: mode is neither READ nor WRITE. This is a bug. Please report it !\n"));
        exit(-1);
    }
}

void ti68k_bkpt_del_vector(int vector) 
{
    assert( (vector >= 2) && (vector < 16) );
    bkpts.vectors[vector] = 0;
    bkpts.n_vectors--;
}

void ti68k_bkpt_del_autoint(int autoint)
{
    assert( (autoint >= 1) && (autoint < 8) );
    bkpts.autoints[autoint] = 0;
    bkpts.n_autoints--;
}

void ti68k_bkpt_del_trap(int trap) 
{
    assert( (trap >= 0) && (trap < 16) );
    bkpts.traps[trap] = 0;
    bkpts.n_traps--;
}

void ti68k_bkpt_get_cause(int *type, int *mode, int *id) 
{
    *type = bkpts.type;
    *mode = bkpts.mode;
    *id   = bkpts.id;
}

void ti68k_bkpt_clear_address(void)
{
	g_list_free(bkpts.code);
	bkpts.code = NULL;
}

void ti68k_bkpt_clear_access(void)
{
	g_list_free(bkpts.mem_rb);
	bkpts.mem_rb = NULL;
	g_list_free(bkpts.mem_rw);
	bkpts.mem_rw = NULL;
	g_list_free(bkpts.mem_rl);
	bkpts.mem_rl = NULL;

	g_list_free(bkpts.mem_wb);
	bkpts.mem_wb = NULL;
	g_list_free(bkpts.mem_ww);
	bkpts.mem_ww = NULL;
	g_list_free(bkpts.mem_wl);
	bkpts.mem_wl = NULL;
}

void ti68k_bkpt_clear_access_range(void)
{
	GList *l;

	l = bkpts.mem_rng_r;
	while(l != NULL)
	{
        g_free(l->data);		
		l = g_list_next(l);
	}			
	g_list_free(bkpts.mem_rng_r);
	bkpts.mem_rng_r = NULL;

	l = bkpts.mem_rng_w;
	while(l != NULL)
	{
	    g_free(l->data);		
		l = g_list_next(l);
	}			
	g_list_free(bkpts.mem_rng_w);
	bkpts.mem_rng_w = NULL;
}

void ti68k_bkpt_clear_vector(void)
{
	memset(bkpts.vectors, 0, 16);
	bkpts.n_vectors = 0;
}

void ti68k_bkpt_clear_autoint(void)
{
	memset(bkpts.traps, 0, 16);
	bkpts.n_traps = 0;
}

void ti68k_bkpt_clear_trap(void)
{
	memset(bkpts.autoints, 0, 8);
	bkpts.n_autoints = 0;
}
