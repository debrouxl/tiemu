/* Hey EMACS -*- linux-c -*- */
/* $Id: hid.h 557 2004-07-14 08:17:39Z roms $ */

/*  TiEmu - a TI emulator
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Li�vin
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

#ifndef __PCKEYS_H__
#define __PCKEYS_H__

// List taken from <http://math.msu.su/~vfnik/WinApi/other/virtualkeycodes.html>

#define PCKEY_LBUTTON 0x1
#define PCKEY_RBUTTON 0x2
#define PCKEY_CANCEL 0x3
#define PCKEY_MBUTTON 0x4
#define PCKEY_BACK 0x8
#define PCKEY_TAB 0x9
#define PCKEY_CLEAR 0xC
#define PCKEY_RETURN 0xD
#define PCKEY_SHIFT 0x10
#define PCKEY_CONTROL 0x11
#define PCKEY_MENU 0x12
#define PCKEY_PAUSE 0x13
#define PCKEY_CAPITAL 0x14
#define PCKEY_ESCAPE 0x1B
#define PCKEY_SPACE 0x20
#define PCKEY_PRIOR 0x21
#define PCKEY_NEXT 0x22
#define PCKEY_END 0x23
#define PCKEY_HOME 0x24
#define PCKEY_LEFT 0x25
#define PCKEY_UP 0x26
#define PCKEY_RIGHT 0x27
#define PCKEY_DOWN 0x28
#define PCKEY_SELECT 0x29
#define PCKEY_PRINT 0x2A
#define PCKEY_EXECUTE 0x2B
#define PCKEY_SNAPSHOT 0x2C
#define PCKEY_INSERT 0x2D
#define PCKEY_DELETE 0x2E
#define PCKEY_HELP 0x2F
#define PCKEY_0 0x30
#define PCKEY_1 0x31
#define PCKEY_2 0x32
#define PCKEY_3 0x33
#define PCKEY_4 0x34
#define PCKEY_5 0x35
#define PCKEY_6 0x36
#define PCKEY_7 0x37
#define PCKEY_8 0x38
#define PCKEY_9 0x39
#define PCKEY_A 0x41
#define PCKEY_B 0x42
#define PCKEY_C 0x43
#define PCKEY_D 0x44
#define PCKEY_E 0x45
#define PCKEY_F 0x46
#define PCKEY_G 0x47
#define PCKEY_H 0x48
#define PCKEY_I 0x49
#define PCKEY_J 0x4A
#define PCKEY_K 0x4B
#define PCKEY_L 0x4C
#define PCKEY_M 0x4D
#define PCKEY_N 0x4E
#define PCKEY_O 0x4F
#define PCKEY_P 0x50
#define PCKEY_Q 0x51
#define PCKEY_R 0x52
#define PCKEY_S 0x53
#define PCKEY_T 0x54
#define PCKEY_U 0x55
#define PCKEY_V 0x56
#define PCKEY_W 0x57
#define PCKEY_X 0x58
#define PCKEY_Y 0x59
#define PCKEY_Z 0x5A
#define PCKEY_STARTKEY 0x5B
#define PCKEY_CONTEXTKEY 0x5D
#define PCKEY_NUMPAD0 0x60
#define PCKEY_NUMPAD1 0x61
#define PCKEY_NUMPAD2 0x62
#define PCKEY_NUMPAD3 0x63
#define PCKEY_NUMPAD4 0x64
#define PCKEY_NUMPAD5 0x65
#define PCKEY_NUMPAD6 0x66
#define PCKEY_NUMPAD7 0x67
#define PCKEY_NUMPAD8 0x68
#define PCKEY_NUMPAD9 0x69
#define PCKEY_MULTIPLY 0x6A
#define PCKEY_ADD 0x6B
#define PCKEY_SEPARATOR 0x6C
#define PCKEY_SUBTRACT 0x6D
#define PCKEY_DECIMAL 0x6E
#define PCKEY_DIVIDE 0x6F
#define PCKEY_F1 0x70
#define PCKEY_F2 0x71
#define PCKEY_F3 0x72
#define PCKEY_F4 0x73
#define PCKEY_F5 0x74
#define PCKEY_F6 0x75
#define PCKEY_F7 0x76
#define PCKEY_F8 0x77
#define PCKEY_F9 0x78
#define PCKEY_F10 0x79
#define PCKEY_F11 0x7A
#define PCKEY_F12 0x7B
#define PCKEY_F13 0x7C
#define PCKEY_F14 0x7D
#define PCKEY_F15 0x7E
#define PCKEY_F16 0x7F
#define PCKEY_F17 0x80
#define PCKEY_F18 0x81
#define PCKEY_F19 0x82
#define PCKEY_F20 0x83
#define PCKEY_F21 0x84
#define PCKEY_F22 0x85
#define PCKEY_F23 0x86
#define PCKEY_F24 0x87
#define PCKEY_NUMLOCK 0x90
#define PCKEY_OEM_SCROLL 0x91
#define PCKEY_OEM_1 0xBA
#define PCKEY_OEM_PLUS 0xBB
#define PCKEY_OEM_COMMA 0xBC
#define PCKEY_OEM_MINUS 0xBD
#define PCKEY_OEM_PERIOD 0xBE
#define PCKEY_OEM_2 0xBF
#define PCKEY_OEM_3 0xC0
#define PCKEY_OEM_4 0xDB
#define PCKEY_OEM_5 0xDC
#define PCKEY_OEM_6 0xDD
#define PCKEY_OEM_7 0xDE
#define PCKEY_OEM_8 0xDF
#define PCKEY_ICO_F17 0xE0
#define PCKEY_ICO_F18 0xE1
#define PCKEY_OEM102 0xE2
#define PCKEY_ICO_HELP 0xE3
#define PCKEY_ICO_00 0xE4
#define PCKEY_ICO_CLEAR 0xE6
#define PCKEY_OEM_RESET 0xE9
#define PCKEY_OEM_JUMP 0xEA
#define PCKEY_OEM_PA1 0xEB
#define PCKEY_OEM_PA2 0xEC
#define PCKEY_OEM_PA3 0xED
#define PCKEY_OEM_WSCTRL 0xEE
#define PCKEY_OEM_CUSEL 0xEF
#define PCKEY_OEM_ATTN 0xF0
#define PCKEY_OEM_FINNISH 0xF1
#define PCKEY_OEM_COPY 0xF2
#define PCKEY_OEM_AUTO 0xF3
#define PCKEY_OEM_ENLW 0xF4
#define PCKEY_OEM_BACKTAB 0xF5
#define PCKEY_ATTN 0xF6
#define PCKEY_CRSEL 0xF7
#define PCKEY_EXSEL 0xF8
#define PCKEY_EREOF 0xF9
#define PCKEY_PLAY 0xFA
#define PCKEY_ZOOM 0xFB
#define PCKEY_NONAME 0xFC
#define PCKEY_PA1 0xFD
#define PCKEY_OEM_CLEAR 0xFE

#endif
