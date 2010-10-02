 /*
  * UAE - The Un*x Amiga Emulator
  *
  * MC68881 emulation
  *
  * Copyright 1996 Herman ten Brugge
  * stubbed out for TiEmu - Copyright (C) 2005 Kevin Kofler
  */

#include <stdlib.h>
#include <stdio.h>
#include "sysdeps.h"

void fdbcc_opp (uae_u32 opcode, uae_u16 extra)
{
    opcode = opcode, extra = extra;
    abort();
}

void fscc_opp (uae_u32 opcode, uae_u16 extra)
{
    opcode = opcode, extra = extra;
    abort();
}

void ftrapcc_opp (uae_u32 opcode, uaecptr oldpc)
{
    opcode = opcode, oldpc = oldpc;
    abort();
}

void fbcc_opp (uae_u32 opcode, uaecptr pc, uae_u32 extra)
{
    opcode = opcode, pc = pc, extra = extra;
    abort();
}

void fsave_opp (uae_u32 opcode)
{
    opcode = opcode;
    abort();
}

void frestore_opp (uae_u32 opcode)
{
    opcode = opcode;
    abort();
}

void fpp_opp (uae_u32 opcode, uae_u16 extra)
{
    opcode = opcode, extra = extra;
    abort();
}
