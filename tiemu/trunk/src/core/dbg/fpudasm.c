/* Hey EMACS -*- linux-c -*- */
/* $Id: error.c 680 2004-10-11 16:45:27Z roms $ */

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

/*
    A pseudo-FPU disassembler.
	Many informations comes from the TI's BCD artihmetic package:
	<ftp://ftp.ti.com/pub/graph-ti/calc-apps/89/asm/exec.inc>
*/

#include <stdio.h>
#include <string.h>

/*
; BCD arithmetic package

***************************************************************************
*                        OPERATOR / OPERAND WORD                          *
*                                                                         *
*    | 15| 14| 13| 12| 11| 10| 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |    *
*    +---------------+-----------+-------------------+---------------+    *
*    |   OPERATOR    |   SIZE    |    SRC OPERAND    |  DEST OPERAND |    *
*    +---------------+-----------+-------------------+---------------+    *
*        FCMP   0      BYTE    0     FP0          0    R0 (FP or D) 0     *
*        FADD   1      WORD    1      |                 |                 *
*        FDIV   2      LONG    2     FP7          7    R7 (FP or D) 7     *
*        FMUL   3      SINGLE  3     D0           8    FRAME_OFF    8     *
*        FSUB   4      DOUBLE  4      |                EFFECT_ADDR  9     *
*        FINTRZ 5      UNSGNED 5     D7          15    RETURN_REG  10     *
*        FMOVE  6                    IMMED_LONG  16                       *
*        FNEG   7                    IMMED_SHORT 17                       *
*        FTST   8                    FRAME_OFF   18                       *
*                                    EFFECT_ADDR 19                       *
*                                    IMMED_ZERO  20                       *
***************************************************************************
bcdCmp          = 0x0000
bcdAdd          = 0x1000
bcdDiv          = 0x2000
bcdMul          = 0x3000
bcdSub          = 0x4000
bcdIntz         = 0x5000
bcdMove         = 0x6000
bcdNeg          = 0x7000
bcdTst          = 0x8000

bcdByte         = 0x0000
bcdWord         = 0x0200
bcdLong         = 0x0400
bcdSingle       = 0x0600
bcdDouble       = 0x0800
bcdUnsigned     = 0x0A00

; Source operand
bcdFP0          = 0x0000
bcdFP1          = 0x0010
bcdFP2          = 0x0020
bcdFP3          = 0x0030
bcdFP4          = 0x0040
bcdFP5          = 0x0050
bcdFP6          = 0x0060
bcdFP7          = 0x0070
bcdD0           = 0x0080
bcdD1           = 0x0090
bcdD2           = 0x00A0
bcdD3           = 0x00B0
bcdD4           = 0x00C0
bcdD5           = 0x00D0
bcdD6           = 0x00E0
bcdD7           = 0x00F0
bcdLongImm      = 0x0100
bcdShortImm     = 0x0110
bcdFrameSrc     = 0x0120
bcdAbsSrc       = 0x0130
bcdZeroImm      = 0x0140

; Destination operand
bcdR0           = 0x0000
bcdR1           = 0x0001
bcdR2           = 0x0002
bcdR3           = 0x0003
bcdR4           = 0x0004
bcdR5           = 0x0005
bcdR6           = 0x0006
bcdR7           = 0x0007
bcdFrameDest    = 0x0008
bcdAbsDest      = 0x0009
bcdRetReg       = 0x000A
*/

int DasmFPU(uint16_t code)
{

	return 0;
}