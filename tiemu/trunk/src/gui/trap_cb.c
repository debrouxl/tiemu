/* Hey EMACS -*- linux-c -*- */
/* $Id: hid.c 163 2004-05-13 06:40:57Z roms $ */

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
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details. *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>

#include "trap_cb.h"
#include "trap_dbox.h"
#include "support.h"

//#include "platform.h"
#include "struct.h"

#include "interface.h"
#include "bkpts.h"

/******************************/ 
/* Definitions & declarations */
/******************************/

struct exceptions_
{
  gint address_error;
  gint illegal_instruction;
  gint divide_by_zero;
  gint chk_instruction;
  gint trapv_instruction;
  gint privilege_violation;
  gint trace;
  gint line_1010;
  gint line_1111;
  gint noninit_interrupt;
};
typedef struct exceptions_ Exceptions;

struct auto_ints_
{
  gint int1;
  gint int2;
  gint int4;
  gint int5;
  gint int6;
  gint int7;
};
typedef struct auto_ints_ Autoints;

struct traps_
{
  gint trap0;
  gint trap1;
  gint trap2;
  gint trap3;
  gint trap4;
  gint trap5;
  gint trap6;
  gint trap7;
  gint trap8;
  gint trap9;
  gint trap10;
  gint trap11;
  gint trap12;
  gint trap13;
  gint trap14;
  gint trap15;
};
typedef struct traps_ Traps;

static Exceptions exceptions = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static Exceptions tmp_exceptions;

static Autoints autoints = { 0, 0, 0, 0, 0, 0 };
static Autoints tmp_autoints;

static Traps traps = { 0, 0, 0, 0, 0, 0, 0, 0, 
		       0, 0, 0, 0, 0, 0, 0, 0 };
static Traps tmp_traps;

/*********/
/* D Box */
/*********/

gint display_trap_dbox(void)
{
  GtkWidget *dbox;

  dbox = create_trap_dbox();
  memcpy(&tmp_exceptions, &exceptions, sizeof(Exceptions));
  memcpy(&tmp_autoints, &autoints, sizeof(Autoints));
  memcpy(&tmp_traps, &traps, sizeof(Traps));

  /* 'Exception' frame */
  if(tmp_exceptions.address_error != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton1")),
				 TRUE);

  if(tmp_exceptions.illegal_instruction != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton2")),
				 TRUE);

  if(tmp_exceptions.divide_by_zero != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton3")),
				 TRUE);

  if(tmp_exceptions.chk_instruction != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton4")),
				 TRUE);

  if(tmp_exceptions.trapv_instruction != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton5")),
				 TRUE);

  if(tmp_exceptions.privilege_violation != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton6")),
				 TRUE);
  
  if(tmp_exceptions.trace != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton7")),
				 TRUE);

  if(tmp_exceptions.line_1010 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton8")),
				 TRUE);

  if(tmp_exceptions.line_1111 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton9")),
				 TRUE);

  /* 'Auto-ints' frame */
  if(tmp_autoints.int1 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton10")),
				 TRUE);

  if(tmp_autoints.int2 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton11")),
				 TRUE);

  if(tmp_autoints.int4 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton12")),
				 TRUE);

  if(tmp_autoints.int5 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton13")),
				 TRUE);

  if(tmp_autoints.int6 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton14")),
				 TRUE);

  if(tmp_autoints.int7 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton15")),
				 TRUE);

  /* 'Traps' frame */
  if(tmp_traps.trap0 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton16")),
				 TRUE);

  if(tmp_traps.trap1 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton17")),
				 TRUE);

  if(tmp_traps.trap2 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton18")),
				 TRUE);
  
  if(tmp_traps.trap3 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton19")),
				 TRUE);

  if(tmp_traps.trap4 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton20")),
				 TRUE);

  if(tmp_traps.trap5 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton21")),
				 TRUE);

  if(tmp_traps.trap6 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton22")),
				 TRUE);

  if(tmp_traps.trap7 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton23")),
				 TRUE);

  if(tmp_traps.trap8 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton24")),
				 TRUE);

  if(tmp_traps.trap9 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton25")),
				 TRUE);

  if(tmp_traps.trap10 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton26")),
				 TRUE);

  if(tmp_traps.trap11 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton27")),
				 TRUE);

  if(tmp_traps.trap12 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton28")),
				 TRUE);

  if(tmp_traps.trap13 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton29")),
				 TRUE);

  if(tmp_traps.trap14 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton30")),
				 TRUE);

  if(tmp_traps.trap15 != 0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dbox, "checkbutton31")),
				 TRUE);

  gtk_widget_show_all(dbox);
  return 0;
}

/*************/
/* Callbacks */
/*************/

/* Exceptions */
void
on_checkbutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.address_error = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      //tmp_exceptions.address_error = setBreakpointVector(BK_ADDRESS_ERROR);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.address_error);
      exceptions.address_error = 0;
    }
  */
}


void
on_checkbutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.illegal_instruction = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      exceptions.illegal_instruction = 
	setBreakpointVector(BK_ILLEGAL_INSTRUCTION);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.illegal_instruction);
      exceptions.illegal_instruction = 0;
    }
  */
}

void
on_checkbutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.divide_by_zero = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      exceptions.divide_by_zero = setBreakpointVector(BK_ZERO_DIVIDE);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.divide_by_zero);
      exceptions.divide_by_zero = 0;
    }
  */
}


void
on_checkbutton4_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.chk_instruction = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      exceptions.chk_instruction = setBreakpointVector(BK_CHK_INSTRUCTION);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.chk_instruction);
      exceptions.chk_instruction = 0;
    }
  */
}


void
on_checkbutton5_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.trapv_instruction = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      exceptions.trapv_instruction = setBreakpointVector(BK_TRAPV_INSTRUCTION);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.trapv_instruction);
      exceptions.trapv_instruction = 0;
    }
  */
}


void
on_checkbutton6_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.privilege_violation = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      exceptions.privilege_violation = 
	setBreakpointVector(BK_PRIVILEGE_VIOLATION);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.privilege_violation);
      exceptions.privilege_violation = 0;
    }
  */
}


void
on_checkbutton7_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.trace = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      exceptions.trace = setBreakpointVector(BK_TRACE);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.trace);
      exceptions.trace = 0;
    }
  */
}


void
on_checkbutton8_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.line_1010 = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      exceptions.line_1010 = setBreakpointVector(BK_LINE1010);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.line_1010);
      exceptions.line_1010 = 0;
    }
  */
}


void
on_checkbutton9_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_exceptions.line_1111 = gtk_toggle_button_get_active(togglebutton);
  /*
  if(gtk_toggle_button_get_active(togglebutton))
    {
      exceptions.line_1111 = setBreakpointVector(BK_LINE_1111);
    }
  else
    {
      delBreakpointVector(tmp_exceptions.line_1111);
      exceptions.line_1111 = 0;
    }
  */
}

/* Auto-ints */
void
on_checkbutton10_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_autoints.int1 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton11_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_autoints.int2 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton12_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_autoints.int4 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton13_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_autoints.int5 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton15_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_autoints.int6 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton14_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_autoints.int7 = gtk_toggle_button_get_active(togglebutton);
}

/* Traps */
void
on_checkbutton16_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap0 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton17_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap1 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton18_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap2 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton19_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap3 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton20_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap4 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton21_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap5 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton22_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap6 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton23_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap7 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton24_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap8 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton25_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap9 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton26_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap10 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton27_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap11 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton28_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap12 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton29_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap13 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton30_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap14 = gtk_toggle_button_get_active(togglebutton);
}


void
on_checkbutton31_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  tmp_traps.trap15 = gtk_toggle_button_get_active(togglebutton);
}


void
on_button46_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  // ok button
  memcpy(&exceptions, &tmp_exceptions, sizeof(Exceptions));
  memcpy(&autoints, &tmp_autoints, sizeof(Autoints));
  memcpy(&traps, &tmp_traps, sizeof(Traps));

  // exceptions
  if(exceptions.address_error != 0)
    {
      ti68k_setBreakpointVector(BK_ADDRESS_ERROR);
    }
  else
    {
      ti68k_delBreakpointVector(BK_ADDRESS_ERROR);
      exceptions.address_error = 0;
    }

  if(exceptions.illegal_instruction != 0)
    {
      ti68k_setBreakpointVector(BK_ILLEGAL_INSTRUCTION);
    }
  else
    {
      ti68k_delBreakpointVector(BK_ILLEGAL_INSTRUCTION);
      exceptions.illegal_instruction = 0;
    }

  if(exceptions.divide_by_zero != 0)
    {
      ti68k_setBreakpointVector(BK_ZERO_DIVIDE);
    }
  else
    {
      ti68k_delBreakpointVector(BK_ZERO_DIVIDE);
      exceptions.divide_by_zero = 0;
    }

  if(exceptions.chk_instruction != 0)
    {
      ti68k_setBreakpointVector(BK_CHK_INSTRUCTION);
    }
  else
    {
      ti68k_delBreakpointVector(BK_CHK_INSTRUCTION);
      exceptions.chk_instruction = 0;
    }

  if(exceptions.trapv_instruction != 0)
    {
      ti68k_setBreakpointVector(BK_TRAPV_INSTRUCTION);
    }
  else
    {
      ti68k_delBreakpointVector(BK_TRAPV_INSTRUCTION);
      exceptions.trapv_instruction = 0;
    }

  if(exceptions.privilege_violation != 0)
    {
      ti68k_setBreakpointVector(BK_PRIVILEGE_VIOLATION);
    }
  else
    {
      ti68k_delBreakpointVector(BK_PRIVILEGE_VIOLATION);
      exceptions.privilege_violation = 0;
    }

  if(exceptions.trace != 0)
    {
      ti68k_setBreakpointVector(BK_TRACE);
    }
  else
    {
      ti68k_delBreakpointVector(BK_TRACE);
      exceptions.trace = 0;
    }

  if(exceptions.line_1010 != 0)
    {
      ti68k_setBreakpointVector(BK_LINE_1010);
    }
  else
    {
      ti68k_delBreakpointVector(BK_LINE_1010);
      exceptions.line_1010 = 0;
    }

  if(exceptions.line_1111 != 0)
    {
      ti68k_setBreakpointVector(BK_LINE_1111);
    }
  else
    {
      ti68k_delBreakpointVector(BK_LINE_1111);
      exceptions.line_1111 = 0;
    }  

  if(exceptions.noninit_interrupt != 0)
    {
      ti68k_setBreakpointVector(BK_NONINIT_INTERRUPT);
    }
  else
    {
      ti68k_delBreakpointVector(BK_NONINIT_INTERRUPT);
      exceptions.noninit_interrupt = 0;
    }  

  //auto-ints
  if(autoints.int1 != 0)
    {
      ti68k_setBreakpointAutoint(BK_AUTOINT_1);
    }
  else
    {
      ti68k_delBreakpointAutoint(BK_AUTOINT_1);
      autoints.int1 = 0;
    }

  if(autoints.int2 != 0)
    {
      ti68k_setBreakpointAutoint(BK_AUTOINT_2);
    }
  else
    {
      ti68k_delBreakpointAutoint(BK_AUTOINT_2);
      autoints.int2 = 0;
    }

  if(autoints.int4 != 0)
    {
      ti68k_setBreakpointAutoint(BK_AUTOINT_4);
    }
  else
    {
      ti68k_delBreakpointAutoint(BK_AUTOINT_4);
      autoints.int4 = 0;
    }

  if(autoints.int5 != 0)
    {
      ti68k_setBreakpointAutoint(BK_AUTOINT_5);
    }
  else
    {
      ti68k_delBreakpointAutoint(BK_AUTOINT_5);
      autoints.int5 = 0;
    }

  if(autoints.int6 != 0)
    {
      ti68k_setBreakpointAutoint(BK_AUTOINT_6);
    }
  else
    {
      ti68k_delBreakpointAutoint(BK_AUTOINT_6);
      autoints.int6 = 0;
    }

  if(autoints.int7 != 0)
    {
      ti68k_setBreakpointAutoint(BK_AUTOINT_7);
    }
  else
    {
      ti68k_delBreakpointAutoint(BK_AUTOINT_7);
      autoints.int7 = 0;
    }

  // traps
  if(traps.trap0 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_0);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap0);
      traps.trap0 = 0;
    }
  
  if(traps.trap1 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_1);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap1);
      traps.trap1 = 0;
    }

  if(traps.trap2 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_2);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap2);
      traps.trap2 = 0;
    }

  if(traps.trap3 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_3);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap3);
      traps.trap3 = 0;
    }

  if(traps.trap4 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_4);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap4);
      traps.trap4 = 0;
    }

  if(traps.trap5 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_5);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap5);
      traps.trap5 = 0;
    }

  if(traps.trap6 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_6);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap6);
      traps.trap6 = 0;
    }

  if(traps.trap7 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_7);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap7);
      traps.trap7 = 0;
    }

  if(traps.trap8 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_8);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap8);
      traps.trap8 = 0;
    }

  if(traps.trap9 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_9);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap9);
      traps.trap9 = 0;
    }

  if(traps.trap10 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_A);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap10);
      traps.trap10 = 0;
    }

  if(traps.trap11 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_B);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap11);
      traps.trap11 = 0;
    }

  if(traps.trap12 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_C);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap12);
      traps.trap12 = 0;
    }

  if(traps.trap13 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_D);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap13);
      traps.trap13 = 0;
    }

  if(traps.trap14 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_E);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap14);
      traps.trap14 = 0;
    }

  if(traps.trap15 != 0)
    {
      ti68k_setBreakpointTrap(BK_TRAP_F);
    }
  else
    {
      ti68k_delBreakpointTrap(traps.trap15);
      traps.trap15 = 0;
    }

  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "trap_dbox"));
}


void
on_button47_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  // cancel button
  gtk_widget_destroy(lookup_widget(GTK_WIDGET(button), "trap_dbox"));
}
