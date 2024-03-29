/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

STATIC_INLINE uae_u32 get_word_prefetch (int o)
{
    uae_u32 v = regs.irc;
    regs.irc = get_word (m68k_getpc() + o);
    return v;
}
STATIC_INLINE uae_u32 get_long_prefetch (int o)
{
    uae_u32 v = get_word_prefetch (o) << 16;
    v |= get_word_prefetch (o + 2);
    return v;
}

#ifdef CPUEMU_6
STATIC_INLINE uae_u32 mem_access_delay_word_read_cycles (uaecptr addr, int *cycles)
{
    if (addr < 0x200000 || (addr >= 0xc00000 && addr < 0xe00000)) {
	return wait_cpu_cycle_read_cycles (addr, 1, cycles);
    } else if (!(addr >= 0xa00000 && addr < 0xc00000)) {
	do_cycles_ce (4 * CYCLE_UNIT / 2);
	*cycles = 4;
    }
    return get_word (addr);
}
STATIC_INLINE uae_u32 mem_access_delay_word_read (uaecptr addr)
{
    if (addr < 0x200000 || (addr >= 0xc00000 && addr < 0xe00000)) {
	return wait_cpu_cycle_read (addr, 1);
    } else if (!(addr >= 0xa00000 && addr < 0xc00000)) {
	do_cycles_ce (4 * CYCLE_UNIT / 2);
    }
    return get_word (addr);
}
STATIC_INLINE uae_u32 mem_access_delay_byte_read (uaecptr addr)
{
    if (addr < 0x200000 || (addr >= 0xc00000 && addr < 0xe00000)) {
	return wait_cpu_cycle_read (addr, 0);
    } else if (!(addr >= 0xa00000 && addr < 0xc00000)) {
	do_cycles_ce (4 * CYCLE_UNIT / 2);
    }
    return get_byte (addr);
}
STATIC_INLINE void mem_access_delay_byte_write (uaecptr addr, uae_u32 v)
{
    if (addr < 0x200000 || (addr >= 0xc00000 && addr < 0xe00000)) {
	wait_cpu_cycle_write (addr, 0, v);
	return;
    } else if (!(addr >= 0xa00000 && addr < 0xc00000)) {
	do_cycles_ce (4 * CYCLE_UNIT / 2);
    }
    put_byte (addr, v);
}
STATIC_INLINE void mem_access_delay_word_write (uaecptr addr, uae_u32 v)
{
    if (addr < 0x200000 || (addr >= 0xc00000 && addr < 0xe00000)) {
	wait_cpu_cycle_write (addr, 1, v);
	return;
    } else if (!(addr >= 0xa00000 && addr < 0xc00000)) {
	do_cycles_ce (4 * CYCLE_UNIT / 2);
    }
    put_word (addr, v);
}

STATIC_INLINE uae_u32 get_word_ce (uaecptr addr)
{
    return mem_access_delay_word_read (addr);
}

STATIC_INLINE uae_u32 get_byte_ce (uaecptr addr)
{
    return mem_access_delay_byte_read (addr);
}

STATIC_INLINE uae_u32 get_word_ce_prefetch (int o)
{
    uae_u32 v = regs.irc;
    regs.irc = get_word_ce (m68k_getpc() + o);
    return v;
}

STATIC_INLINE int get_word_ce_prefetch_cycles (int o)
{
    int cycles = 0;
    regs.irc = mem_access_delay_word_read_cycles (m68k_getpc() + o, &cycles);
    return cycles;
}

STATIC_INLINE void put_word_ce (uaecptr addr, uae_u16 v)
{
    mem_access_delay_word_write (addr, v);
}

STATIC_INLINE void put_byte_ce (uaecptr addr, uae_u8 v)
{
    mem_access_delay_byte_write (addr, v);
}

STATIC_INLINE void m68k_do_rts_ce (void)
{
    uaecptr pc;
    pc = get_word_ce (m68k_areg (regs, 7)) << 16;
    pc |= get_word_ce (m68k_areg (regs, 7) + 2);
    m68k_areg (regs, 7) += 4;
    if (pc & 1)
	exception3 (0x4e75, m68k_getpc(), pc);
    else
	m68k_setpc (pc);
}

STATIC_INLINE void m68k_do_bsr_ce (uaecptr oldpc, uae_s32 offset)
{
    m68k_areg (regs, 7) -= 4;
    put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
    put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
    m68k_incpc (offset);
}

STATIC_INLINE void m68k_do_jsr_ce (uaecptr oldpc, uaecptr dest)
{
    m68k_areg (regs, 7) -= 4;
    put_word_ce (m68k_areg (regs, 7), oldpc >> 16);
    put_word_ce (m68k_areg (regs, 7) + 2, oldpc);
    m68k_setpc (dest);
}
#endif
