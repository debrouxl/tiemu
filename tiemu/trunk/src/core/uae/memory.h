/* UAE - The Un*x Amiga Emulator
 *
 * MC68000 emulation
 *
 * Copyright 1995 Bernd Schmidt
 */

/*
extern UBYTE get_byte(CPTR adr);
extern UWORD get_word(CPTR adr);
extern ULONG get_long(CPTR adr);
extern void put_long(CPTR adr, ULONG arg);
extern void put_word(CPTR adr, UWORD arg);
extern void put_byte(CPTR adr, UBYTE arg);

//extern UBYTE *get_real_address(CPTR adr);
extern int valid_address(CPTR adr, ULONG size);
*/

//wrappers
#define get_byte(addr)	hw_get_byte(addr)
#define get_word(addr)	hw_get_word(addr)
#define get_long(addr)	hw_get_long(addr)

#define put_long(adr, arg)	hw_put_long(adr, arg)
#define put_word(adr, arg)	hw_put_word(adr, arg)
#define put_byte(adr, arg)	hw_put_byte(adr, arg)

#define get_real_address(addr)	hw_get_real_address(addr)
