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

#include "libuae.h"
#include "ti68k_def.h"
#include "mem.h"
#include "hw.h"
#include "bkpts.h"
#include "images.h"

int hw_m68k_init(void)
{
    // init breakpoints
    ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_exception();
    bkpts.mode = bkpts.type = bkpts.id = 0;

    // set trap on illegal instruction
    ti68k_bkpt_set_exception(4);

    // init instruction logging
    bkpts.pclog_size = 10;
    bkpts.pclog_buf = (uint32_t *)malloc(bkpts.pclog_size * sizeof(uint32_t));
    if(bkpts.pclog_buf == NULL)
        return ERR_MALLOC;
    bkpts.pclog_ptr = 0;

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
    ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_exception();

    free(bkpts.pclog_buf);

    return 0;
}

void hw_m68k_irq(int n)
{
	specialflags |= SPCFLAG_INT;
    currIntLev = n;
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
    static FILE *flog;
    //static once = 0;
  
    for(i = 0; i < n; i++) 
    {
        UWORD opcode;

		// V200 debug purpose
	  /*
	  if(!once && (m68k_getpc() < 0x200000 || m68k_getpc() >= 0x600000))
	  {
		  printf("<%06x>\n", m68k_getpc());
			bkpts.type = BK_TYPE_CODE;
		    specialflags |= SPCFLAG_BRK;
			once = !0;
	  }
	  */
		
        // save log to disk (internal use)
        if(flog != NULL)
        {
            //fprintf(flog, "0x%06lx\n", m68k_getpc());
        }
        else
            flog = fopen("C:\\tiemu.log", "wt");        

        // search for breakpoint
        if(((l = bkpts.code) != NULL) && !(specialflags & SPCFLAG_DBSKIP))
        {
            bkpts.id = 0;
            while(l)
            {
                if(GPOINTER_TO_INT(l->data) == (int)m68k_getpc())
                {
                    bkpts.type = BK_TYPE_CODE;
		            //specialflags |= SPCFLAG_BRK;
                    return 1;
                }

                bkpts.id++;
                l = g_list_next(l);
            }
        }

        // store PC in the trace buffer
        if(bkpts.pclog_size > 1)
        {
            bkpts.pclog_buf[bkpts.pclog_ptr++ % bkpts.pclog_size] = m68k_getpc();
        }

        // search for next opcode, execute it and refresh hardware (if not STOP'ed)
		if (!(specialflags & SPCFLAG_STOP))
		{
			opcode = nextiword();
			(*cpufunctbl[opcode])(opcode);
		}
        do_cycles();

        // management of special flags
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
	      
			if (specialflags & SPCFLAG_STOP)	//while (specialflags & SPCFLAG_STOP) 
	        {
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
	            if (intr != -1 && intr > regs.intmask) 
                {
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

            if(specialflags & SPCFLAG_DBSKIP)
                specialflags &= ~SPCFLAG_DBSKIP;
	    }  
    }
  return 0;
}
