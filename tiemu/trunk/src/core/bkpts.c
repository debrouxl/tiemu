/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
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

#include <stdlib.h>
#include <assert.h>

#include "uae.h"
#include "bkpts.h"
#include "intlist.h"
#include "errcodes.h"
#include "intl.h"
#include "tilibs.h"

/* Variables */

// Memory
int breakType = 0;
int breakMode = 0;
int breakId   = 0;

struct intlist *listBkptAsRB;  int nBkptAsRB;
struct intlist *listBkptAsWB;  int nBkptAsWB;
struct intlist *listBkptAsRW;  int nBkptAsRW;
struct intlist *listBkptAsWW;  int nBkptAsWW;
struct intlist *listBkptAsRL;  int nBkptAsRL;
struct intlist *listBkptAsWL;  int nBkptAsWL;
struct intlist *listBkptAsRgR; int nBkptAsRgR;
struct intlist *listBkptAsRgW; int nBkptAsRgW;

// Code
struct intlist *listBkptAddress = 0;
int nBkptAddress = 0;

// Vectors & Traps
int listBkptVector[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int listBkptTrap[16]   = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int listBkptAutoint[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int nBkptVector = 0;
int nBkptTrap = 0; 
int nBkptAutoint = 0;


/* Functions */

int ti68k_setBreakpointAddress(int address) 
{
  addEnd(&listBkptAddress, address);
  nBkptAddress++;
  return (nBkptAddress-1);
}

int ti68k_setBreakpointAddressRange(int addressMin, int addressMax, int mode) 
{
  if(mode & BK_READ) 
    {		
      add2End(&listBkptAsRgR,addressMin,addressMax);
      return nBkptAsRgR++;
    }
  if(mode & BK_WRITE) 
    {
      add2End(&listBkptAsRgW,addressMin,addressMax);
      return nBkptAsRgW++;
    }
  return ERR_INVALID_MODE;
}

int ti68k_setBreakpointAccess(int address, int mode) 
{
  if(mode==BK_READ_BYTE) 
    {		
      addEnd(&listBkptAsRB, address);
      return nBkptAsRB++;
    }
  else if(mode==BK_READ_WORD) 
    {
      addEnd(&listBkptAsRW, address);
      return nBkptAsRW++;
    }
  else if(mode==BK_READ_LONG) 
    {
      addEnd(&listBkptAsRL,address);
      return nBkptAsRL++;
    }
  else if (mode==BK_WRITE_BYTE) 
    {
      addEnd(&listBkptAsWB,address);
      return nBkptAsWB++;
    }
  else if (mode==BK_WRITE_WORD) 
    {
      addEnd(&listBkptAsWW,address);
      return nBkptAsWW++;
    }
  else if (mode==BK_WRITE_LONG) 
    {
      addEnd(&listBkptAsWL,address);
      return nBkptAsWL++;
    }
  else
    return ERR_INVALID_MODE;
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
  delAt(&listBkptAddress, i);
}

void ti68k_delBreakpointAccess(int i, int mode) 
{
  if (mode==BK_READ_BYTE) 
    {		
      delAt(&listBkptAsRB,i);
      nBkptAsRB--;
    }
  if (mode==BK_READ_WORD) 
    {
      delAt(&listBkptAsRW,i);
      nBkptAsRW--;
    }
  if (mode==BK_READ_LONG) 
    {
      delAt(&listBkptAsRL,i);
      nBkptAsRL--;
    }
  if (mode==BK_WRITE_BYTE) 
    {
      delAt(&listBkptAsWB,i);
      nBkptAsWB--;
    }
  if (mode==BK_WRITE_WORD) 
    {
      delAt(&listBkptAsWW,i);
      nBkptAsWW--;
    }
  if (mode==BK_WRITE_LONG) 
    {
      delAt(&listBkptAsWL,i);
      nBkptAsWL--;
    }
}

void ti68k_delBreakpointAccessRange(int i,int mode) 
{
  if (mode & BK_READ) 
    {
      delAt(&listBkptAsRgR,i);
      nBkptAsRgR--;
    }
  else if (mode & BK_WRITE) 
    {
      delAt(&listBkptAsRgW,i);
      nBkptAsRgW--;
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
  //return (nBkptVector-1);
}

void ti68k_delBreakpointAutoint(int autoint)
{
  assert( (autoint >= 1) && (autoint < 8) );
  listBkptAutoint[autoint] = 0;
  nBkptAutoint--;
  //return (nBkptAutoint-1);
}

void ti68k_delBreakpointTrap(int trap) 
{
  assert( (trap >= 0) && (trap < 16) );
  listBkptTrap[trap] = 0;
  nBkptTrap--;
  //return (nBkptTrap-1);
}
