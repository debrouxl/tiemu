/* 
  * UAE - The Un*x Amiga Emulator
  * 
  * MC68000 emulation
  *
  * (c) 1995 Bernd Schmidt
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "sysdeps.h"
#include "uconfig.h"
#include "options.h"
#include "memory.h"
#include "newcpu.h"
#include "readcpu.h"

// tiemu begin
#include "tilibs.h"
#include "ti68k_def.h"
#include "memory.h"
#include "../ti_hw/dbus.h"
#include "bkpts.h"
#include "hw.h" //do_cycles
// tiemu end

int areg_byteinc[] = { 1,1,1,1,1,1,1,2 };
int imm8_table[] = { 8,1,2,3,4,5,6,7 };

cpuop_func *cpufunctbl[65536];
unsigned long specialflags;
int currIntLev = -1;
int broken_in;
int delayTime = 0;

#ifdef INTEL_FLAG_OPT
union flagu intel_flag_lookup[256];
#endif

#ifdef PENT_COUNTER

/*
  On Pentium processors (IA-32) or equivalent (I have an AMD K6), there is an
  64-bits timer called TSC (Timer Stamp Counter) which counts the number of CPU cycles.
*/
#if defined(__LINUX__)
unsigned long highCnt;
# define readpec(l) __asm__(".byte 0x0f, 0x31" :"=a" (l), "=d" (highCnt))
#elif defined(__WIN32__)
static _inline readpec(l) {
	__asm {
		rdtsc		;
		mov	l,eax		;lsw
	};	
};

/*
static _inline double getAbsoluteCPUcycle(void) {
	unsigned __int32 temp[2];	//__int64

	__asm {
		rdtsc					; edx:eax
		mov		temp,eax		; little endian
		mov		temp[4],edx		; msvc byte offset
	}

	return temp[1]*4294967296.0 + (*temp));
}
*/

#else
# define readpec(l) 
#endif

#endif /* PENT_COUNTER */

/* Pentium ticks between do_period() calls */
unsigned long timeDiff = 0;
/* Pentium ticks betweein instructions */
unsigned long cycleDiff = 0;
/* Pentium ticks on one millisecond */
unsigned long permSec = 0;

//tiemu begin
unsigned long lastTime = 0;
unsigned long cycleRate = 0;
unsigned long tickRate = 0;
//tiemu end

#ifdef PENT_COUNTER

#if defined(__UNIX__) || defined(__LINUX__) || defined(__MACOSX__)
# include <unistd.h>
# define PAUSE(x)  usleep(1000*(x));
#elif defined(__WIN32__)
# include <windows.h>
# define PAUSE(x)  Sleep((x));
#endif


#define NMS 1000 // number of ms
float calibrate_pcounter(void) 
{
  unsigned long  i,j; 
  float pfreq = 0;

  readpec(i);
  PAUSE(NMS);
  readpec(j);
    
  if(i > j) 
    {
      readpec(i);
      PAUSE(NMS);
      readpec(j);
    }

  permSec = (j-i)/1000; // proc freq in kHz
  pfreq = (float)(j-i)/1000/NMS;
  fprintf(stderr, "UAE: calibration loop gives %f MHz\n", pfreq);
  
  //tickRate = params.tick_rate;
  //cycleRate = params.cycle_rate;
  
  //timeDiff = ((unsigned long)permSec * (unsigned long)tickRate)/100000;
  //cycleDiff = (permSec*cycleRate)/100000;
  
  return pfreq;
}

#endif /* PENT_COUNTER */

/* 
   Build CPU table 
*/
void init_m68k (void)
{
    long int opcode;
    int i;

#ifdef INTEL_FLAG_OPT
    for (i = 0; i < 256; i++) 
      {
	intel_flag_lookup[i].flags.c = !!(i & 1);
	intel_flag_lookup[i].flags.z = !!(i & 64);
	intel_flag_lookup[i].flags.n = !!(i & 128);
	intel_flag_lookup[i].flags.v = 0;
      }
#endif
    printf("UAE: building CPU table...\n");
    read_table68k ();
    do_merges ();
    for (opcode = 0; opcode < 65536; opcode++)
	cpufunctbl[opcode] = op_illg;
    for (i = 0; smallcputbl[i].handler != NULL; i++) 
      {
	if (!smallcputbl[i].specific)
	  cpufunctbl[smallcputbl[i].opcode] = smallcputbl[i].handler;
      }
    for (opcode = 0; opcode < 65536; opcode++) 
      {
	cpuop_func *f;
	
	if (table68k[opcode].mnemo == i_ILLG)
	  continue;
	
	if (table68k[opcode].handler != -1) 
	  {
	    f = cpufunctbl[table68k[opcode].handler];
	    if (f == op_illg)
	      abort();
	    cpufunctbl[opcode] = f;
	  }
      }	
    for (i = 0; smallcputbl[i].handler != NULL; i++) 
      {
	if (smallcputbl[i].specific)
	  cpufunctbl[smallcputbl[i].opcode] = smallcputbl[i].handler;
      }
}

struct regstruct regs;
union flagu regflags;

static void ShowEA(int reg, amodes mode, wordsizes size,char *output)
{
    UWORD dp;
    BYTE disp8;
    WORD disp16;
    int r;
    ULONG dispreg;
    CPTR addr;
	char temp[40];
    
    switch(mode){
     case Dreg:
	sprintf(temp,"D%d", reg);
	strcat(output,temp);
	break;
     case Areg:
	sprintf(temp,"A%d", reg);
	strcat(output,temp);
	break;
     case Aind:
	sprintf(temp,"(A%d)", reg);
	strcat(output,temp);
	break;
     case Aipi:
	sprintf(temp,"(A%d)+", reg);
	strcat(output,temp);
	break;
     case Apdi:
	sprintf(temp,"-(A%d)", reg);
	strcat(output,temp);
	break;
     case Ad16:
	disp16 = nextiword();
	addr = regs.a[reg] + (WORD)disp16;
	sprintf(temp,"(A%d,$%04lx) == $%08lx", reg, disp16, (long unsigned int)addr);
	strcat(output,temp);
	break;
     case Ad8r:
	dp = nextiword();
	disp8 = dp & 0xFF;
	r = (dp & 0x7000) >> 12;
	dispreg = dp & 0x8000 ? regs.a[r] : regs.d[r];	
	if (!(dp & 0x800)) dispreg = (LONG)(WORD)(dispreg);
	
	addr = regs.a[reg] + disp8 + dispreg;
	sprintf(temp,"(A%d, %c%d.%c, $%02x) == $%08lx", reg, 
	       dp & 0x8000 ? 'A' : 'D', (int)r, dp & 0x800 ? 'L' : 'W', disp8,
	       (long unsigned int)addr);
	strcat(output,temp);
	break;
     case PC16:
	addr = m68k_getpc();
	disp16 = nextiword();
	addr += (WORD)disp16;
	sprintf(temp,"(PC,$%08lx) == $%08lx", disp16, (long unsigned int)addr);
	strcat(output,temp);
	break;
     case PC8r:
	addr = m68k_getpc();
	dp = nextiword();
	disp8 = dp & 0xFF;
	r = (dp & 0x7000) >> 12;
	dispreg = dp & 0x8000 ? regs.a[r] : regs.d[r];
	
	if (!(dp & 0x800)) dispreg = (LONG)(WORD)(dispreg);
	addr += disp8 + dispreg;
	sprintf(temp,"(PC, %c%d.%c, $%02x) == $%08lx", dp & 0x8000 ? 'A' : 'D', 
	       (int)r, dp & 0x800 ? 'L' : 'W', disp8, (long unsigned int)addr);
	strcat(output,temp);
	break;
     case absw:
	sprintf(temp,"$%08lx", (LONG)(WORD)nextiword());
	strcat(output,temp);
	break;
     case absl:
	sprintf(temp,"$%08lx", nextilong());
	strcat(output,temp);
	break;
     case imm:
	switch(size){
	 case sz_byte:
	    sprintf(temp,"#$%02x", nextiword() & 0xff); 
		strcat(output,temp);
		break;
	 case sz_word:
	    sprintf(temp,"#$%04x", nextiword()); 
		strcat(output,temp);
		break;
	 case sz_long:
	    sprintf(temp,"#$%08lx", nextilong()); 
		strcat(output,temp);
		break;
	 default:
	    abort();
	}
	break;
     case imm0:
	sprintf(temp,"#$%02lx", nextiword() & 0xff);
	strcat(output,temp);
	break;
     case imm1:
	sprintf(temp,"#$%04lx", nextiword());
	strcat(output,temp);
	break;
     case imm2:
	sprintf(temp,"#$%08lx", nextilong());
	strcat(output,temp);
	break;
     case immi:
	sprintf(temp,"#$%04lx", reg);
	strcat(output,temp);
	break;
     default:
	//abort();
         break;
    }
}


int intlev(void) 
{
  int rc = currIntLev;
  currIntLev = -1;
  return rc;
}

//tiemu: added to be VTi compatible (VTi does not maintain SR unused bits at 0)
static char u14, u12, u11, u7, u6, u5;

void MakeSR(void)
{
  regs.sr = (
      (regs.t << 15) | 
      (u14 << 14) |
      (regs.s << 13) | 
      (u12 << 12) |
      (u11 << 11) |
      (regs.intmask << 8) |
	 
      (u7 << 7) |
      (u6 << 6) |
      (u5 << 5) |
      (regs.x << 4) | 
      (NFLG << 3) | 
      (ZFLG << 2) | 
      (VFLG << 1) |  
      (CFLG << 0)
      );
}

void MakeFromSR(void)
{
  int olds = regs.s;

  regs.t = (regs.sr >> 15) & 1;
  u14 = (regs.sr >> 14) & 1;
  regs.s = (regs.sr >> 13) & 1;
  u12 = (regs.sr >> 12) & 1;
  u11 = (regs.sr >> 11) & 1;
  regs.intmask = (regs.sr >> 8) & 7;

  u7 = (regs.sr >> 7) & 1;
  u6 = (regs.sr >> 6) & 1;
  u5 = (regs.sr >> 5) & 1;
  regs.x = (regs.sr >> 4) & 1;
  NFLG = (regs.sr >> 3) & 1;
  ZFLG = (regs.sr >> 2) & 1;
  VFLG = (regs.sr >> 1) & 1;
  CFLG = regs.sr & 1;
  if (olds != regs.s) 
    {
      CPTR temp = regs.usp;
      regs.usp = regs.a[7];
      regs.a[7] = temp;
    }
  
  //tiemu begin
  /*
    This part of code (turbo)boost the direct transfer of files through
    the internal link 
  */  
#if 1

  if(tihw.lc_speedy && regs.intmask < 4 && 
     (tihw.io[0xc]&0x2 || hw_dbus_checkread())) 
     //((tihw.io[0xc] & 2) && (tihw.io[0xd] & 0x40)) ||
	 //((tihw.io[0xc] & 1) && (tihw.io[0xd] & 0x20)))
    {
      tihw.io[0xc] |= 0x2;
      specialflags |= SPCFLAG_INT;
      currIntLev = 4;
    }
#endif
  //tiemu end

  specialflags |= SPCFLAG_INT;
  if (regs.t)
    specialflags |= SPCFLAG_TRACE;
  else
    specialflags &= ~(SPCFLAG_TRACE | SPCFLAG_DOTRACE);
}

void Exception(int nr)
{
	GList *l;

//printf("before: <0x%06lx>\n", m68k_getpc());

  MakeSR();
  if (!regs.s) 
    {
      CPTR temp = regs.usp;
      regs.usp = regs.a[7];
      regs.a[7] = temp;
      regs.s = 1;
    }
  
  regs.a[7] -= 4;
  put_long (regs.a[7], m68k_getpc ());
  regs.a[7] -= 2;
  put_word (regs.a[7], regs.sr);
  m68k_setpc(get_long(regs.vbr + 4*nr));
  regs.t = 0;
  specialflags &= ~(SPCFLAG_TRACE | SPCFLAG_DOTRACE);

//printf("after: <0x%06lx>\n", m68k_getpc());

  // added for capturing the exception and next launch a debugger
  if (l = bkpts.exception) 
    {
        bkpts.id = 0;
        while (l) 
	    {
	        if (GPOINTER_TO_INT(l->data) == nr) 
	        {
				bkpts.type = BK_CAUSE_EXCEPTION;
				bkpts.mode = nr;
	            specialflags |= SPCFLAG_BRK;	            
	            break;
	        }
	        
			bkpts.id++;
	        l = l->next;
	    }
    }
}

int uscycle = 0;

void Interrupt(int nr)
{
  assert(nr < 8 && nr >= 0);
  
  Exception(nr+24);
  
  regs.intmask = nr;
  specialflags |= SPCFLAG_INT;
}

static int caar, cacr;

void m68k_move2c (int regno, ULONG *regp)
{
  switch (regno) 
    {
    case 0: regs.sfc = *regp; break;
    case 1: regs.dfc = *regp; break;
    case 2: cacr = *regp & 0xFF; break;
    case 0x800: regs.usp = *regp; break;
    case 0x801: regs.vbr = *regp; break;
    case 0x802: caar = *regp & 0xFF; break;
    default:
      op_illg (0x4E7B);
      break;
    }
}

void m68k_movec2 (int regno, ULONG *regp)
{
  switch (regno) 
    {
    case 0: *regp = regs.sfc; break;
    case 1: *regp = regs.dfc; break;
    case 2: *regp = cacr; break;
    case 0x800: *regp = regs.usp; break;
    case 0x801: *regp = regs.vbr; break;
    case 0x802: *regp = caar; break;
    default:
      op_illg (0x4E7A);
      break;
    }
}

extern void m68k_divl (UWORD opcode, ULONG src, UWORD extra)
{
  if (src == 0)
    return;
#ifdef INT_64BIT
  if (extra & 0x800) 
    {
      /* signed variant */
      INT_64BIT a = regs.d[(extra >> 12) & 7];
      INT_64BIT quot, rem;
      
      if (extra & 0x400)
	a |= (INT_64BIT)regs.d[extra & 7] << 32;
      rem = a % src;
      quot = a / src;
      if ((extra & 0x400) && (extra & 7) != ((extra >> 12) & 7))
	regs.d[extra & 7] = rem;
      regs.d[(extra >> 12) & 7] = quot;
    } 
  else 
    {
      /* unsigned */
      unsigned INT_64BIT a = regs.d[(extra >> 12) & 7];
      unsigned INT_64BIT quot, rem;
      
      if (extra & 0x400)
	a |= (INT_64BIT)regs.d[extra & 7] << 32;
      rem = a % src;
      quot = a / src;
      if ((extra & 0x400) && (extra & 7) != ((extra >> 12) & 7))
	regs.d[extra & 7] = rem;
      regs.d[(extra >> 12) & 7] = quot;
    }
#endif
}

extern void m68k_mull (UWORD opcode, ULONG src, UWORD extra)
{
#ifdef INT_64BIT
    if (extra & 0x800) {
	/* signed variant */
	INT_64BIT a = (LONG)regs.d[(extra >> 12) & 7];

	a *= (LONG)src;
	if ((extra & 0x400) && (extra & 7) != ((extra >> 12) & 7))
	    regs.d[extra & 7] = a >> 32;
	regs.d[(extra >> 12) & 7] = (ULONG)a;
    } else {
	/* unsigned */
	unsigned INT_64BIT a = (ULONG)regs.d[(extra >> 12) & 7];
	unsigned INT_64BIT quot, rem;
	
	a *= src;
	if ((extra & 0x400) && (extra & 7) != ((extra >> 12) & 7))
	    regs.d[extra & 7] = a >> 32;
	regs.d[(extra >> 12) & 7] = (ULONG)a;
    }
#endif
}

static char* ccnames[] =
{ "T ","F ","HI","LS","CC","CS","NE","EQ",
  "VC","VS","PL","MI","GE","LT","GT","LE" };

void MC68000_reset(void)
{
  regs.a[7] = get_long(0x000000);
  m68k_setpc(tihw.initial_pc);
  regs.s = 1;
  regs.stopped = 0;
  regs.t = 0;
  specialflags = 0;
  regs.intmask = 7;
  regs.vbr = regs.sfc = regs.dfc = 0;
}

void op_illg(ULONG opcode)
{
  regs.pc_p-=2;
  
#ifdef DEBUGGER_TIGER // this part scan breakpoint and launch debugger
  if(opcode == 0xdfff) 
    {
      int i;
      for(i=0; i<breakPoints; i++) 
	{
	  if(get_real_address(breakAdr[i]) == regs.pc_p) 
	    {
	      cb_launch_debugger();
	      regs.pc_p[0] = breakSave[i]>>8;
	      regs.pc_p[1] = breakSave[i]&0xff;
	      opcode = nextiword();
	      (*cpufunctbl[opcode])(opcode);
	      put_word(breakAdr[i], 0xdfff);
	      i = breakPoints;
	    }
	}
      return;
    }
#endif
  
  if ((opcode & 0xF000) == 0xF000) 
    {
      if ((opcode & 0xE00) == 0x200)
	Exception(0xB);
      else 
	switch (opcode & 0x1FF) 
	  {
	  case 0x17:
	    regs.pc_p+=4;
	    break;
	  default:
	    regs.pc_p+=2;
	  }
      return;
    }
  if ((opcode & 0xF000) == 0xA000) 
    {
      Exception(0xA);
      return;
    }
  
  fprintf(stderr, "UAE: illegal instruction: %04x at $%06x (intel = 0x%08x) %08x\n", 
	  opcode, regs.pc, get_real_address(regs.pc), m68k_getpc());
  Exception(4);
}


void MC68000_run(void)
{
#ifdef PENT_COUNTER
  unsigned long l, k;
#endif  

  for(;;) 
    {
      UWORD opcode;    
      opcode = nextiword();
      
#ifdef PENT_COUNTER          
      readpec(l);
      (*cpufunctbl[opcode])(opcode);
	  do_cycles();
      
      if(l - lastTime >= timeDiff) 
	{
	  lastTime = l;
	  hw_update();
	  if(params.tick_rate != tickRate) 
	    {
	      tickRate = params.tick_rate;
	      timeDiff = ((unsigned long)permSec * (unsigned long)tickRate)/100000; // long long
	    }
	  if(params.cycle_rate != cycleRate) 
	    {
	      cycleRate = params.cycle_rate;
	      cycleDiff = (permSec*cycleRate)/100000;	
	    }
	}
      else
	do 
	  { 
	    readpec(k);
	  } while( (k - l) < cycleDiff);
#else /* PENT_COUNTER */
      (*cpufunctbl[opcode])(opcode);
      do_cycles();
#endif /* PENT_COUNTER */
      
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
	      return;		
	    }

	  if(specialflags & SPCFLAG_DBTRACE) 
	    {
	      specialflags &= ~SPCFLAG_DBTRACE;
	      //cb_launch_debugger();
	    }
	}   
    }
}

#ifdef DEBUGGER

void MC68000_step(void)
{
    specialflags |= SPCFLAG_BRK;
    MC68000_run();
}

void MC68000_skip(CPTR nextpc)
{
  broken_in = 0;
  specialflags |= SPCFLAG_BRK;
  do 
    {
      MC68000_step();
    } while (nextpc != m68k_getpc() && !broken_in);
}


void MC68000_disasm(CPTR addr, CPTR *nextpc, int cnt, char *output)
{
  char temp[40];
  CPTR pc;

  output[0]=0;
  pc = m68k_getpc();
  m68k_setpc(addr);
  for (;cnt--;)
    {
      char instrname[20],*ccpt;
      UWORD opcode;
      struct mnemolookup *lookup;
      struct instr *dp;
      
      sprintf(temp, "%08lx: ", m68k_getpc());
      strcat(output, temp);
	
      opcode = nextiword();
      if (cpufunctbl[opcode] == op_illg) 
	{
	  opcode = 0x4AFC;
	}
      dp = table68k + opcode;
      for (lookup = lookuptab;lookup->mnemo != dp->mnemo; lookup++);
      
      strcpy(instrname,lookup->name);
      ccpt = strstr(instrname,"cc");
      if (ccpt != 0) 
	{
	  strncpy(ccpt,ccnames[dp->cc],2);
	}

      // sprintf(temp,"%s", instrname);
      strcat(output,instrname);
      switch(dp->size)
	{
	case sz_byte: strcat(output, ".B "); break;
	case sz_word: strcat(output, ".W "); break;
	case sz_long: strcat(output, ".L "); break;
	default: break;
	}      
      if (dp->suse) 
	{
	  ShowEA(dp->sreg, dp->smode, dp->size, output);
	}
      if (dp->suse && dp->duse)
	{
	  strcat(output, ",");
	}
      if (dp->duse) 
	{
	  ShowEA(dp->dreg, dp->dmode, dp->size,output);
	}
      if (ccpt != 0) 
	{
	  if (cctrue(dp->cc)) 
	    {
	      sprintf(temp," (TRUE)");
	      strcat(output,temp);
	    }
	  else 
	    {
	      sprintf(temp," (FALSE)");
	      strcat(output,temp);
	    }
	}
      strcat(output, "\n");
    }
  *nextpc = m68k_getpc();
  m68k_setpc(pc);
}

void MC68000_dumpstate(CPTR *nextpc)
{
    int i;
    char buffer[256];

    for(i = 0; i < 8; i++){
	printf("D%d: %08lx ", i, regs.d[i]);
	if ((i & 3) == 3) printf("\n");
    }
    for(i=0;i<8;i++){
	printf("A%d: %08lx ", i, regs.a[i]);
	if ((i & 3) == 3) printf("\n");
    }
    printf ("T=%d S=%d X=%d N=%d Z=%d V=%d C=%d IMASK=%d\n", 
	    regs.t, regs.s, regs.x, NFLG, ZFLG, VFLG, CFLG, regs.intmask);
    MC68000_disasm(m68k_getpc(), nextpc, 1, buffer);
    DISPLAY("%s\n", buffer);
    DISPLAY("next PC: %08lx\n", *nextpc);
}

#endif /* DEBUGGER */
