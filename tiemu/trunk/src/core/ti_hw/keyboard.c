/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
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

/*
  TI keyboard management
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "uae.h"
#include "keydefs.h"
#include "memory.h"
#include "callbacks.h"
#include "images.h"
#include "ti68k_def.h"

int on_key = 0;
static int key_states[NB_MAX_KEYS];

int keyRow92[10][8] =
{
  {TIKEY_DOWN, TIKEY_RIGHT, TIKEY_UP, TIKEY_LEFT, TIKEY_HAND, 
   TIKEY_SHIFT, TIKEY_DIAMOND, TIKEY_2ND},
  {TIKEY_3, TIKEY_2, TIKEY_1, TIKEY_F8, TIKEY_W, TIKEY_S, TIKEY_Z, TIKEY_NU},
  {TIKEY_6, TIKEY_5, TIKEY_4, TIKEY_F3, TIKEY_E, TIKEY_D, TIKEY_X, TIKEY_NU},
  {TIKEY_9, TIKEY_8, TIKEY_7, TIKEY_F7, TIKEY_R, TIKEY_F, TIKEY_C, 
   TIKEY_STORE},
  {TIKEY_COMMA, TIKEY_PARIGHT, TIKEY_PALEFT, TIKEY_F2, TIKEY_T, TIKEY_G, 
   TIKEY_V, TIKEY_SPACE},
  {TIKEY_TAN, TIKEY_COS, TIKEY_SIN, TIKEY_F6, TIKEY_Y, TIKEY_H, TIKEY_B, 
   TIKEY_DIVIDE},
  {TIKEY_P, TIKEY_ENTER2, TIKEY_LN, TIKEY_F1, TIKEY_U, TIKEY_J, TIKEY_N, 
   TIKEY_POWER},
  {TIKEY_MULTIPLY, TIKEY_APPS, TIKEY_CLEAR, TIKEY_F5, TIKEY_I, TIKEY_K, 
   TIKEY_M, TIKEY_EQUALS},
  {TIKEY_NU, TIKEY_ESCAPE, TIKEY_MODE, TIKEY_PLUS, TIKEY_O, TIKEY_L, 
   TIKEY_THETA, TIKEY_BACKSPACE},
  {TIKEY_NEGATE, TIKEY_PERIOD, TIKEY_0, TIKEY_F4, TIKEY_Q, TIKEY_A, 
   TIKEY_ENTER1, TIKEY_MINUS}
};

int keyRow89[10][8] =
{
  {TIKEY_ALPHA, TIKEY_DIAMOND, TIKEY_SHIFT, TIKEY_2ND, TIKEY_RIGHT, 
   TIKEY_DOWN, TIKEY_LEFT, TIKEY_UP},
  {TIKEY_F5, TIKEY_CLEAR, TIKEY_POWER, TIKEY_DIVIDE, TIKEY_MULTIPLY, 
   TIKEY_MINUS, TIKEY_PLUS, TIKEY_ENTER1},
  {TIKEY_F4, TIKEY_BACKSPACE, TIKEY_T, TIKEY_COMMA, TIKEY_9, TIKEY_6, 
   TIKEY_3, TIKEY_NEGATE},
  {TIKEY_F3, TIKEY_CATALOG, TIKEY_Z, TIKEY_PARIGHT, TIKEY_8, TIKEY_5, 
   TIKEY_2, TIKEY_PERIOD},
  {TIKEY_F2, TIKEY_MODE, TIKEY_Y, TIKEY_PALEFT, TIKEY_7, TIKEY_4, TIKEY_1, 
   TIKEY_0},
  {TIKEY_F1, TIKEY_HOME, TIKEY_X, TIKEY_EQUALS, TIKEY_PIPE, TIKEY_EE, 
   TIKEY_STORE, TIKEY_APPS},
  {TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, 
   TIKEY_VOID, TIKEY_ESCAPE},
  {TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, 
   TIKEY_VOID, TIKEY_VOID},
  {TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, 
   TIKEY_VOID, TIKEY_VOID},
  {TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, TIKEY_VOID, 
   TIKEY_VOID, TIKEY_VOID}
};

int *key_row;

/* Press a key */
void ti68k_kbd_set_key(int key, int active)
{
  key_states[key] = active;
}

/* Returns true if the corresponding key was pressed */
int ti68k_isKeyPressed(int key)
{
  return key_states[key];
}

int hw_update_kbd(void) 
{
  int rc = cb_update_keys();

  if( (on_key = ti68k_isKeyPressed(TIKEY_ON)) ) 
    {
      if(specialflags & SPCFLAG_STOP)
	specialflags &= ~SPCFLAG_STOP;
      if(specialflags < 6) // no ints: ON works but FARGO do not
	//if(currIntLev < 6)
	{
	  specialflags |= SPCFLAG_INT;
	  currIntLev = 6;
	}
      key_states[TIKEY_ON] = 0;
    }
  else if(rc)
    {
      if(currIntLev < 2)
	{
	  specialflags |= SPCFLAG_INT;
	  currIntLev = 2;
	}
    }
  
  return rc;
}


UBYTE get_rowmask(UBYTE r) 
{
  UBYTE rc = 0;
  int i;
  int *row = key_row+(r<<3);
  
  for(i=0; i<8; i++)
    {
      rc |= key_states[row[i]]<<(7-i);
    }
  
  return rc;
}

UBYTE read_keyboard_mask(void)
{
  static int i;
  static UBYTE arg;
  static UWORD mask;

  arg=0;
  mask = (((UWORD)tihw.io[0x18])<<8)|tihw.io[0x19];
  for(i=0; i<10; i++)
    {
      if(!(mask & (1<<i)))
        arg |= get_rowmask(i);
    }

  return (UBYTE)(~arg);
}

int hw_kbd_init(void)
{
  if(img_infos.calc_type & TI92)
    key_row = (int*)keyRow92;
  else
    key_row = (int*)keyRow89;
}

int hw_kbd_reset(void)
{
}

int hw_kbd_exit(void)
{
  key_row = NULL;
}
