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

#ifndef __IUPDATE__
#define __IUPDATE__

extern Ti68kInfoUpdate *iupdate;            // defined in lib68k.c

/* Functions for updating */
/*
  void (*start)   (void);                   // Init internal variables
  void (*msg_box) (const char *t, char *s); // Display a message box
  void (*pbar)    (void);                   // Refresh the progress bar
  void (*label)   (void);                   // Refresh the label
  void (*refresh) (void);                   // Pass control to GUI for refresh
  int  (*choose)  (char *cur_name, 
  char *new_name);         // Display choice box (skip, rename, overwrite, ... )
  void (*stop)    (void);
*/

/* 
   Macros: they check that function pointer is good and then
   call the pointer itself 
*/
#define iupdate_start()      if(iupdate && iupdate->start)   iupdate->start()
#define iupdate_msgbox(t, s) if(iupdate && iupdate->msg_box) iupdate->msg_box(t, s)
#define iupdate_pbar()       if(iupdate && iupdate->pbar)    iupdate->pbar()
#define iupdate_label()      if(iupdate && iupdate->label)   iupdate->label()
#define iupdate_refresh()    if(iupdate && iupdate->refresh) iupdate->refresh()
#define iupdate_stop()       if(iupdate && iupdate->stop)    iupdate->stop()
/*
int inline iupdate_choose(char* c, char* n)
{ if(iupdate && iupdate->choose) return iupdate->choose(c, n); else return 0; }
*/
#endif
