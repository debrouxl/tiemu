/*  gtktiemu - a TI89/92/92+ emulator
 *  (c) Copyright 2000-2001, Romain Lievin and Thomas Corvazier
 *  (c) Copyright 2001-2002, Romain Lievin
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

#ifndef __TI68K_CALLBACKS__
#define __TI68K_CALLBACKS__

#include <time.h>
//#include "tilibs.h"

/*
  Variables
*/

// Callbacks type
typedef int  (*callback_iv_t) (void);
typedef int  (*callback_ii_t) (int);
typedef void (*callback_vi_t) (int);
typedef void (*callback_vv_t) (void);

// Function pointers
extern callback_iv_t cb_launch_debugger;
extern callback_vi_t cb_update_progbar;

extern callback_iv_t cb_init_specific;
extern callback_iv_t cb_exit_specific;
extern callback_ii_t cb_set_contrast;
extern callback_iv_t cb_update_screen;
extern callback_iv_t cb_update_keys;
extern callback_vi_t cb_screen_on_off;

/* 
   Refresh/progress functions
   This structure allows to implement a kind of callbacks mechanism (which
   allow libti68k to interact with user without being dependant of a GUI).
*/

  struct ti68k_info_update
  {
    /* Variables to update */
    int cancel;                // Abort the current transfer
    char label_text[256];      // A text to display (varname, ...)
    float percentage;          // Percentage of the current operation
    float main_percentage;     // Percentage of all operations
    float prev_percentage;     // Previous percentage of current operation
    float prev_main_percentage;// Previous percentage of all operations
    int count;                 // Number of bytes exchanged
    int total;                 // Number of bytes to exchange
    clock_t start_time;        // Time when transfer has begun
    clock_t current_time;      // Current time
    
    /* Functions for updating */
    void (*start)   (void);                   // Init internal variables
    void (*stop)    (void);                   // Release internal variables
    void (*refresh) (void);                   // Pass control to GUI for refresh
    void (*msg_box) (const char *t, char *s); // Display a message box
    void (*pbar)    (void);                   // Refresh the progress bar
    void (*label)   (void);                   // Refresh the label
    int  (*choose)  (char *cur_name, 
		     char *new_name);         // Display choice box
  };
  typedef struct ti68k_info_update Ti68kInfoUpdate;


/* 
   Functions 
*/

void ti68k_set_update(Ti68kInfoUpdate *iu,
		      void (*start)    (void),
		      void (*stop)     (void),
		      void (*resfresh) (void),
		      void (*msg_box)  (const char *t, char *s),
		      void (*pbar)     (void),
		      void (*label)    (void),
		      int  (*choose)   (char *cur_name, 
					char *new_name));

int ti68k_setGuiCallbacks(callback_iv_t initSpecific, 
			  callback_iv_t exitSpecific,
			  callback_iv_t updateScreen,
			  callback_iv_t updateKeys,
			  callback_vi_t screenOnOff,
			  callback_ii_t setContrast
			  );

callback_iv_t ti68k_defineDebugger(callback_iv_t new_debugger);

callback_vi_t ti68k_defineProgbar(callback_vi_t new_progbar);

#endif
