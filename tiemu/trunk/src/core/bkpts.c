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


void ti68k_bkpt_set_address(uint32_t address) 
{
    bkpts.code = g_list_append(bkpts.code, GINT_TO_POINTER(address));
}

void ti68k_bkpt_set_access(uint32_t address, int mode) 
{
    if((mode & BK_READ) && (mode & BK_BYTE))
        bkpts.mem_rb = g_list_append(bkpts.mem_rb, GINT_TO_POINTER(address));
    else if((mode & BK_READ) && (mode & BK_WORD))
		bkpts.mem_rw = g_list_append(bkpts.mem_rw, GINT_TO_POINTER(address));
    else if((mode & BK_READ) && (mode & BK_LONG))
		bkpts.mem_rl = g_list_append(bkpts.mem_rl, GINT_TO_POINTER(address));
    else if((mode & BK_WRITE) && (mode & BK_BYTE))
		bkpts.mem_wb = g_list_append(bkpts.mem_wb, GINT_TO_POINTER(address));
    else if((mode & BK_WRITE) && (mode & BK_WORD))
		bkpts.mem_ww = g_list_append(bkpts.mem_ww, GINT_TO_POINTER(address));
    else if((mode & BK_WRITE) && (mode & BK_LONG))
		bkpts.mem_wl = g_list_append(bkpts.mem_wl, GINT_TO_POINTER(address));
}

void ti68k_bkpt_set_range(uint32_t addressMin, uint32_t addressMax, int mode) 
{
    if(mode & BK_READ) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

		s->val1 = addressMin;
		s->val2 = addressMax;

        bkpts.mem_rng_r = g_list_append(bkpts.mem_rng_r, s);
    }

    if(mode & BK_WRITE) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

		s->val1 = addressMin;
		s->val2 = addressMax;

        bkpts.mem_rng_w = g_list_append(bkpts.mem_rng_w, s);
    }
}

void ti68k_bkpt_set_exception(int number) 
{
    bkpts.exception = g_list_append(bkpts.exception, GINT_TO_POINTER(number));
}


void ti68k_bkpt_del_address(uint32_t address) 
{
    bkpts.code = g_list_remove(bkpts.code, GINT_TO_POINTER(address));
}

void ti68k_bkpt_del_access(uint32_t address, int mode) 
{
    if (mode==BK_READ_BYTE) 
        bkpts.mem_rb = g_list_remove(bkpts.mem_rb, GINT_TO_POINTER(address));
    if (mode==BK_READ_WORD) 
        bkpts.mem_rw = g_list_remove(bkpts.mem_rw, GINT_TO_POINTER(address));
    if (mode==BK_READ_LONG) 
        bkpts.mem_rl = g_list_remove(bkpts.mem_rl, GINT_TO_POINTER(address));
    if (mode==BK_WRITE_BYTE) 
        bkpts.mem_wb = g_list_remove(bkpts.mem_wb, GINT_TO_POINTER(address));
    if (mode==BK_WRITE_WORD) 
        bkpts.mem_ww = g_list_remove(bkpts.mem_ww, GINT_TO_POINTER(address));
    if (mode==BK_WRITE_LONG) 
        bkpts.mem_wl = g_list_remove(bkpts.mem_wl, GINT_TO_POINTER(address));
}

static gint compare_func(gconstpointer a, gconstpointer b)
{
    ADDR_RANGE *sa = (ADDR_RANGE *)a;
    ADDR_RANGE *sb = (ADDR_RANGE *)b;

    return (sa->val1 == sb->val1) && (sa->val2 == sb->val2);
}

void ti68k_bkpt_del_range(uint32_t min, uint32_t max, int mode) 
{
    ADDR_RANGE s;
    GList *elt = NULL;

    s.val1 = min;
    s.val2 = max;

    if (mode & BK_READ) 
    {       
        elt = g_list_find_custom(bkpts.mem_rng_r, &s, compare_func);       
        bkpts.mem_rng_r = g_list_delete_link(bkpts.mem_rng_r, elt);
    }
    else if (mode & BK_WRITE) 
    {   
        elt = g_list_find_custom(bkpts.mem_rng_r, &s, compare_func);       
        bkpts.mem_rng_w = g_list_delete_link(bkpts.mem_rng_w, elt);
    }
}

void ti68k_bkpt_del_exception(int n) 
{
    bkpts.exception = g_list_remove(bkpts.exception, GINT_TO_POINTER(n));
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

void ti68k_bkpt_clear_range(void)
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

void ti68k_bkpt_clear_exception(void)
{
	g_list_free(bkpts.exception);
	bkpts.exception = NULL;
}


void ti68k_bkpt_get_cause(int *type, int *mode, int *id) 
{
    *type = bkpts.type;
    *mode = bkpts.mode;
    *id   = bkpts.id;
}
