/* Hey EMACS -*- linux-c -*- */
/* $Id: dbg_cause.c 446 2004-06-13 09:11:07Z roms $ */

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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk/gdkkeysyms.h>

#include "intl.h"
#include "paths.h"
#include "skinops.h"
#include "keydefs.h"
#include "support.h"
#include "ti68k_int.h"
#include "popup.h"
#include "screenshot.h"
#include "romversion.h"
#include "fs_misc.h"
#include "calc.h"

const char* key_mapping;
static int alpha = 0;

static int gdk_to_ti89(guint keyval)
{
    alpha = 0;
    switch(keyval) 
	{
		// Alphabetical
		case GDK_a : alpha = 1; 	return TIKEY_EQUALS;
		case GDK_b : alpha = 1; 	return TIKEY_PALEFT;
		case GDK_c : alpha = 1; 	return TIKEY_PARIGHT;
		case GDK_d : alpha = 1; 	return TIKEY_COMMA;
		case GDK_e : alpha = 1; 	return TIKEY_DIVIDE;
		case GDK_f : alpha = 1; 	return TIKEY_PIPE;
		case GDK_g : alpha = 1; 	return TIKEY_7;
		case GDK_h : alpha = 1; 	return TIKEY_8;
		case GDK_i : alpha = 1; 	return TIKEY_9;
		case GDK_j : alpha = 1; 	return TIKEY_MULTIPLY;
		case GDK_k : alpha = 1; 	return TIKEY_EE;
		case GDK_l : alpha = 1; 	return TIKEY_4;
		case GDK_m : alpha = 1; 	return TIKEY_5;
		case GDK_n : alpha = 1; 	return TIKEY_6;
		case GDK_o : alpha = 1; 	return TIKEY_MINUS;
		case GDK_p : alpha = 1; 	return TIKEY_STORE;
		case GDK_q : alpha = 1; 	return TIKEY_1;
		case GDK_r : alpha = 1; 	return TIKEY_2;
		case GDK_s : alpha = 1; 	return TIKEY_3;
		case GDK_t : 				return TIKEY_T;
		case GDK_u : alpha = 1; 	return TIKEY_PLUS;
		case GDK_v : alpha = 1; 	return TIKEY_0;
		case GDK_w : alpha = 1; 	return TIKEY_PERIOD;
		case GDK_x : 				return TIKEY_X;
		case GDK_y : 				return TIKEY_Y;
		case GDK_z : 				return TIKEY_Z;
		
		// Numerical
		case GDK_KP_0 : return TIKEY_0;
		case GDK_KP_1 : return TIKEY_1;
		case GDK_KP_2 : return TIKEY_2;
		case GDK_KP_3 : return TIKEY_3;
		case GDK_KP_4 : return TIKEY_4;
		case GDK_KP_5 : return TIKEY_5;
		case GDK_KP_6 : return TIKEY_6;
		case GDK_KP_7 : return TIKEY_7;
		case GDK_KP_8 : return TIKEY_8;
		case GDK_KP_9 : return TIKEY_9;
		case GDK_0 : 	return TIKEY_0;
		case GDK_1 : 	return TIKEY_1;
		case GDK_2 : 	return TIKEY_2;
		case GDK_3 : 	return TIKEY_3;
		case GDK_4 : 	return TIKEY_4;
		case GDK_5 : 	return TIKEY_5;
		case GDK_6 : 	return TIKEY_6;
		case GDK_7 : 	return TIKEY_7;
		case GDK_8 : 	return TIKEY_8;
		case GDK_9 : 	return TIKEY_9;
		
		// Arrows
		case GDK_Up : 		return TIKEY_UP;
		case GDK_Left : 	return TIKEY_LEFT;
		case GDK_Right : 	return TIKEY_RIGHT;
		case GDK_Down : 	return TIKEY_DOWN;
		
		// Functions
		case GDK_F1 : return TIKEY_F1;
		case GDK_F2 : return TIKEY_F2;
		case GDK_F3 : return TIKEY_F3;
		case GDK_F4 : return TIKEY_F4;
		case GDK_F5 : return TIKEY_F5;
		case GDK_F7 : return TIKEY_F7;
		case GDK_F8 : return TIKEY_F8;
		
		// Standard
		case GDK_Return : 	    return TIKEY_ENTER1;
		case GDK_KP_Enter:      return TIKEY_ENTER2;
		case GDK_Shift_L : 	    return TIKEY_SHIFT;
		case GDK_Shift_R : 	    return TIKEY_SHIFT;
		case GDK_Control_L : 	return TIKEY_DIAMOND;
		case GDK_Control_R : 	return TIKEY_DIAMOND;
		case GDK_Alt_L : 	    return TIKEY_2ND;
		case GDK_Alt_R : 	    return TIKEY_2ND;

		case GDK_space : alpha = 1;	return TIKEY_NEGATE;
		case GDK_Escape : 		return TIKEY_ESCAPE;
		case GDK_BackSpace : 	return TIKEY_BACKSPACE;
		case GDK_parenleft : 	return TIKEY_PALEFT;
		case GDK_parenright : 	return TIKEY_PARIGHT;
		case GDK_period : 		return TIKEY_PERIOD;
        case GDK_KP_Decimal:    return TIKEY_PERIOD;
		case GDK_comma : 		return TIKEY_COMMA;
		case GDK_KP_Add : 	    return TIKEY_PLUS;
		case GDK_KP_Multiply :  return TIKEY_MULTIPLY;
		case GDK_KP_Divide : 	return TIKEY_DIVIDE;    
		case GDK_KP_Subtract : 	return TIKEY_MINUS;
		case GDK_minus : 		return TIKEY_NEGATE;
		case GDK_backslash : 	return TIKEY_PIPE;
		case GDK_slash : 		return TIKEY_DIVIDE;
		case GDK_equal : 		return TIKEY_EQUALS;
		case GDK_less : 		return TIKEY_NEGATE;
		
		// Specific
		case GDK_F6 : 			return TIKEY_CATALOG;
		case GDK_F9  : 		    return TIKEY_APPS;
		case GDK_Tab : 		    return TIKEY_STORE;
		case GDK_Caps_Lock :	return TIKEY_ALPHA;
		case GDK_Page_Down : 	return TIKEY_MODE;
		// missing key here
		case GDK_Page_Up : 		return TIKEY_EE;	
		case GDK_Delete : 		return TIKEY_CLEAR;
		case GDK_Scroll_Lock :	return TIKEY_ON;
	
		default : return TIKEY_VOID;
	}
}

static int gdk_to_ti92(guint keyval)
{
    switch(keyval) 
	{
		// Alphabetical
		case GDK_a : 	return TIKEY_A;
		case GDK_b : 	return TIKEY_B;
		case GDK_c : 	return TIKEY_C;
		case GDK_d : 	return TIKEY_D;
		case GDK_e : 	return TIKEY_E;
		case GDK_f : 	return TIKEY_F;
		case GDK_g : 	return TIKEY_G;
		case GDK_h : 	return TIKEY_H;
		case GDK_i : 	return TIKEY_I;
		case GDK_j : 	return TIKEY_J;
		case GDK_k : 	return TIKEY_K;
		case GDK_l : 	return TIKEY_L;
		case GDK_m : 	return TIKEY_M;
		case GDK_n : 	return TIKEY_N;
		case GDK_o : 	return TIKEY_O;
		case GDK_p : 	return TIKEY_P;
		case GDK_q : 	return TIKEY_Q;
		case GDK_r : 	return TIKEY_R;
		case GDK_s : 	return TIKEY_S;
		case GDK_t : 	return TIKEY_T;
		case GDK_u : 	return TIKEY_U;
		case GDK_v : 	return TIKEY_V;
		case GDK_w : 	return TIKEY_W;
		case GDK_x : 	return TIKEY_X;
		case GDK_y : 	return TIKEY_Y;
		case GDK_z : 	return TIKEY_Z;
		
		// Numerical
		case GDK_KP_0 : return TIKEY_0;
		case GDK_KP_1 : return TIKEY_1;
		case GDK_KP_2 : return TIKEY_2;
		case GDK_KP_3 : return TIKEY_3;
		case GDK_KP_4 : return TIKEY_4;
		case GDK_KP_5 : return TIKEY_5;
		case GDK_KP_6 : return TIKEY_6;
		case GDK_KP_7 : return TIKEY_7;
		case GDK_KP_8 : return TIKEY_8;
		case GDK_KP_9 : return TIKEY_9;
		case GDK_0 : 	return TIKEY_0;
		case GDK_1 : 	return TIKEY_1;
		case GDK_2 : 	return TIKEY_2;
		case GDK_3 : 	return TIKEY_3;
		case GDK_4 : 	return TIKEY_4;
		case GDK_5 : 	return TIKEY_5;
		case GDK_6 : 	return TIKEY_6;
		case GDK_7 : 	return TIKEY_7;
		case GDK_8 : 	return TIKEY_8;
		case GDK_9 : 	return TIKEY_9;
		
		// Arrows
		case GDK_Up : 		return TIKEY_UP;
		case GDK_Left : 	return TIKEY_LEFT;
		case GDK_Right : 	return TIKEY_RIGHT;
		case GDK_Down : 	return TIKEY_DOWN;
		
		// Functions
		case GDK_F1 : return TIKEY_F1;
		case GDK_F2 : return TIKEY_F2;
		case GDK_F3 : return TIKEY_F3;
		case GDK_F4 : return TIKEY_F4;
		case GDK_F5 : return TIKEY_F5;
		case GDK_F6 : return TIKEY_F6;
		case GDK_F7 : return TIKEY_F7;
		case GDK_F8 : return TIKEY_F8;
		
		// Standard
		case GDK_Return :	    return TIKEY_ENTER1;
		case GDK_KP_Enter:      return TIKEY_ENTER2;
		case GDK_Shift_L : 	    return TIKEY_SHIFT;
		case GDK_Shift_R : 	    return TIKEY_SHIFT;
		case GDK_Control_L : 	return TIKEY_DIAMOND;
		case GDK_Control_R : 	return TIKEY_DIAMOND;
		case GDK_Alt_L : 	    return TIKEY_2ND;
		case GDK_Alt_R : 	    return TIKEY_2ND;
		case GDK_space : 		return TIKEY_SPACE;
		case GDK_Escape : 		return TIKEY_ESCAPE;
		case GDK_BackSpace : 	return TIKEY_BACKSPACE;
		case GDK_parenleft : 	return TIKEY_PALEFT;
		case GDK_parenright : 	return TIKEY_PARIGHT;
		case GDK_period : 		return TIKEY_PERIOD;
        case GDK_KP_Decimal:    return TIKEY_PERIOD;
		case GDK_comma : 		return TIKEY_COMMA;
		case GDK_KP_Add : 	    return TIKEY_PLUS;
		case GDK_KP_Multiply :  return TIKEY_MULTIPLY;
		case GDK_KP_Divide : 	return TIKEY_DIVIDE;    
		case GDK_slash : 		return TIKEY_DIVIDE;
		case GDK_KP_Subtract : 	return TIKEY_MINUS;			
		case GDK_minus : 		return TIKEY_NEGATE;
		case GDK_equal : 		return TIKEY_EQUALS;
		case GDK_less : 		return TIKEY_NEGATE;			
		
		// Specific
		case GDK_F9  : 		    return TIKEY_APPS;
		case GDK_semicolon : 	return TIKEY_THETA;
		case GDK_Tab : 		    return TIKEY_STORE;
		case GDK_Caps_Lock : 	return TIKEY_HAND;
		case GDK_Page_Down : 	return TIKEY_MODE;
		case GDK_backslash : 	return TIKEY_LN;			
		case GDK_Insert : 		return TIKEY_SIN;
		case GDK_Home : 		return TIKEY_COS;
		case GDK_Page_Up : 		return TIKEY_TAN;
		case GDK_Delete : 		return TIKEY_CLEAR;
		case GDK_Scroll_Lock : 	return TIKEY_ON;			

		default : return TIKEY_VOID;
	}
}

static int gdk_to_ti(guint keyval) 
{
  	if((tihw.calc_type != TI89) && (tihw.calc_type != TI89t))
        return gdk_to_ti92(keyval);
    else
        return gdk_to_ti89(keyval);

    return 0;
}

/* 
   Converts the mouse position into a TIKEY_xx code
   Checks if the mouse cursor is within a rectangle defined in 
   the key_pos array
*/
static int pos_to_key(int x, int y) 
{
  	int i;
  	RECT *kp = skin_infos.keys_pos;
    int nkeys = sizeof(skin_infos.keys_pos) / sizeof(RECT);
  
  	for(i = 0; i < nkeys; i++)
    {
      	if((x >= kp[i].left) && (x < kp[i].right) && 
	 	   (y >= kp[i].top) && (y < kp[i].bottom)) 
		
		return key_mapping[i];
    }
  
  	return -1;
}

GLADE_CB gboolean
on_calc_wnd_button_press_event     (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    if(event->type != GDK_BUTTON_PRESS)
        return FALSE;

	if (event->button == 3) 
    {
        GdkEventButton *bevent;
        GtkWidget *menu;

		bevent = (GdkEventButton *) (event);

        ti68k_engine_stop();

        menu = display_popup_menu();
		gtk_menu_popup(GTK_MENU(menu),
				   NULL, NULL, NULL, NULL,
				   bevent->button, bevent->time);
	    gtk_widget_show(menu);

		return TRUE;
	}

    if(event->button == 1)
    {
        int key = pos_to_key((int)event->x, (int)event->y);
        if(key < 0)
            return FALSE;

        ti68k_kbd_set_key(key, 1);
        return TRUE;
    }

	return FALSE;
}

GLADE_CB gboolean
on_calc_wnd_button_release_event     (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    if(event->type != GDK_BUTTON_RELEASE)
        return FALSE;

    if(event->button == 1)
    {
        int key = pos_to_key((int)event->x, (int)event->y);
        if(key < 0)
            return FALSE;
	    
      	ti68k_kbd_set_key(key, 0);
        return TRUE;
    }

    return FALSE;
}


GLADE_CB gboolean
on_calc_wnd_key_press_event        (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
    if(event->hardware_keycode == 0x0014)
        event->keyval = GDK_Caps_Lock;

    if(event->hardware_keycode == 0x0013)
	{
        hid_screenshot(NULL);
        return TRUE;
    } 
    else if(event->keyval == GDK_F10)
    {
        display_tifile_dbox();
        return TRUE;
    }
    else if(event->keyval == GDK_F11)
    {
        if(!dbg_on)
            ti68k_debug_break();
        return TRUE;
    }
    else if(event->keyval == GDK_F12)
    {
        display_romversion_dbox();
        return TRUE;
    }
    else
    {
        int key = gdk_to_ti(event->keyval);
        if(alpha)
			ti68k_kbd_set_key(TIKEY_ALPHA, 1);
	    ti68k_kbd_set_key(key, 1);
        return TRUE;
    }

    return FALSE;
}


GLADE_CB gboolean
on_calc_wnd_key_release_event      (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
    if(event->hardware_keycode == 0x0014)
        event->keyval = GDK_Caps_Lock;

    {
        int key = gdk_to_ti(event->keyval);
        if(alpha)
			ti68k_kbd_set_key(TIKEY_ALPHA, alpha =0);
        ti68k_kbd_set_key(key, 0);
        return TRUE;
    }

    return FALSE;
}

