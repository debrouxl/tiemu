typedef enum {
  Dreg, Areg, Aind, Aipi, Apdi, Ad16, Ad8r, 
  absw, absl, PC16, PC8r, imm, imm0, imm1, imm2, immi, am_unknown, am_illg
} amodes;

typedef enum {
    i_ILLG,
    
    i_OR, i_AND, i_EOR, i_ORSR, i_ANDSR, i_EORSR, 
    i_SUB, i_SUBA, i_SUBX, i_SBCD, 
    i_ADD, i_ADDA, i_ADDX, i_ABCD,
    i_NEG, i_NEGX, i_NBCD, i_CLR, i_NOT, i_TST,
    i_BTST, i_BCHG, i_BCLR, i_BSET,
    i_CMP, i_CMPM, i_CMPA, 
    i_MVPRM, i_MVPMR, i_MOVE, i_MOVEA, i_MVSR2, i_MV2SR, 
    i_SWAP, i_EXG, i_EXT, i_MVMEL, i_MVMLE, 
    i_TRAP, i_MVR2USP, i_MVUSP2R, i_RESET, i_NOP, i_STOP, i_RTE, i_RTD, 
    i_LINK, i_UNLK, 
    i_RTS, i_TRAPV, i_RTR, 
    i_JSR, i_JMP, i_BSR, i_Bcc,
    i_LEA, i_PEA, i_DBcc, i_Scc,
    i_DIVU, i_DIVS, i_MULU, i_MULS, 
    i_ASR, i_ASL, i_LSR, i_LSL, i_ROL, i_ROR, i_ROXL, i_ROXR,
    i_ASRW, i_ASLW, i_LSRW, i_LSLW, i_ROLW, i_RORW, i_ROXLW, i_ROXRW,
    i_CHK,
    i_MOVEC2, i_MOVE2C, i_CAS, i_DIVL, i_MULL
} instrmnem;

extern struct mnemolookup {
    instrmnem mnemo;
    const char *name;
} lookuptab[];

typedef enum {
    sz_unknown, sz_byte, sz_word, sz_long
} wordsizes;

typedef enum {
    bit0, bit1, bitc, bitC, bitf, biti, bitI, bitj, bitJ, 
    bits, bitS, bitd, bitD, bitr, bitR, bitz, lastbit
} bitvals;

struct instr_def {
    UWORD bits;
    int n_variable;
    char bitpos[16];
    UWORD mask;
    int plevel;
    const char *opcstr;	
};

extern struct instr_def defs68k[];
extern int n_defs68k;

extern struct instr {
    instrmnem mnemo;
    wordsizes size;
    amodes dmode, smode;
    unsigned char dreg;
    unsigned char sreg;
    unsigned int suse:1;
    unsigned int duse:1;
    unsigned int stype:2;
    unsigned int cc:4;
    unsigned int plev:2;
    signed char dpos;
    signed char spos;
    long int handler;
} *table68k;

extern void read_table68k (void);
extern void do_merges (void);
extern int get_no_mismatches (void);
