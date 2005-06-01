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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*
    Breakpoints management
	Note: addresses are 24 bits but arguments are 32 bits. The MSB is used to store
	extra informations and save speed for comparison.
	Searching does not take into account the MSB (24 bits).
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

/* Add */

int ti68k_bkpt_add_address(uint32_t address)
{
    bkpts.code = g_list_append(bkpts.code, GINT_TO_POINTER(address));
	return g_list_length(bkpts.code) - 1;
}

int ti68k_bkpt_add_access(uint32_t address, int mode) 
{
    if((mode & BK_READ) && (mode & BK_BYTE))
        bkpts.mem_rb = g_list_append(bkpts.mem_rb, GINT_TO_POINTER(address));
    else if((mode & BK_READ) && (mode & BK_WORD))
		bkpts.mem_rw = g_list_append(bkpts.mem_rw, GINT_TO_POINTER(address));
    else if((mode & BK_READ) && (mode & BK_LONG))
		bkpts.mem_rl = g_list_append(bkpts.mem_rl, GINT_TO_POINTER(address));
    
	if((mode & BK_WRITE) && (mode & BK_BYTE))
		bkpts.mem_wb = g_list_append(bkpts.mem_wb, GINT_TO_POINTER(address));
    else if((mode & BK_WRITE) && (mode & BK_WORD))
		bkpts.mem_ww = g_list_append(bkpts.mem_ww, GINT_TO_POINTER(address));
    else if((mode & BK_WRITE) && (mode & BK_LONG))
		bkpts.mem_wl = g_list_append(bkpts.mem_wl, GINT_TO_POINTER(address));

	return -1;
}

int ti68k_bkpt_add_range(uint32_t addressMin, uint32_t addressMax, int mode) 
{
    if(mode & BK_READ) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

		s->val1 = addressMin;
		s->val2 = addressMax;

        bkpts.mem_rng_r = g_list_append(bkpts.mem_rng_r, s);
		//return g_list_length(bkpts.mem_rng_r) - 1;
    }

    if(mode & BK_WRITE) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

		s->val1 = addressMin;
		s->val2 = addressMax;

        bkpts.mem_rng_w = g_list_append(bkpts.mem_rng_w, s);
		//return g_list_length(bkpts.mem_rng_w) - 1;
    }

	return g_list_length(bkpts.mem_rng_r) - 1;
}

int ti68k_bkpt_add_exception(uint32_t number) 
{
    bkpts.exception = g_list_append(bkpts.exception, GINT_TO_POINTER(number));
	return g_list_length(bkpts.exception)-1;
}

int ti68k_bkpt_add_pgmentry(uint16_t handle) 
{
    bkpts.pgmentry = g_list_append(bkpts.pgmentry, GINT_TO_POINTER((uint32_t)handle));
	return g_list_length(bkpts.pgmentry)-1;
}

/* Delete */

static gint compare_func(gconstpointer a, gconstpointer b)
{
    ADDR_RANGE *sa = (ADDR_RANGE *)a;
    ADDR_RANGE *sb = (ADDR_RANGE *)b;

    return !((BKPT_ADDR(sa->val1) == BKPT_ADDR(sb->val1)) && 
			 (BKPT_ADDR(sa->val2) == BKPT_ADDR(sb->val2)));
}

static gint compare_func2(gconstpointer a, gconstpointer b)
{
	uint32_t aa = GPOINTER_TO_INT(a);
	uint32_t bb = GPOINTER_TO_INT(b);

	return !(BKPT_ADDR(aa) == BKPT_ADDR(bb));
}

int ti68k_bkpt_del_address(uint32_t address) 
{
	GList *elt = g_list_find_custom(bkpts.code, GINT_TO_POINTER(address), compare_func2);

	if(elt != NULL)
		bkpts.code = g_list_delete_link(bkpts.code, elt);
	else
		return -1;

	return 0;
}

int ti68k_bkpt_del_access(uint32_t address, int mode) 
{
    if((mode & BK_READ) && (mode & BK_BYTE))
	{
		GList *elt = g_list_find_custom(bkpts.mem_rb, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			bkpts.mem_rb = g_list_delete_link(bkpts.mem_rb, elt);
		else
			return -1;
	}
    if((mode & BK_READ) && (mode & BK_WORD))
    {
		GList *elt = g_list_find_custom(bkpts.mem_rw, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			bkpts.mem_rw = g_list_delete_link(bkpts.mem_rw, elt);
		else
			return -1;
	}
    if((mode & BK_READ) && (mode & BK_LONG))
    {
		GList *elt = g_list_find_custom(bkpts.mem_rl, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			bkpts.mem_rl = g_list_delete_link(bkpts.mem_rl, elt);
		else
			return -1;
	}
    if((mode & BK_WRITE) && (mode & BK_BYTE))
    {
		GList *elt = g_list_find_custom(bkpts.mem_wb, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			bkpts.mem_wb = g_list_delete_link(bkpts.mem_wb, elt);
		else
			return -1;
	}
    if((mode & BK_WRITE) && (mode & BK_WORD))
    {
		GList *elt = g_list_find_custom(bkpts.mem_ww, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			bkpts.mem_ww = g_list_delete_link(bkpts.mem_ww, elt);
		else
			return -1;
	}
    if((mode & BK_WRITE) && (mode & BK_LONG))
    {
		GList *elt = g_list_find_custom(bkpts.mem_wl, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			bkpts.mem_wl = g_list_delete_link(bkpts.mem_wl, elt);
		else
			return -1;
	}

	return 0;
}

int ti68k_bkpt_del_range(uint32_t min, uint32_t max, int mode) 
{
    ADDR_RANGE s;
    GList *elt = NULL;

    s.val1 = min;
    s.val2 = max;

    if (mode & BK_READ) 
    {       
        elt = g_list_find_custom(bkpts.mem_rng_r, &s, compare_func);       
        if(elt != NULL)
			bkpts.mem_rng_r = g_list_delete_link(bkpts.mem_rng_r, elt);
		else
			return -1;
    }
    
	if (mode & BK_WRITE) 
    {   
        elt = g_list_find_custom(bkpts.mem_rng_w, &s, compare_func);       
        if(elt != NULL)
			bkpts.mem_rng_w = g_list_delete_link(bkpts.mem_rng_w, elt);
		else
			return -1;
    }

	return 0;
}

int ti68k_bkpt_del_exception(uint32_t number) 
{
	GList *elt = g_list_find_custom(bkpts.exception, GINT_TO_POINTER(number), compare_func2);
    if(elt != NULL)
		bkpts.exception = g_list_delete_link(bkpts.exception, elt);
	else
		return -1;

	return 0;
}

int ti68k_bkpt_del_pgmentry(uint16_t handle) 
{
	GList *elt = g_list_find_custom(bkpts.pgmentry, GINT_TO_POINTER((uint32_t)handle), compare_func2);
    if(elt != NULL)
		bkpts.pgmentry = g_list_delete_link(bkpts.pgmentry, elt);
	else
		return -1;

	return 0;
}

/* Set */

int ti68k_bkpt_set_address(uint32_t address, uint32_t new_address)
{
	GList *elt = g_list_find_custom(bkpts.code, GINT_TO_POINTER(address), compare_func2);
	if(elt != NULL)
		elt->data = GINT_TO_POINTER(new_address);
	else
		return -1;

	return 0;
}

int ti68k_bkpt_set_access(uint32_t address, int mode, uint32_t new_address)
{
	if((mode & BK_READ) && (mode & BK_BYTE))
	{
		GList *elt = g_list_find_custom(bkpts.mem_rb, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			elt->data = GINT_TO_POINTER(new_address);
		else
			return -1;
	}
    else if((mode & BK_READ) && (mode & BK_WORD))
	{
		GList *elt = g_list_find_custom(bkpts.mem_rw, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			elt->data = GINT_TO_POINTER(new_address);
		else
			return -1;
	}
    else if((mode & BK_READ) && (mode & BK_LONG))
	{
		GList *elt = g_list_find_custom(bkpts.mem_rl, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			elt->data = GINT_TO_POINTER(new_address);
		else
			return -1;
	}
    
	if((mode & BK_WRITE) && (mode & BK_BYTE))
	{
		GList *elt = g_list_find_custom(bkpts.mem_wb, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			elt->data = GINT_TO_POINTER(new_address);
		else
			return -1;
	}
    else if((mode & BK_WRITE) && (mode & BK_WORD))
	{
		GList *elt = g_list_find_custom(bkpts.mem_ww, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			elt->data = GINT_TO_POINTER(new_address);
		else
			return -1;
	}
    else if((mode & BK_WRITE) && (mode & BK_LONG))
	{
		GList *elt = g_list_find_custom(bkpts.mem_wl, GINT_TO_POINTER(address), compare_func2);
		if(elt != NULL)
			elt->data = GINT_TO_POINTER(new_address);
		else
			return -1;
	}

	return 0;
}

int ti68k_bkpt_set_range(uint32_t min, uint32_t max, int mode, uint32_t new_min, uint32_t new_max)
{
	ADDR_RANGE s, *p;
    GList *elt = NULL;

    s.val1 = min;
    s.val2 = max;

    if (mode & BK_READ) 
    {       
        elt = g_list_find_custom(bkpts.mem_rng_r, &s, compare_func);       
		if(elt == NULL)
			return -1;

        p = elt->data;
		p->val1 = new_min;
		p->val2 = new_max;
    }
    
	if (mode & BK_WRITE) 
    {   
        elt = g_list_find_custom(bkpts.mem_rng_w, &s, compare_func);       
		if(elt == NULL)
			return -1;

        p = elt->data;
		p->val1 = new_min;
		p->val2 = new_max;
    }

	return 0;
}

int ti68k_bkpt_set_exception(uint32_t number, uint32_t new_n)
{
	GList *elt = g_list_find_custom(bkpts.exception, GINT_TO_POINTER(number), compare_func2);
	if(elt != NULL)
		elt->data = GINT_TO_POINTER(new_n);
	else
		return -1;

	return 0;
}

int ti68k_bkpt_set_pgmentry(uint16_t handle, uint16_t new_h)
{
	GList *elt = g_list_find_custom(bkpts.pgmentry, 
					GINT_TO_POINTER((uint32_t)handle), 
					compare_func2);
	if(elt != NULL)
		elt->data = GINT_TO_POINTER((uint32_t)new_h);
	else
		return -1;

	return 0;
}

/* Get */

int ti68k_bkpt_get_address(int id, uint32_t *address)
{
	if(g_list_length(bkpts.code) == 0)
		return -1;

	*address = GPOINTER_TO_INT(g_list_nth(bkpts.code, id)->data);
	return 0;
}

int ti68k_bkpt_get_access(int id, uint32_t *address, int mode)
{
	if((mode & BK_READ) && (mode & BK_BYTE))
	{
		if(g_list_length(bkpts.mem_rb) == 0) return -1;
		*address = GPOINTER_TO_INT(g_list_nth(bkpts.mem_rb, id)->data);
	}
    else if((mode & BK_READ) && (mode & BK_WORD))
	{
		if(g_list_length(bkpts.mem_rw) == 0) return -1;
		*address = GPOINTER_TO_INT(g_list_nth(bkpts.mem_rw, id)->data);
	}
    else if((mode & BK_READ) && (mode & BK_LONG))
	{
		if(g_list_length(bkpts.mem_rl) == 0) return -1;
		*address = GPOINTER_TO_INT(g_list_nth(bkpts.mem_rl, id)->data);
	}
    
	if((mode & BK_WRITE) && (mode & BK_BYTE))
	{
		if(g_list_length(bkpts.mem_wb) == 0) return -1;
		*address = GPOINTER_TO_INT(g_list_nth(bkpts.mem_wb, id)->data);
	}
    else if((mode & BK_WRITE) && (mode & BK_WORD))
	{
		if(g_list_length(bkpts.mem_wl) == 0) return -1;
		*address = GPOINTER_TO_INT(g_list_nth(bkpts.mem_ww, id)->data);
	}
    else if((mode & BK_WRITE) && (mode & BK_LONG))
	{
		if(g_list_length(bkpts.mem_wl) == 0) return -1;
		*address = GPOINTER_TO_INT(g_list_nth(bkpts.mem_wl, id)->data);
	}

	return 0;
}

int ti68k_bkpt_get_range(int id, uint32_t *min, uint32_t *max, int mode)
{
	if(mode & BK_READ) 
    {
        ADDR_RANGE *s;
		
		if(g_list_length(bkpts.mem_rng_r) == 0) return -1;		
		s = g_list_nth(bkpts.mem_rng_r, id)->data;

		*min = s->val1;
		*max = s->val2;
    }

    if(mode & BK_WRITE) 
    {
        ADDR_RANGE *s;
		
		if(g_list_length(bkpts.mem_rng_w) == 0) return -1;		
		s = g_list_nth(bkpts.mem_rng_w, id)->data;

		*min = s->val1;
		*max = s->val2;
    }

	return 0;
}

int ti68k_bkpt_get_exception(int id, uint32_t *number)
{
	if(g_list_length(bkpts.exception) == 0)
		return -1;

	*number = GPOINTER_TO_INT(g_list_nth(bkpts.exception, id)->data);
	return 0;
}

int ti68k_bkpt_get_pgmentry(int id, uint16_t *handle)
{
	if(g_list_length(bkpts.pgmentry) == 0)
		return -1;
	
	*handle = GPOINTER_TO_INT(g_list_nth(bkpts.pgmentry, id)->data);
	return 0;
}

/* Clear */

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

void ti68k_bkpt_clear_pgmentry(void)
{
	g_list_free(bkpts.pgmentry);
	bkpts.pgmentry = NULL;
}

/* Others */

void ti68k_bkpt_set_cause(int type, int mode, int id)
{
    bkpts.type = type;
    bkpts.mode = mode;
    bkpts.id = id;
}

void ti68k_bkpt_get_cause(int *type, int *mode, int *id) 
{
    *type = bkpts.type;
    *mode = bkpts.mode;
    *id   = bkpts.id;
}

void ti68k_bkpt_clear_all(void)
{
    ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_access();
	ti68k_bkpt_clear_range();
    ti68k_bkpt_clear_exception();

	ti68k_bkpt_set_cause(0, 0, 0);	
}
