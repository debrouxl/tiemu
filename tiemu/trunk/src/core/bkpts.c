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


/* Variables */
/*
// Memory
int breakType = 0;
int breakMode = 0;
int breakId   = 0;

GList* listBkptAsRB = NULL;
GList* listBkptAsWB = NULL;
GList* listBkptAsRW = NULL;
GList* listBkptAsWW = NULL;
GList* listBkptAsRL = NULL;
GList* listBkptAsWL = NULL;

GList* listBkptAsRgR = NULL;
GList* listBkptAsRgW = NULL;

// Code
GList *listBkptAddress  = NULL;

// Vectors & Traps
int listBkptVector[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int nBkptVector = 0;

int listBkptTrap[16]   = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int nBkptTrap = 0; 

int listBkptAutoint[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int nBkptAutoint = 0;
*/
/* Functions */


int ti68k_bkpt_set_address(int address) 
{
    bkpts.listBkptAddress = g_list_append(bkpts.listBkptAddress, GINT_TO_POINTER(address));
    return g_list_length(bkpts.listBkptAddress);
}

int ti68k_bkpt_set_address_range(int addressMin, int addressMax, int mode) 
{
    if(mode & BK_READ) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

        bkpts.listBkptAsRgR = g_list_append(bkpts.listBkptAsRgR, s);
        return g_list_length(bkpts.listBkptAsRgR);
    }

    if(mode & BK_WRITE) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

        bkpts.listBkptAsRgW = g_list_append(bkpts.listBkptAsRgW, s);
        return g_list_length(bkpts.listBkptAsRgW);
    }
    
    return ERR_68K_INVALID_MODE;
}

int ti68k_bkpt_set_access(int address, int mode) 
{
    if(mode==BK_READ_BYTE) 
    {		
        bkpts.listBkptAsRB = g_list_append(bkpts.listBkptAsRB, GINT_TO_POINTER(address));
        return g_list_length(bkpts.listBkptAsRB);
    }
    else if(mode==BK_READ_WORD) 
    {
      bkpts.listBkptAsRW = g_list_append(bkpts.listBkptAsRW, GINT_TO_POINTER(address));
      return g_list_length(bkpts.listBkptAsRW);
    }
    else if(mode==BK_READ_LONG) 
    {
      bkpts.listBkptAsRL = g_list_append(bkpts.listBkptAsRL, GINT_TO_POINTER(address));
      return g_list_length(bkpts.listBkptAsRL);
    }
    else if (mode==BK_WRITE_BYTE) 
    {
      bkpts.listBkptAsWB = g_list_append(bkpts.listBkptAsWB, GINT_TO_POINTER(address));
      return g_list_length(bkpts.listBkptAsWB);
    }
    else if (mode==BK_WRITE_WORD) 
    {
      bkpts.listBkptAsWW = g_list_append(bkpts.listBkptAsWW, GINT_TO_POINTER(address));
      return g_list_length(bkpts.listBkptAsWW);
    }
    else if (mode==BK_WRITE_LONG) 
    {
      bkpts.listBkptAsWL = g_list_append(bkpts.listBkptAsWL, GINT_TO_POINTER(address));
      return g_list_length(bkpts.listBkptAsWL);
    }
    else
        return ERR_68K_INVALID_MODE;
}

int ti68k_bkpt_set_vector(int vector) 
{
    assert( (vector >= 2) && (vector < 16) );
    bkpts.listBkptVector[vector] = !0;
    bkpts.nBkptVector++;
    return (bkpts.nBkptVector-1);
}

int ti68k_bkpt_set_autoint(int autoint)
{
    assert( (autoint >= 1) && (autoint < 8) );
    bkpts.listBkptAutoint[autoint] = !0;
    bkpts.nBkptAutoint++;
    return (bkpts.nBkptAutoint-1);
}

int ti68k_bkpt_set_trap(int trap) 
{
    assert( (trap >= 0) && (trap < 16) );
    bkpts.listBkptTrap[trap] = !0;
    bkpts.nBkptTrap++;
    return (bkpts.nBkptTrap-1);
}


void ti68k_bkpt_del_address(int i) 
{
    bkpts.listBkptAddress = g_list_delete_link(bkpts.listBkptAddress, g_list_nth(bkpts.listBkptAddress, i));
}

void ti68k_bkpt_del_access(int i, int mode) 
{
    if (mode==BK_READ_BYTE) 
    {		
        bkpts.listBkptAsRB = g_list_delete_link(bkpts.listBkptAsRB, g_list_nth(bkpts.listBkptAsRB, i));
    }
    if (mode==BK_READ_WORD) 
    {
      bkpts.listBkptAsRW = g_list_delete_link(bkpts.listBkptAsRW, g_list_nth(bkpts.listBkptAsRW, i));
    }
    if (mode==BK_READ_LONG) 
    {
      bkpts.listBkptAsRL = g_list_delete_link(bkpts.listBkptAsRL, g_list_nth(bkpts.listBkptAsRL, i));
    }
    if (mode==BK_WRITE_BYTE) 
    {
      bkpts.listBkptAsWB = g_list_delete_link(bkpts.listBkptAsWB, g_list_nth(bkpts.listBkptAsWB, i));
    }
    if (mode==BK_WRITE_WORD) 
    {
      bkpts.listBkptAsWW = g_list_delete_link(bkpts.listBkptAsWW, g_list_nth(bkpts.listBkptAsWW, i));
    }
    if (mode==BK_WRITE_LONG) 
    {
      bkpts.listBkptAsWL = g_list_delete_link(bkpts.listBkptAsWL, g_list_nth(bkpts.listBkptAsWL, i));
    }
}

void ti68k_bkpt_delaccess_range(int i,int mode) 
{
    if (mode & BK_READ) 
    {
        GList *elt = g_list_nth(bkpts.listBkptAsRgR, i);

        g_free(elt->data);
        bkpts.listBkptAsRgR = g_list_delete_link(bkpts.listBkptAsRgR, elt);
    }
    else if (mode & BK_WRITE) 
    {
        GList *elt = g_list_nth(bkpts.listBkptAsRgW, i);

        g_free(elt->data);
        bkpts.listBkptAsRgW = g_list_delete_link(bkpts.listBkptAsRgW, elt);
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
    bkpts.listBkptVector[vector] = 0;
    bkpts.nBkptVector--;
}

void ti68k_bkpt_del_autoint(int autoint)
{
    assert( (autoint >= 1) && (autoint < 8) );
    bkpts.listBkptAutoint[autoint] = 0;
    bkpts.nBkptAutoint--;
}

void ti68k_bkpt_del_trap(int trap) 
{
    assert( (trap >= 0) && (trap < 16) );
    bkpts.listBkptTrap[trap] = 0;
    bkpts.nBkptTrap--;
}

void ti68k_bkpt_get_cause(int *type, int *id, int *mode) 
{
    *type = bkpts.breakType;
    *mode = bkpts.breakMode;
    *id = bkpts.breakId;
}

void ti68k_bkpt_clear_address(void)
{
	g_list_free(bkpts.listBkptAddress);
	bkpts.listBkptAddress = NULL;
}

void ti68k_bkpt_clear_access(void)
{
	g_list_free(bkpts.listBkptAsRB);
	bkpts.listBkptAsRB = NULL;
	g_list_free(bkpts.listBkptAsRW);
	bkpts.listBkptAsRW = NULL;
	g_list_free(bkpts.listBkptAsRL);
	bkpts.listBkptAsRL = NULL;

	g_list_free(bkpts.listBkptAsWB);
	bkpts.listBkptAsWB = NULL;
	g_list_free(bkpts.listBkptAsWW);
	bkpts.listBkptAsWW = NULL;
	g_list_free(bkpts.listBkptAsWL);
	bkpts.listBkptAsWL = NULL;
}

void ti68k_bkpt_clear_access_range(void)
{
	GList *l;

	l = bkpts.listBkptAsRgR;
	while(l != NULL)
	{
        g_free(l->data);		
		l = g_list_next(l);
	}			
	g_list_free(bkpts.listBkptAsRgR);
	bkpts.listBkptAsRgR = NULL;

	l = bkpts.listBkptAsRgW;
	while(l != NULL)
	{
	    g_free(l->data);		
		l = g_list_next(l);
	}			
	g_list_free(bkpts.listBkptAsRgW);
	bkpts.listBkptAsRgW = NULL;
}

void ti68k_bkpt_clear_vector(void)
{
	memset(bkpts.listBkptVector, 0, 16);
	bkpts.nBkptVector = 0;
}

void ti68k_bkpt_clear_autoint(void)
{
	memset(bkpts.listBkptTrap, 0, 16);
	bkpts.nBkptTrap = 0;
}

void ti68k_bkpt_clear_trap(void)
{
	memset(bkpts.listBkptAutoint, 0, 8);
	bkpts.nBkptAutoint = 0;
}
