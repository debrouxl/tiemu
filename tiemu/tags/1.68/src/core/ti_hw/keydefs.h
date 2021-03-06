/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001, Romain Lievin
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

#ifndef __TI68K_KEYDEF__
#define __TI68K_KEYDEF__

#define NB_MAX_KEYS 120

enum {	
  TIKEY_DOWN, TIKEY_RIGHT, TIKEY_UP, TIKEY_LEFT, TIKEY_HAND, TIKEY_SHIFT, 
  TIKEY_DIAMOND, TIKEY_2ND,
  TIKEY_3, TIKEY_2, TIKEY_1, TIKEY_F8, TIKEY_W, TIKEY_S, TIKEY_Z,
  TIKEY_6, TIKEY_5, TIKEY_4, TIKEY_F3, TIKEY_E, TIKEY_D, TIKEY_X,
  TIKEY_9, TIKEY_8, TIKEY_7, TIKEY_F7, TIKEY_R, TIKEY_F, TIKEY_C, TIKEY_STORE,
  TIKEY_COMMA, TIKEY_PARIGHT, TIKEY_PALEFT, TIKEY_F2, TIKEY_T, TIKEY_G, 
  TIKEY_V, TIKEY_SPACE,
  TIKEY_TAN, TIKEY_COS, TIKEY_SIN, TIKEY_F6, TIKEY_Y, TIKEY_H, TIKEY_B, 
  TIKEY_DIVIDE,
  TIKEY_P, TIKEY_ENTER2, TIKEY_LN, TIKEY_F1, TIKEY_U, TIKEY_J, TIKEY_N, 
  TIKEY_POWER,
  TIKEY_MULTIPLY, TIKEY_APPS, TIKEY_CLEAR, TIKEY_F5, TIKEY_I, TIKEY_K, 
  TIKEY_M, TIKEY_EQUALS,
  TIKEY_NU, TIKEY_ESCAPE, TIKEY_MODE, TIKEY_PLUS, TIKEY_O, TIKEY_L, 
  TIKEY_THETA, TIKEY_BACKSPACE,
  TIKEY_NEGATE, TIKEY_PERIOD, TIKEY_0, TIKEY_F4, TIKEY_Q, TIKEY_A, 
  TIKEY_ENTER1, TIKEY_MINUS, TIKEY_ON,
  TIKEY_ALPHA, TIKEY_EE, TIKEY_CATALOG, TIKEY_HOME, TIKEY_PIPE, 
  TIKEY_VOID, /*added by tom for ti89 support*/
  OPT_DEBUGGER, OPT_SCREENCAPTURE,
  MAX_KEY // important !
};

typedef struct {
   int x1;
   int x2;
   int y1;
   int y2;
   int code;
} KeyArea;

#endif
