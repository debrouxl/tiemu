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
  M68K management (wrapper for the UAE engine)
*/

#include "uae.h"
#include "memory.h"
#include "hardware.h"
#include "bkpts.h"
#include "images.h"
//#include "interface.h"
#include "callbacks.h"

void hw_init_m68k()
{
  init_m68k();
}

void hw_reset_m68k()
{
  find_pc();
  rom_at_0();
  MC68000_reset();
  ram_at_0();
}

void hw_exit_m68k()
{
}

/* Replace UAE's M68000_run() */
/*
  Do 'n' instructions.
  return ERR_NONE if successful, a negative value if an error occured,
  a positive value if a breakpoint has been encountered.
*/
int hw_run_m68k(int n)
{
  int i;
  struct intlist *l;
  //char inst[100];
  
  for(i=0; i<n; i++) 
    {
      UWORD opcode;

      opcode = nextiword();
      (*cpufunctbl[opcode])(opcode);
      do_cycles();

      if(nBkptAddress) 
	{
	  l=listBkptAddress;
	  breakId = 0;
	  while (l) 
	    {
	      if (l->val==(int)regs.pc)
		{
		  //DISPLAY("l->val=0x%06x, pc=0x%06x\n", 
		  //  l->val, (int)regs.pc);
		  breakType = BK_CAUSE_ADDRESS;
		  cb_update_screen();
		  cb_launch_debugger();

		  return 1;
		}
	      breakId++;
	      l=l->next;
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
	      cb_update_screen();
	      cb_launch_debugger();
	      return 1;		
	    }
	  if(specialflags & SPCFLAG_DBTRACE) 
	    {
	      specialflags &= ~SPCFLAG_DBTRACE;
	      cb_update_screen();
	      cb_launch_debugger();
	    }
	}  
    }

  return 0;
}
