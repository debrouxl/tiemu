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

#ifndef __TI68K_FLASH__
#define __TI68K_FLASH__

#include "stdint.h"

/* Types */

typedef struct
{
	int	cmd;
    int ret_or;
    int write;
    int erase;

    int write_ready;
    int write_phase;
    int erase_phase;
} FLASH_WSM;

extern FLASH_WSM   wsm;

/* Functions */

uint8_t  FlashReadByte(uint32_t addr);
uint16_t FlashReadWord(uint32_t addr);
uint32_t FlashReadLong(uint32_t addr);

void FlashWriteByte(uint32_t addr, uint8_t  v);
void FlashWriteWord(uint32_t addr, uint16_t v);
void FlashWriteLong(uint32_t addr, uint32_t v);

uint32_t find_pc(void);

/* Command definitions */
#define READ_ARRAY_RESET		0xff
#define READ_ID_CODES			0x90
#define QUERY					0x98
#define READ_STATUS_REGISTER	0x70
#define CLEAR_STATUS_REGISTER	0x50
#define BLOCK_ERASE_SETUP		0x20
#define FULL_CHIP_ERASE			0x30
#define BYTE_WRITE_SETUP		0x40
#define CONFIRM_BLOCK_ERASE		0xd0

#endif
