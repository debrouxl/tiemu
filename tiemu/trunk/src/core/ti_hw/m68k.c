/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 245 2004-05-23 20:45:43Z roms $ */

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
    M68K management (wrapper for the UAE engine)
*/

#include "uae.h"
#include "ti68k_def.h"
#include "memory.h"
#include "hardware.h"
#include "bkpts.h"
#include "images.h"
#include "callbacks.h"

int hw_m68k_init(void)
{
	ti68k_bkpt_clear_exception();
	ti68k_bkpt_set_exception(4);	// illegal instruction
    bkpts.mode = bkpts.type = bkpts.id = 0;

    init_m68k();

    return 0;
}

int hw_m68k_reset(void)
{
    rom_at_0();
    MC68000_reset();
    ram_at_0();

    return 0;
}

int hw_m68k_exit(void)
{
	ti68k_bkpt_clear_exception();

    return 0;
}

/* Replace UAE's M68000_run() */
/*
  Do 'n' instructions.
  return ERR_NONE if successful, a negative value if an error occured,
  a positive value if a breakpoint has been encountered.
*/
int hw_m68k_run(int n)
{
  int i;
  GList *l;
  //char inst[100];
  
  for(i=0; i<n; i++) 
    {
      UWORD opcode;

      opcode = nextiword();
      (*cpufunctbl[opcode])(opcode);
      do_cycles();

      if((l = bkpts.code) != NULL)
      {
          bkpts.id = 0;
          while(l)
          {
              if(GPOINTER_TO_INT(l->data) == (int)regs.pc)
              {
                bkpts.type = BK_TYPE_CODE;
		        specialflags |= SPCFLAG_BRK;
              }

              bkpts.id++;
              l = g_list_next(l);
          }
      }

      /* Debug purposes */
#if 0
      if (bInstructionsDisplay) 
	{
	  disasm(getPcRegister(), inst);
	  printf("disasm: %s\n", inst);
	}
#endif
      /* Flag management */      
      if(specialflags) 
	{
	  if(specialflags & SPCFLAG_ADRERR) 
	    {
	      Exception(3);
	      specialflags &= ~SPCFLAG_ADRERR;
	    }
	  
	  if (specialflags & SPCFLAG_DOTRACE) 
	    {
	      Exception(9);
	    }
	  
	  while (specialflags & SPCFLAG_STOP) 
	    {
	      do_cycles();
	      if (specialflags & (SPCFLAG_INT | SPCFLAG_DOINT)) 
		{
		  int intr = intlev();
		  specialflags &= ~(SPCFLAG_INT | SPCFLAG_DOINT);
		  if (intr != -1 && intr > regs.intmask) 
		    {
		      Interrupt(intr);
		      regs.stopped = 0;
		      specialflags &= ~SPCFLAG_STOP;
		    }	    
		}
	    }		
	  
	  if (specialflags & SPCFLAG_TRACE) 
	    {
	      specialflags &= ~SPCFLAG_TRACE;
	      specialflags |= SPCFLAG_DOTRACE;
	    }	  
	  if (specialflags & SPCFLAG_DOINT) 
	    {
	      int intr = intlev();
	      specialflags &= ~(SPCFLAG_INT | SPCFLAG_DOINT);
	      if (intr != -1 && intr > regs.intmask) {
		Interrupt(intr);
		regs.stopped = 0;
	      }	    
	    }
	  if (specialflags & SPCFLAG_INT) 
	    {
	      specialflags &= ~SPCFLAG_INT;
	      specialflags |= SPCFLAG_DOINT;
	    }
	  if (specialflags & SPCFLAG_BRK) 
	    {		
	      specialflags &= ~SPCFLAG_BRK;
	      return 1;		// DBG_BREAK
	    }
	  if(specialflags & SPCFLAG_DBTRACE) 
	    {
	      specialflags &= ~SPCFLAG_DBTRACE;
          return 2;     // DBG_TRACE
	    }
	}  
    }

  return 0;
}
