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
#include <glib.h>

#include "uae.h"
#include "bkpts.h"
#include "intl.h"
#include "tilibs.h"
#include "ti68k_err.h"

/* Variables */

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


/* Functions */

int ti68k_setBreakpointAddress(int address) 
{
    listBkptAddress = g_list_append(listBkptAddress, GINT_TO_POINTER(address));
    return g_list_length(listBkptAddress);
}

int ti68k_setBreakpointAddressRange(int addressMin, int addressMax, int mode) 
{
    if(mode & BK_READ) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

        listBkptAsRgR = g_list_append(listBkptAsRgR, s);
        return g_list_length(listBkptAsRgR);
    }

    if(mode & BK_WRITE) 
    {
        ADDR_RANGE *s = g_malloc(sizeof(ADDR_RANGE));

        listBkptAsRgW = g_list_append(listBkptAsRgW, s);
        return g_list_length(listBkptAsRgW);
    }
    
    return ERR_68K_INVALID_MODE;
}

int ti68k_setBreakpointAccess(int address, int mode) 
{
    if(mode==BK_READ_BYTE) 
    {		
        listBkptAsRB = g_list_append(listBkptAsRB, GINT_TO_POINTER(address));
        return g_list_length(listBkptAsRB);
    }
    else if(mode==BK_READ_WORD) 
    {
      listBkptAsRW = g_list_append(listBkptAsRW, GINT_TO_POINTER(address));
      return g_list_length(listBkptAsRW);
    }
    else if(mode==BK_READ_LONG) 
    {
      listBkptAsRL = g_list_append(listBkptAsRL, GINT_TO_POINTER(address));
      return g_list_length(listBkptAsRL);
    }
    else if (mode==BK_WRITE_BYTE) 
    {
      listBkptAsWB = g_list_append(listBkptAsWB, GINT_TO_POINTER(address));
      return g_list_length(listBkptAsWB);
    }
    else if (mode==BK_WRITE_WORD) 
    {
      listBkptAsWW = g_list_append(listBkptAsWW, GINT_TO_POINTER(address));
      return g_list_length(listBkptAsWW);
    }
    else if (mode==BK_WRITE_LONG) 
    {
      listBkptAsWL = g_list_append(listBkptAsWL, GINT_TO_POINTER(address));
      return g_list_length(listBkptAsWL);
    }
    else
        return ERR_68K_INVALID_MODE;
}

int ti68k_setBreakpointVector(int vector) 
{
    assert( (vector >= 2) && (vector < 16) );
    listBkptVector[vector] = !0;
    nBkptVector++;
    return (nBkptVector-1);
}

int ti68k_setBreakpointAutoint(int autoint)
{
    assert( (autoint >= 1) && (autoint < 8) );
    listBkptAutoint[autoint] = !0;
    nBkptAutoint++;
    return (nBkptAutoint-1);
}

int ti68k_setBreakpointTrap(int trap) 
{
    assert( (trap >= 0) && (trap < 16) );
    listBkptTrap[trap] = !0;
    nBkptTrap++;
    return (nBkptTrap-1);
}


void ti68k_delBreakpointAddress(int i) 
{
    listBkptAddress = g_list_delete_link(listBkptAddress, g_list_nth(listBkptAddress, i));
}

void ti68k_delBreakpointAccess(int i, int mode) 
{
    if (mode==BK_READ_BYTE) 
    {		
        listBkptAsRB = g_list_delete_link(listBkptAsRB, g_list_nth(listBkptAsRB, i));
    }
    if (mode==BK_READ_WORD) 
    {
      listBkptAsRW = g_list_delete_link(listBkptAsRW, g_list_nth(listBkptAsRW, i));
    }
    if (mode==BK_READ_LONG) 
    {
      listBkptAsRL = g_list_delete_link(listBkptAsRL, g_list_nth(listBkptAsRL, i));
    }
    if (mode==BK_WRITE_BYTE) 
    {
      listBkptAsWB = g_list_delete_link(listBkptAsWB, g_list_nth(listBkptAsWB, i));
    }
    if (mode==BK_WRITE_WORD) 
    {
      listBkptAsWW = g_list_delete_link(listBkptAsWW, g_list_nth(listBkptAsWW, i));
    }
    if (mode==BK_WRITE_LONG) 
    {
      listBkptAsWL = g_list_delete_link(listBkptAsWL, g_list_nth(listBkptAsWL, i));
    }
}

void ti68k_delBreakpointAccessRange(int i,int mode) 
{
    if (mode & BK_READ) 
    {
        GList *elt = g_list_nth(listBkptAsRgR, i);

        g_free(elt->data);
        listBkptAsRgR = g_list_delete_link(listBkptAsRgR, elt);
    }
    else if (mode & BK_WRITE) 
    {
        GList *elt = g_list_nth(listBkptAsRgW, i);

        g_free(elt->data);
        listBkptAsRgW = g_list_delete_link(listBkptAsRgW, elt);
    }
    else
    {
        DISPLAY(_("delBreakpointAccessRange: mode is neither READ nor WRITE. This is a bug. Please report it !\n"));
        exit(-1);
    }
}

void ti68k_delBreakpointVector(int vector) 
{
    assert( (vector >= 2) && (vector < 16) );
    listBkptVector[vector] = 0;
    nBkptVector--;
}

void ti68k_delBreakpointAutoint(int autoint)
{
    assert( (autoint >= 1) && (autoint < 8) );
    listBkptAutoint[autoint] = 0;
    nBkptAutoint--;
}

void ti68k_delBreakpointTrap(int trap) 
{
    assert( (trap >= 0) && (trap < 16) );
    listBkptTrap[trap] = 0;
    nBkptTrap--;
}
