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
    M68K management (wrapper for the UAE engine)
*/

#include "libuae.h"
#include "ti68k_def.h"
#include "mem.h"
#include "hw.h"
#include "bkpts.h"
#include "images.h"

int pending_ints;

int hw_m68k_init(void)
{
    // init breakpoints
    ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_exception();
    bkpts.mode = bkpts.type = bkpts.id = 0;

    // set trap on illegal instruction
    ti68k_bkpt_add_exception(4);

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

	pending_ints = 0;

    return 0;
}

int hw_m68k_exit(void)
{
    ti68k_bkpt_clear_address();
	ti68k_bkpt_clear_exception();

    free(bkpts.pclog_buf);

    return 0;
}

// UAE does not implement interrupt priority and pending interrupts
// I re-implement it as replacement of Interrupt()
void Interrupt2(int nr)
{
	//if((nr > regs.intmask) || (nr == 7))	// ok, if idle disabled (bug ?!)
		Interrupt(nr);

	pending_ints &= ~(1 << nr);
}

void hw_m68k_irq(int n)
{
	specialflags |= SPCFLAG_INT;
    currIntLev = n;

	pending_ints |= (1 << n);
}

#define GET_INT_LVL	\
		int level, mask = 0x80;	\
			for (level = 7; level; level--, mask >>= 1)	\
				if (pending_ints & mask)	\
					break;

extern void lcd_hook_hw2(int);

/* Replace UAE's M68000_run() */
/*
  Do 'n' instructions.
  return ERR_NONE if successful, a negative value if an error occured,
  a positive value if a breakpoint has been encountered.
*/
int hw_m68k_run(int n)
{
    int i=n;
    GList *l = NULL;
    static FILE *f = NULL;
    static foo = 0;

#if 1

	for(i = 0; i < n; i++)
	{
		UWORD opcode;

		// refresh hardware
		do_cycles();
		// used by grayscale for time plane exposure
		tihw.lcd_tick++;

		// OSC1 stopped ? Refresh hardware and wake-up on interrupt. No opcode execution.
		if ((specialflags & SPCFLAG_STOP))
	    {
			if(pending_ints)
			{
				GET_INT_LVL;

				// wake-up on int level 6 (ON key) or level 1..5
				if ((pending_ints & (tihw.io[5] << 1)) || (level == 6))
				{
					Interrupt2(level);
					regs.stopped = 0;
					specialflags &= ~SPCFLAG_STOP;
					//printf("R");
				}
			}

			continue;
	    }		

		// search for breakpoint
        if(((l = bkpts.code) != NULL) && !(specialflags & SPCFLAG_DBSKIP))
        {
            bkpts.id = 0;
            while(l)
            {
                if(BKPT_ADDR(GPOINTER_TO_INT(l->data)) == (int)m68k_getpc())
                {
					if(BKPT_IS_TMP(GPOINTER_TO_INT(l->data)))
						bkpts.code = g_list_remove(bkpts.code, l->data);

                    bkpts.type = BK_TYPE_CODE;
		            //specialflags |= SPCFLAG_BRK;
                    return 1;
                }

                bkpts.id++;
                l = g_list_next(l);
            }
        }

        // store PC in the log buffer
        if(bkpts.pclog_size > 1)
        {
            bkpts.pclog_buf[bkpts.pclog_ptr++ % bkpts.pclog_size] = m68k_getpc();
        }

		// search for next opcode and execute it
		opcode = nextiword();
		(*cpufunctbl[opcode])(opcode);

		// HW2/3 grayscales management
		lcd_hook_hw2(0);

		// process (pending) interrupts
		if(pending_ints)
		{
			GET_INT_LVL;
			if(level > regs.intmask)
				Interrupt2 (level);
			regs.stopped = 0;
		}

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
	      
	        if (specialflags & SPCFLAG_TRACE) 
	        {
	            specialflags &= ~SPCFLAG_TRACE;
	            specialflags |= SPCFLAG_DOTRACE;
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

#else

    for(i = 0; i < n; i++) 
    {
        UWORD opcode;

        // search for breakpoint
        if(((l = bkpts.code) != NULL) && !(specialflags & SPCFLAG_DBSKIP))
        {
            bkpts.id = 0;
            while(l)
            {
                if(BKPT_ADDR(GPOINTER_TO_INT(l->data)) == (int)m68k_getpc())
                {
					if(BKPT_IS_TMP(GPOINTER_TO_INT(l->data)))
						bkpts.code = g_list_remove(bkpts.code, l->data);

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

		// TI89 HW1 AMS203 and gray3pt() test program
		//if((int)m68k_getpc() == 0x3f7e0) printf(".");
		//if((int)m68k_getpc() == 0x2a2568) printf(".");

        // search for next opcode, execute it and refresh hardware (if not STOP'ed)
		if (!((specialflags & SPCFLAG_STOP) && !(specialflags & SPCFLAG_DBSKIP)))
		{
			opcode = nextiword();
			(*cpufunctbl[opcode])(opcode);
		}

		// refresh hardware
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

			// while (specialflags & SPCFLAG_STOP)
			// need to be re-entrant so that hardware (ON key) is still running.
			if ((specialflags & SPCFLAG_STOP) && (specialflags & (SPCFLAG_INT | SPCFLAG_DOINT)))
	        {
				int intr = intlev();
				// wake on int level 6 (ON key) or level 1..5
				specialflags &= ~(SPCFLAG_INT | SPCFLAG_DOINT);
				if(
					((intr == 5) && (tihw.io[0x05] & 0x10)) ||
					((intr == 4) && (tihw.io[0x05] & 0x08)) ||
					((intr == 3) && (tihw.io[0x05] & 0x04)) ||
					((intr == 2) && (tihw.io[0x05] & 0x02)) ||
					((intr == 1) && (tihw.io[0x05] & 0x01))
					)
				{
					Interrupt2(intr);
					regs.stopped = 0;
					specialflags &= ~SPCFLAG_STOP;
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
		            Interrupt2(intr);
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

#endif

	return 0;
}
