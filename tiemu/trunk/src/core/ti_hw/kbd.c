/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*
    Keyboard management
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "libuae.h"
#include "keydefs.h"
#include "ti68k_def.h"
#include "m68k.h"
#include "bits.h"
#include "ports.h"

static TiKey key_states[NB_MAX_KEYS];
static int  *key_row;
static int   key_change;

int keyRow92[10][8] =
{
  {TIKEY_DOWN, TIKEY_RIGHT, TIKEY_UP, TIKEY_LEFT, TIKEY_HAND, TIKEY_SHIFT, TIKEY_DIAMOND, TIKEY_2ND},
  {TIKEY_3, TIKEY_2, TIKEY_1, TIKEY_F8, TIKEY_W, TIKEY_S, TIKEY_Z, TIKEY_VOID},
  {TIKEY_6, TIKEY_5, TIKEY_4, TIKEY_F3, TIKEY_E, TIKEY_D, TIKEY_X, TIKEY_VOID},
  {TIKEY_9, TIKEY_8, TIKEY_7, TIKEY_F7, TIKEY_R, TIKEY_F, TIKEY_C, TIKEY_STORE},
  {TIKEY_COMMA, TIKEY_PARIGHT, TIKEY_PALEFT, TIKEY_F2, TIKEY_T, TIKEY_G, TIKEY_V, TIKEY_SPACE},
  {TIKEY_TAN, TIKEY_COS, TIKEY_SIN, TIKEY_F6, TIKEY_Y, TIKEY_H, TIKEY_B, TIKEY_DIVIDE},
  {TIKEY_P, TIKEY_ENTER2, TIKEY_LN, TIKEY_F1, TIKEY_U, TIKEY_J, TIKEY_N, TIKEY_POWER},
  {TIKEY_MULTIPLY, TIKEY_APPS, TIKEY_CLEAR, TIKEY_F5, TIKEY_I, TIKEY_K, TIKEY_M, TIKEY_EQUALS},
  {TIKEY_NU, TIKEY_ESCAPE, TIKEY_MODE, TIKEY_PLUS, TIKEY_O, TIKEY_L, TIKEY_THETA, TIKEY_BACKSPACE},
  {TIKEY_NEGATE, TIKEY_PERIOD, TIKEY_0, TIKEY_F4, TIKEY_Q, TIKEY_A, TIKEY_ENTER1, TIKEY_MINUS}
};

int keyRow89[10][8] =
{
  {TIKEY_ALPHA, TIKEY_DIAMOND, TIKEY_SHIFT, TIKEY_2ND, TIKEY_RIGHT, TIKEY_DOWN, TIKEY_LEFT, TIKEY_UP},
  {TIKEY_F5, TIKEY_CLEAR, TIKEY_POWER, TIKEY_DIVIDE, TIKEY_MULTIPLY, TIKEY_MINUS, TIKEY_PLUS, TIKEY_ENTER1},
  {TIKEY_F4, TIKEY_BACKSPACE, TIKEY_T, TIKEY_COMMA, TIKEY_9, TIKEY_6, TIKEY_3, TIKEY_NEGATE},
  {TIKEY_F3, TIKEY_CATALOG, TIKEY_Z, TIKEY_PARIGHT, TIKEY_8, TIKEY_5, TIKEY_2, TIKEY_PERIOD},
  {TIKEY_F2, TIKEY_MODE, TIKEY_Y, TIKEY_PALEFT, TIKEY_7, TIKEY_4, TIKEY_1, TIKEY_0},
  {TIKEY_F1, TIKEY_HOME, TIKEY_X, TIKEY_EQUALS, TIKEY_PIPE, TIKEY_EE, TIKEY_STORE, TIKEY_APPS},
  {TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_ESCAPE},
  {TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID},
  {TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID},
  {TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID}
};

int keyRowV200[10][8] =
{
  {TIKEY_DOWN, TIKEY_RIGHT, TIKEY_UP, TIKEY_LEFT, TIKEY_HAND, TIKEY_SHIFT, TIKEY_DIAMOND, TIKEY_2ND},
  {TIKEY_3, TIKEY_2, TIKEY_1, TIKEY_F8, TIKEY_W, TIKEY_S, TIKEY_Z, TIKEY_NU},
  {TIKEY_6, TIKEY_5, TIKEY_4, TIKEY_F3, TIKEY_E, TIKEY_D, TIKEY_X, TIKEY_NU},
  {TIKEY_9, TIKEY_8, TIKEY_7, TIKEY_F7, TIKEY_R, TIKEY_F, TIKEY_C, TIKEY_STORE},
  {TIKEY_COMMA, TIKEY_PARIGHT, TIKEY_PALEFT, TIKEY_F2, TIKEY_T, TIKEY_G, TIKEY_V, TIKEY_SPACE},
  {TIKEY_TAN, TIKEY_COS, TIKEY_SIN, TIKEY_F6, TIKEY_Y, TIKEY_H, TIKEY_B, TIKEY_DIVIDE},  
  {TIKEY_P, TIKEY_ENTER2, TIKEY_LN, TIKEY_F1, TIKEY_U, TIKEY_J, TIKEY_N, TIKEY_POWER},
  {TIKEY_MULTIPLY, TIKEY_APPS, TIKEY_CLEAR, TIKEY_F5, TIKEY_I, TIKEY_K, TIKEY_M, TIKEY_EQUALS},
  {TIKEY_NU, TIKEY_ESCAPE, TIKEY_MODE, TIKEY_PLUS, TIKEY_O, TIKEY_L, TIKEY_THETA, TIKEY_BACKSPACE},
  {TIKEY_NEGATE, TIKEY_PERIOD, TIKEY_0, TIKEY_F4, TIKEY_Q, TIKEY_A, TIKEY_ENTER1, TIKEY_MINUS}
};

int hw_kbd_init(void)
{
	int i;

	key_change = 0;
    tihw.on_key = 0;

    switch(tihw.calc_type)
    {
    case TI89:
    case TI89t:
        key_row = (int*)keyRow89;
        break;
    case TI92:
    case TI92p:
        key_row = (int*)keyRow92;
        break;
    case V200:
        key_row = (int*)keyRowV200;
    default:
        break;
    }

	for(i = 0; i < MAX_TIKEYS; i++)
		key_states[i] = 0;

    return 0;
}

int hw_kbd_reset(void)
{
    return 0;
}

int hw_kbd_exit(void)
{
    key_row = NULL;

    return 0;
}

void ti68k_kbd_set_key(int key, int active)
{
	if(key == TIKEY_ALPHA)
	{
		if(active)
		{
			key_states[key]++;
			key_change = !0;
		}
		else
			key_states[key]--;
	}
	else if(key == TIKEY_ON)
		tihw.on_key = active;
	else
	{
		key_states[key] = active;
		if(active)
			key_change = !0;	
	}
}

int ti68k_kbd_is_key_pressed(int key)
{
    return key_states[key];
}

int hw_kbd_update(void)		// ~600Hz
{
	if(key_change)
    {
    	// Auto-Int 2 is triggered when the first unmasked key is pressed. Keeping the key
		// pressed, or pressing another one without releasing the first key, will not generate
		// additional interrupts.
		if((tihw.hw_type == HW1) || !(io2_bit_tst(0x1f, 2) && !io2_bit_tst(0x1f, 1)))
			hw_m68k_irq(2);
		key_change = 0;
    }
	
	if(tihw.on_key) 
    {
    	// Auto-Int 6 triggered when [ON] is pressed.
		hw_m68k_irq(6);
    }
  
    return 0;
}

static uint8_t get_rowmask(uint8_t r) 
{
    uint8_t rc = 0;
    int i;
    int *row = key_row + (r << 3);
  
    for(i=0; i<8; i++)
    {
        rc |= (key_states[row[i]] & 1) << (7-i);
    }

    return rc;
}

uint8_t hw_kbd_read_cols(void)
{
    static uint8_t i;
    static uint8_t arg;
    static uint16_t mask;

    mask = (((uint16_t)tihw.io[0x18]) << 8) | tihw.io[0x19];
    for(i = 0, arg = 0; i < 10; i++)
    {
        if(!(mask & (1<<i)))
            arg |= get_rowmask(i);
    }

    return ~arg;
}
