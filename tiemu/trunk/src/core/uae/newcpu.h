 /* 
  * UAE - The Un*x Amiga Emulator
  * 
  * MC68000 emulation
  *
  * Copyright 1995 Bernd Schmidt
  */

#define REGPARAM

extern unsigned long specialflags;

#define SPCFLAG_DBTRACE 1
#define SPCFLAG_STOP 2
#define SPCFLAG_DBSKIP 4
#define SPCFLAG_INT  8
#define SPCFLAG_BRK  16
#define SPCFLAG_EXTRA_CYCLES 32
#define SPCFLAG_TRACE 64
#define SPCFLAG_DOTRACE 128
#define SPCFLAG_DOINT 256
#define SPCFLAG_TIMER 512
#define SPCFLAG_DELAY 1024
#define SPCFLAG_ADRERR 2048

//tiemu begin
extern int breakCause; 
extern int speedyLink;
extern int delayTime;
//tiemu end

extern int areg_byteinc[];
extern int imm8_table[];

extern int broken_in;

extern int uae_initial_pc;
extern int currIntLev;
extern void Interrupt(int);

typedef void cpuop_func(ULONG) REGPARAM;

struct cputbl {
    cpuop_func *handler;
    int specific;
    UWORD opcode;
};

extern struct cputbl smallcputbl[];

extern cpuop_func *cpufunctbl[65536];
extern void op_illg(ULONG) REGPARAM;

typedef char flagtype; 

union flagu {
    struct {
	/* v must be at the start so that the x86 seto instruction
	 * changes the V flag. C must follow after V. */
	char v;
	char c;
	char n;
	char z;
    } flags;
    ULONG longflags;
};

extern struct regstruct 
{
    ULONG d[8];
    CPTR  a[8],usp;
    UWORD sr;
    flagtype t;
    flagtype s;
    flagtype x;
    flagtype stopped;
    int intmask;
    ULONG pc;
    UBYTE *pc_p;
    UBYTE *pc_oldp;
    
    ULONG vbr,sfc,dfc;
} regs;

#ifdef INTEL_FLAG_OPT
extern union flagu intel_flag_lookup[256] __asm__ ("intel_flag_lookup");
extern union flagu regflags __asm__ ("regflags");
#else
extern union flagu regflags;
#endif

#define ZFLG (regflags.flags.z)
#define NFLG (regflags.flags.n)
#define CFLG (regflags.flags.c)
#define VFLG (regflags.flags.v)

#ifdef PENT_COUNTER
extern float calibrate_pcounter(void);
#endif

extern void MC68000_oldstep(UWORD opcode);

static INLINE_DECLARATION UWORD nextiword(void)
{
  UWORD r = (((UWORD)regs.pc_p[0])<<8) | regs.pc_p[1];
  regs.pc_p += 2;
  return r;
}

static INLINE_DECLARATION ULONG nextilong(void)
{
    return (nextiword()<<16) | nextiword();
}

static INLINE_DECLARATION void m68k_setpc(CPTR newpc)
{
    regs.pc = newpc&0xffffff;
    regs.pc_p = regs.pc_oldp = get_real_address(regs.pc);
}

static INLINE_DECLARATION CPTR m68k_getpc(void)
{
    return regs.pc + regs.pc_p - regs.pc_oldp;
}

static INLINE_DECLARATION void m68k_setstopped(int stop)
{
    regs.stopped = stop;
    if (stop)
	specialflags |= SPCFLAG_STOP;
}

static INLINE_DECLARATION int cctrue(const int cc)
{
    switch(cc){
     case 0: return 1;                       /* T */
     case 1: return 0;                       /* F */
     case 2: return !CFLG && !ZFLG;          /* HI */
     case 3: return CFLG || ZFLG;            /* LS */
     case 4: return !CFLG;                   /* CC */
     case 5: return CFLG;                    /* CS */
     case 6: return !ZFLG;                   /* NE */
     case 7: return ZFLG;                    /* EQ */
     case 8: return !VFLG;                   /* VC */
     case 9: return VFLG;                    /* VS */
     case 10:return !NFLG;                   /* PL */
     case 11:return NFLG;                    /* MI */
     case 12:return NFLG == VFLG;            /* GE */
     case 13:return NFLG != VFLG;            /* LT */
     case 14:return !ZFLG && (NFLG == VFLG); /* GT */
     case 15:return ZFLG || (NFLG != VFLG);  /* LE */
    }
    //abort();
    return 0;
}

static INLINE_DECLARATION ULONG get_disp_ea (ULONG base, UWORD dp)
{
    int reg = (dp >> 12) & 7;
    LONG regd;
    
    if (dp & 0x8000)
	regd = regs.a[reg];
    else
	regd = regs.d[reg];
    if (!(dp & 0x800))
	regd = (LONG)(WORD)regd;
    return base + (BYTE)(dp) + regd;
}


extern void MakeSR(void);
extern void MakeFromSR(void);
extern void Exception(int);
extern void m68k_move2c(int, ULONG *);
extern void m68k_movec2(int, ULONG *);
extern void m68k_divl (UWORD, ULONG, UWORD);
extern void m68k_mull (UWORD, ULONG, UWORD);
extern void init_m68k (void);
extern void MC68000_step(void);
extern void MC68000_run(void);
extern void MC68000_skip(CPTR);
extern void MC68000_dumpstate(CPTR *);
extern void MC68000_disasm(CPTR, CPTR *, int, char*);
extern void MC68000_reset(void);
extern int intlev(void);
