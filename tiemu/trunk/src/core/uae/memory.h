/* UAE - The Un*x Amiga Emulator
 *
 * MC68000 emulation
 *
 * Copyright 1995 Bernd Schmidt
 */

extern UBYTE get_byte(CPTR adr);
extern UWORD get_word(CPTR adr);
extern ULONG get_long(CPTR adr);
extern void put_long(CPTR adr, ULONG arg);
extern void put_word(CPTR adr, UWORD arg);
extern void put_byte(CPTR adr, UBYTE arg);

extern UBYTE io_get_byte(CPTR adr);
extern UWORD io_get_word(CPTR adr);
extern ULONG io_get_long(CPTR adr);
extern void io_put_long(CPTR adr, ULONG arg);
extern void io_put_word(CPTR adr, UWORD arg);
extern void io_put_byte(CPTR adr, UBYTE arg);

extern UBYTE *get_real_address(CPTR adr);
extern int valid_address(CPTR adr, ULONG size);
