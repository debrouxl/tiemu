 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Memory management
  *
  * (c) 1995 Bernd Schmidt
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include "config.h"
#include "options.h"
#include "uae.h"
#include "memory.h"
#include "ersatz.h"
#include "zfile.h"

#ifdef USE_MAPPED_MEMORY
#include <sys/mman.h>
#endif

/* Set by each memory handler that does not simply access real memory.  */
int special_mem;

int ersatzkickfile = 0;

uae_u32 allocated_chipmem;

#ifdef SAVE_MEMORY_BANKS
addrbank *mem_banks[65536];
#else
addrbank mem_banks[65536];
#endif

#ifdef NO_INLINE_MEMORY_ACCESS
__inline__ uae_u32 longget (uaecptr addr)
{
    return call_mem_get_func (get_mem_bank (addr).lget, addr);
}
__inline__ uae_u32 wordget (uaecptr addr)
{
    return call_mem_get_func (get_mem_bank (addr).wget, addr);
}
__inline__ uae_u32 byteget (uaecptr addr)
{
    return call_mem_get_func (get_mem_bank (addr).bget, addr);
}
__inline__ void longput (uaecptr addr, uae_u32 l)
{
    call_mem_put_func (get_mem_bank (addr).lput, addr, l);
}
__inline__ void wordput (uaecptr addr, uae_u32 w)
{
    call_mem_put_func (get_mem_bank (addr).wput, addr, w);
}
__inline__ void byteput (uaecptr addr, uae_u32 b)
{
    call_mem_put_func (get_mem_bank (addr).bput, addr, b);
}
#endif

uae_u32 chipmem_mask;

/* A dummy bank that only contains zeros */

static uae_u32 dummy_lget (uaecptr) REGPARAM;
static uae_u32 dummy_wget (uaecptr) REGPARAM;
static uae_u32 dummy_bget (uaecptr) REGPARAM;
static void dummy_lput (uaecptr, uae_u32) REGPARAM;
static void dummy_wput (uaecptr, uae_u32) REGPARAM;
static void dummy_bput (uaecptr, uae_u32) REGPARAM;
static int dummy_check (uaecptr addr, uae_u32 size) REGPARAM;

uae_u32 REGPARAM dummy_lget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem)
      write_log ("Illegal lget at %08lx\n", addr);

    return 0;
}

uae_u32 REGPARAM dummy_wget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem)
      write_log ("Illegal wget at %08lx\n", addr);

    return 0;
}

uae_u32 REGPARAM dummy_bget (uaecptr addr)
{
    special_mem |= S_READ;
    if (currprefs.illegal_mem)
      write_log ("Illegal bget at %08lx\n", addr);

    return 0;
}

void REGPARAM dummy_lput (uaecptr addr, uae_u32 l)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem)
      write_log ("Illegal lput at %08lx\n", addr);
}
void REGPARAM dummy_wput (uaecptr addr, uae_u32 w)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem)
      write_log ("Illegal wput at %08lx\n", addr);
}
void REGPARAM dummy_bput (uaecptr addr, uae_u32 b)
{
    special_mem |= S_WRITE;
    if (currprefs.illegal_mem)
      write_log ("Illegal bput at %08lx\n", addr);
}

int REGPARAM dummy_check (uaecptr addr, uae_u32 size)
{
    if (currprefs.illegal_mem)
      write_log ("Illegal check at %08lx\n", addr);

    return 0;
}

/* Chip memory */

uae_u8 *chipmemory;

static int chipmem_check (uaecptr addr, uae_u32 size) REGPARAM;
static uae_u8 *chipmem_xlate (uaecptr addr) REGPARAM;

uae_u32 REGPARAM chipmem_lget (uaecptr addr)
{
    uae_u32 *m;

    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    m = (uae_u32 *)(chipmemory + addr);
    return do_get_mem_long (m);
}

uae_u32 REGPARAM chipmem_wget (uaecptr addr)
{
    uae_u16 *m;

    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    m = (uae_u16 *)(chipmemory + addr);
    return do_get_mem_word (m);
}

uae_u32 REGPARAM chipmem_bget (uaecptr addr)
{
    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    return chipmemory[addr];
}

void REGPARAM chipmem_lput (uaecptr addr, uae_u32 l)
{
    uae_u32 *m;

    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    m = (uae_u32 *)(chipmemory + addr);
    do_put_mem_long (m, l);
}

void REGPARAM chipmem_wput (uaecptr addr, uae_u32 w)
{
    uae_u16 *m;

    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    m = (uae_u16 *)(chipmemory + addr);
    do_put_mem_word (m, w);
}

void REGPARAM chipmem_bput (uaecptr addr, uae_u32 b)
{
    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    chipmemory[addr] = b;
}

int REGPARAM chipmem_check (uaecptr addr, uae_u32 size)
{
    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    return (addr + size) <= allocated_chipmem;
}

uae_u8 REGPARAM *chipmem_xlate (uaecptr addr)
{
    addr -= chipmem_start & chipmem_mask;
    addr &= chipmem_mask;
    return chipmemory + addr;
}

/* Default memory access functions */

int REGPARAM default_check (uaecptr a, uae_u32 b)
{
    return 0;
}

uae_u8 REGPARAM *default_xlate (uaecptr a)
{
    write_log ("Your m68k program just did something terribly stupid\n");
    uae_reset();
    abort();
}

static int load_kickstart (void)
{
  /* NOTE No need for this code */
}

/* Address banks */

addrbank dummy_bank = {
    dummy_lget, dummy_wget, dummy_bget,
    dummy_lput, dummy_wput, dummy_bput,
    default_xlate, dummy_check
};

addrbank chipmem_bank = {
    chipmem_lget, chipmem_wget, chipmem_bget,
    chipmem_lput, chipmem_wput, chipmem_bput,
    chipmem_xlate, chipmem_check
};

char *address_space, *good_address_map;
int good_address_fd;

static void init_mem_banks (void)
{
    int i;
    for (i = 0; i < 65536; i++)
      put_mem_bank (i<<16, &dummy_bank);
}

#define MAKE_USER_PROGRAMS_BEHAVE 1
void memory_init (void)
{
    char buffer[4096];
    char *nam;
    int i, fd;
    int custom_start;

    allocated_chipmem = currprefs.chipmem_size;

    chipmemory = (uae_u8 *)malloc (allocated_chipmem);

    while (! chipmemory && allocated_chipmem > 512*1024) {
      allocated_chipmem >>= 1;
      chipmemory = (uae_u8 *)malloc (allocated_chipmem);
      if (chipmemory)
          fprintf (stderr, "Reducing chipmem size to %dkb\n", allocated_chipmem >> 10);
    }
    if (! chipmemory) {
      write_log ("virtual memory exhausted (chipmemory)!\n");
      abort ();
    }

    do_put_mem_long ((uae_u32 *)(chipmemory + 4), 0);
    init_mem_banks ();

    /* Map the chipmem into all of the lower 16MB */
    map_banks (&chipmem_bank, 0x8000, allocated_chipmem>>16);

    /* NOTE Do we care about the following? -WEC */
    chipmem_mask = allocated_chipmem- 1;
}

void map_banks (addrbank *bank, int start, int size)
{
    int bnr;
    unsigned long int hioffs = 0, endhioffs = 0x100;

    if (start >= 0x100) {
      for (bnr = start; bnr < start + size; bnr++)
          put_mem_bank (bnr << 16, bank);
      return;
    }
    /* Some '020 Kickstarts apparently require a 24 bit address space... */
    if (currprefs.address_space_24)
      endhioffs = 0x10000;
    for (hioffs = 0; hioffs < endhioffs; hioffs += 0x100)
      for (bnr = start; bnr < start+size; bnr++)
          put_mem_bank ((bnr + hioffs) << 16, bank);
}
