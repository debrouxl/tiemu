/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*
 *   skinedit - a skin editor for the TiEmu emulator
 *   Copyright (C) 2002 Julien BLACHE <jb@tilp.info>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __KEYMAPS_H__
#define __KEYMAPS_H__


const char *keynames[5][SKIN_KEYS + 1]= 
  {
    /* 0 => TI-73 */
    {"Y=", "Window", "Zoom", "Trace",
     "Graph", "2nd", "Mode", "Del",
     "Left", "Right", "Up", "Down",
     "Math", "Draw", "List", "x^2",
     "^", "Prgm", "Apps", "Clear",
     "Unit", "b/c", "F<->D", "A(b/c) <-> d/e",
     "Const", "Simp", "%", "(",
     ")", "/ (Divide)", "x", "7",
     "8", "9", "*", ", (Comma)",
     "4", "5", "6", "-",
     "Sto>", "1", "2", "3",
     "+", "On", "0", ". (Period)",
     "(-)", "Enter", NULL}, 
    /* 1 => TI-82, TI-83, TI-83+ */
    {"Y=", "Window", "Zoom", "Trace",
     "Graph", "2nd", "Mode", "Del",
     "Left", "Right", "Up", "Down",
     "Alpha", "X,T,theta", "Stat",
     "Math", "Matrx", "Prgm", "Vars",
     "Clear", "x^-1", "sin", "cos",
     "tan", "^", "x^2", ", (Comma)",
     "(", ")", "/ (Divide)", "log",
     "7", "8", "9", "*",
     "ln", "4", "5", "6",
     "-", "Sto>", "1", "2",
     "3", "+", "On", "0",
     ". (Period)", "(-)", "Enter", NULL}, 
    /* 2 => TI-85, TI-86 */     
    {"F1", "F2", "F3", "F4",
     "F5", "2nd", "Exit", "More",
     "Left", "Right", "Up", "Down",
     "Alpha", "x-var", "Del", "Graph",
     "Stat", "Prgm", "Custom", "Clear",
     "log", "sin", "cos", "tan",
     "^", "ln", "ee", "(",
     ")", "/ (Divide)", "x^2", "7",
     "8", "9", "*", ", (Comma)",
     "4", "5", "6", "-",
     "Sto>", "1", "2", "3",
     "+", "On", "0", ". (Period)",
     "(-)", "Enter", NULL},
    /* 3 => TI-89 */
    {"F1", "F2", "F3", "F4",
     "F5", "2nd", "Shift", "Esc",
     "Left", "Right", "Up", "Down",
     "Diamond", "Alpha", "Apps", "Home",
     "Mode", "Catalog", "Backspace", "Clear",
     "X", "Y", "Z", "T",
     "^", "=", "(", ")",
     ", (Comma)", "/ (Divide)", "|", "7",
     "8", "9", "*", "ee",
     "4", "5", "6", "-",
     "Sto>", "1", "2", "3",
     "+", "On", "0", ". (Period)",
     "(-)", "Enter", NULL},
    /* 4 => TI-92, TI-92+ */
    {"Grab (Hand)", "F1", "F2", "F3",
     "F4", "F5", "F6", "F7",
     "F8", "Q", "W", "E",
     "R", "T", "Y", "U",
     "I", "O", "P", "A",
     "S", "D", "F", "G",
     "H", "J", "K", "L",
     "Z", "X", "C", "V",
     "B", "N", "M", "Theta",
     "Shift", "On", "Diamond", "2nd (Alpha area)",
     "Sto>", "Space", "=", "Backspace",
     "Enter (Alpha area)", "2nd (Cursor area)",
     "Esc", "Mode", "Clear", "ln",
     "Apps", "Enter (Cursor area)", "Left", "Right",
     "Up", "Down", "sin", "cos",
     "tan", "^", "(", ")", ", (Comma)",
     "/ (Divide)", "7", "8", "9",
     "*", "4", "5", "6",
     "-", "1", "2", "3",
     "+", "0", ". (Period)",
     "(-)", "Enter (Numeric area)", NULL}
  }; 

#endif /* !__KEYMAPS_H__ */
