/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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

#ifdef __WIN32__
#include "C:\SDL-1.2.7\include\SDL.h"
#else
# include <SDL/SDL.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "support.h"
#include "hid.h"
#include "skinops.h"
#include "struct.h"
#include "ti68k_int.h"
#include "screenshot.h"
#include "tie_error.h"
#include "keydefs.h"
#include "fs_misc.h"
#include "romversion.h"
#include "dbg_all.h"

#define DEFAULT_BPP   8
#define DEFAULT_FLAGS (SDL_HWPALETTE | SDL_HWSURFACE | SDL_RESIZABLE) 


/*****************************************/
/* Various variables and short functions */
/*****************************************/

#define NGS 16                // Number of gray scales

char *sTitle = "TiEmu II";    // Name of app in title bar

SDL_Color sdlPal[256];        // global palette: greyscale + skin
SDL_Surface *sdlWindow;       // SDL formatted surface (window)
SDL_Surface *sdlLcdSrc;       // SDL unformatted surface (lcd)
SDL_Surface *sdlLcdDst;       // SDL formatted surface (lcd)

int iBpp, iDepth;             // bytes per pixel & number of colors
int iScale = 0;               // scaling factor (1 or 2)
unsigned int iWinLineSize;    // length of a row (in bytes)
unsigned int iLcdLineSize;    // length of a row (in bytes)

Uint32 blackPixel;            // LCD black pixel value (true color)
Uint32 whitePixel;            // LCD white pixel value (true color)

int iWinW, iWinH;             // window size
int iScrW = 240, iScrH = 128; // LCD size (TI memory)
int iLcdW, iLcdH;             // LCD size (clipped)

Uint32 *pLcdBuf = NULL;       // LCD screen  (TI screen: bytemapped)
Uint32 convtab[512] = { 0 };  // Planar to chunky conversion table

Uint8 sdl2ti[512] = { 0 };    // SDL to TI key conversion table
int iKeyWasPressed;			  // true if a TI key has been pressed
int lastKey = -1;             // the latest key pressed
int iAlpha = 0;               // The TI89 Alpha key has been pressed

int bFullscreen=0;            // true if toggled in FullScreen mode
int bFullscreenFailed=0;      // true if failed to toggle in fullscreen
int bWindowedFailed=0;        // true if failed to toggle in windowed

int iContrast = NGS;          // current contrast level
int iLastContrast = 0;        // previous contrast level
int iNewContrast = NGS;       // new contrast level
int iGrayPlanes = -1;         // number of grayscales to emulate
int iCurrPlane = 0;           // ?
int iScrState = 0;            // screen state

const char* key_mapping = NULL; // key mapping
extern const char sknKey92[];   // in tikeys.c
extern const char sknKey89[];
extern const char sknKeyV2[];

static void compute_convtable(void);
static int sdl_to_ti(int key);
static void set_colors(void);
static void redraw_skin(void);

static int pos_to_key(int x, int y);

#define SCREEN_ON  (!0)
#define SCREEN_OFF	0

/* TI89: RGB = 49:46:34 & 204:204:207	*/
/* TI92: RGB = 83:111:138 & 174:204:176 */
/* V200: RGB = ?						*/

static void SDL_ComplainAndExit(void)
{
	gchar *s = g_strdup_printf("SDL problem: %s\n", SDL_GetError());
	tiemu_error(0, s);
	g_free(s);

	exit(-1);
}

/**************************/
/* Init/Exit entry points */
/**************************/

static int match_skin(int calc_type)
{
	SKIN_INFOS si;
	int ok;
	//gchar *str;

	// filename is "", load default skin
	if(!strcmp(g_basename(options.skin_file), ""))
	{
		g_free(options.skin_file);
      	options.skin_file = g_strdup_printf("%s%s.skn", 
			inst_paths.skin_dir, ti68k_calctype_to_string(calc_type));
		return -1;
	}

	// load skin header
	if(skin_read_header(options.skin_file, &si) == -1)
	{
		g_free(options.skin_file);
      	options.skin_file = g_strdup_printf("%s%s.skn", 
			inst_paths.skin_dir, ti68k_calctype_to_string(calc_type));
		return -1;
	}

	// is skin compatible
	switch(tihw.calc_type)
	{
	    case TI92:
		case TI92p:
            ok = !strcmp(si.calc, SKIN_TI92) || !strcmp(si.calc, SKIN_TI92P);
		break;
	    case TI89:
            ok = !strcmp(si.calc, SKIN_TI89);
		break;
		case V200:
			ok = !strcmp(si.calc, SKIN_V200);
		break;
	    default: 
            ok = 0;
		break;
	}

	if(!ok)
	{
		g_free(options.skin_file);
      	options.skin_file = g_strdup_printf("%s%s.skn", 
			inst_paths.skin_dir, ti68k_calctype_to_string(calc_type));

		tiemu_error(0, _("skin incompatible with the current calc model. Falling back to default skin."));
		return -1;
	}

	return 0;
}

/*
  	Initialize SDL, buffers and some other stuffs.
  	This function can be called at any time
*/
static int hid_init_subsystem(void)
{
	// Allocate the TI screen buffer
	pLcdBuf = malloc((iScrW << iScale) * (iScrH << iScale));
  
	// Get LCD size depending on calculator type
	if((tihw.calc_type == TI92) || (tihw.calc_type == TI92p))
    {
      	iLcdW = 240 << iScale; 
      	iLcdH = 128 << iScale;

		match_skin(tihw.calc_type);

      	if(skin_load(options.skin_file) == -1) {
	      	gchar *s = g_strdup_printf("unable to load this skin: <%s>\n", options.skin_file);
	      	tiemu_error(0, s);
	      	g_free(s);
	      	return -1;
      	}
      
      	key_mapping = sknKey92;
    }
	else if (tihw.calc_type == TI89)
    {
    	iLcdW = 160 << iScale; 
    	iLcdH = 100 << iScale;
      
      	match_skin(tihw.calc_type);

      	if(skin_load(options.skin_file) == -1) {
	      	gchar *s = g_strdup_printf("unable to load this skin: <%s>\n", options.skin_file);
	      	tiemu_error(0, s);
	      	g_free(s);
	      	return -1;
      	}

      	key_mapping = sknKey89;
    }
	else if(tihw.calc_type == V200)
    {
      	iLcdW = 240 << iScale; 
      	iLcdH = 128 << iScale;

      	g_free(options.skin_file);
      	options.skin_file = g_strconcat(inst_paths.skin_dir, "v200plt.skn", NULL);

      	if(skin_load(options.skin_file) == -1) {
	      	gchar *s = g_strdup_printf("unable to load this skin: <%s>\n", options.skin_file);
	      	tiemu_error(0, s);
	      	g_free(s);
	      	return -1;
      	}
      
      	key_mapping = sknKeyV2;
    }
	else
	{
	  	gchar *s = g_strdup_printf("no skin found for this calc\n");
	  	tiemu_error(0, s);
	  	g_free(s);
	  	return -1;
	}
  
  	// Init B&W pixel values)
  	whitePixel = skin_infos.lcd_white;
  	blackPixel = skin_infos.lcd_black;

  	// Get window size depending on windowed/fullscreen
  	if(params.background) 
    {
      	iWinH = skin_infos.height << iScale;
      	iWinW = skin_infos.width << iScale;
    }
  	else 
    {
      	iWinH = iLcdH << iScale;
      	iWinW = iLcdW << iScale;
    }

  	// Set VIDEO mode and create the window surface
  	sdlWindow = SDL_SetVideoMode(iWinW, iWinH, DEFAULT_BPP, DEFAULT_FLAGS);
  	if(sdlWindow == NULL)
    {
	  	gchar *s = g_strdup_printf("SDL: can't set video mode: %s\n", SDL_GetError());
	  	tiemu_error(0, s);
	  	g_free(s);
	  	return -1;
    }
  
  	// Create LCD surface to blit into window
  	sdlLcdSrc = SDL_CreateRGBSurface(SDL_HWSURFACE, 
				   iScrW << iScale, iScrH << iScale, 
				   DEFAULT_BPP, 
				   255, 255, 255, 0);
  
  	// Get some constants
  	iDepth = sdlWindow->format->BitsPerPixel;
  	iBpp = sdlWindow->format->BytesPerPixel;
  	iWinLineSize = (sdlWindow->pitch) >> (iBpp >> 1);
  	iLcdLineSize = (sdlLcdSrc->pitch) >> (iBpp >> 1);

  	// Draw the skin and compute grayscale palette
  	set_colors();
  	redraw_skin();
  
  	return 0;
}

/*
  	Release allocated ressources
*/
static int hid_quit_subsystem(void)
{
	if(sdlLcdSrc != NULL)
	{
  		SDL_FreeSurface(sdlLcdSrc);
  		sdlLcdSrc = NULL;
  	}

	if(sdlWindow != NULL)
	{
  		SDL_FreeSurface(sdlWindow);
  		sdlWindow = NULL;
  	}

  	free(pLcdBuf);

  	return 0;
}

static int hid_restart_subsystem(void)
{
	int ret1, ret2;
	
	ret1 = hid_quit_subsystem();
	ret2 = hid_init_subsystem();
	
	return ret1 | ret2;
}

int hid_init(void)
{
  	int i;
	gchar *icon;

  	// Init SDL
  	DISPLAY("Initializing Simple Directmedia Layer (SDL)... ");
  	if(SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
	  	gchar *s = g_strdup_printf("SDL: can't initialize: %s\n", SDL_GetError());
	  	tiemu_error(0, s);
	  	g_free(s);
	  	return -1;
    }
  	DISPLAY("Done.\n");

  	// Set application title
	icon = g_strconcat(inst_paths.pixmap_dir, "icon.xpm", NULL);
  	SDL_WM_SetCaption(sTitle, icon);
	g_free(icon);

  	// Init the SDL key -> TI key conversion table
  	for(i=0; i<256; i++)
    	sdl2ti[i] = sdl_to_ti(i);

  	for(i=0; i<256; i++)
    	sdl2ti[i+256] = sdl_to_ti(i+65288);

  	// Init the planar/chunky conversion table for LCD
  	compute_convtable();

  	// Do the rest
  	hid_init_subsystem();

  	return 0;
}

int hid_exit(void)
{
  	hid_quit_subsystem();
  	SDL_Quit();

  	return 0;
}


/***********************************/
/* Events management (kbd & mouse) */
/***********************************/

/* Converts a keyboard key (an SDL event) into a TI key */
static int sdl_to_ti(int key) 
{
  	if(tihw.calc_type & TI92)
    {
      	switch(key) 
		{
			// Alphabetical
			case SDLK_a : 	return TIKEY_A;
			case SDLK_b : 	return TIKEY_B;
			case SDLK_c : 	return TIKEY_C;
			case SDLK_d : 	return TIKEY_D;
			case SDLK_e : 	return TIKEY_E;
			case SDLK_f : 	return TIKEY_F;
			case SDLK_g : 	return TIKEY_G;
			case SDLK_h : 	return TIKEY_H;
			case SDLK_i : 	return TIKEY_I;
			case SDLK_j : 	return TIKEY_J;
			case SDLK_k : 	return TIKEY_K;
			case SDLK_l : 	return TIKEY_L;
			case SDLK_m : 	return TIKEY_M;
			case SDLK_n : 	return TIKEY_N;
			case SDLK_o : 	return TIKEY_O;
			case SDLK_p : 	return TIKEY_P;
			case SDLK_q : 	return TIKEY_Q;
			case SDLK_r : 	return TIKEY_R;
			case SDLK_s : 	return TIKEY_S;
			case SDLK_t : 	return TIKEY_T;
			case SDLK_u : 	return TIKEY_U;
			case SDLK_v : 	return TIKEY_V;
			case SDLK_w : 	return TIKEY_W;
			case SDLK_x : 	return TIKEY_X;
			case SDLK_y : 	return TIKEY_Y;
			case SDLK_z : 	return TIKEY_Z;
			
			// Numerical
			case SDLK_KP0 : return TIKEY_0;
			case SDLK_KP1 : return TIKEY_1;
			case SDLK_KP2 : return TIKEY_2;
			case SDLK_KP3 : return TIKEY_3;
			case SDLK_KP4 : return TIKEY_4;
			case SDLK_KP5 : return TIKEY_5;
			case SDLK_KP6 : return TIKEY_6;
			case SDLK_KP7 : return TIKEY_7;
			case SDLK_KP8 : return TIKEY_8;
			case SDLK_KP9 : return TIKEY_9;
			case SDLK_0 : 	return TIKEY_0;
			case SDLK_1 : 	return TIKEY_1;
			case SDLK_2 : 	return TIKEY_2;
			case SDLK_3 : 	return TIKEY_3;
			case SDLK_4 : 	return TIKEY_4;
			case SDLK_5 : 	return TIKEY_5;
			case SDLK_6 : 	return TIKEY_6;
			case SDLK_7 : 	return TIKEY_7;
			case SDLK_8 : 	return TIKEY_8;
			case SDLK_9 : 	return TIKEY_9;
			
			// Arrows
			case SDLK_UP : 		return TIKEY_UP;
			case SDLK_LEFT : 	return TIKEY_LEFT;
			case SDLK_RIGHT : 	return TIKEY_RIGHT;
			case SDLK_DOWN : 	return TIKEY_DOWN;
			
			// Functions
			case SDLK_F1 : return TIKEY_F1;
			case SDLK_F2 : return TIKEY_F2;
			case SDLK_F3 : return TIKEY_F3;
			case SDLK_F4 : return TIKEY_F4;
			case SDLK_F5 : return TIKEY_F5;
			case SDLK_F6 : return TIKEY_F6;
			case SDLK_F7 : return TIKEY_F7;
			case SDLK_F8 : return TIKEY_F8;
			
			// Standard
			case SDLK_RETURN :	return TIKEY_ENTER1;
			case SDLK_KP_ENTER: return TIKEY_ENTER2;
			case SDLK_LSHIFT : 	return TIKEY_SHIFT;
			case SDLK_RSHIFT : 	return TIKEY_SHIFT;
			case SDLK_RCTRL : 	return TIKEY_DIAMOND;
			case SDLK_LCTRL : 	return TIKEY_DIAMOND;
			case SDLK_LALT : 	return TIKEY_2ND;
			case SDLK_RALT : 	return TIKEY_2ND;
			case SDLK_SPACE : 		return TIKEY_SPACE;
			case SDLK_ESCAPE : 		return TIKEY_ESCAPE;
			case SDLK_BACKSPACE : 	return TIKEY_BACKSPACE;
			case SDLK_LEFTPAREN : 	return TIKEY_PALEFT;
			case SDLK_RIGHTPAREN : 	return TIKEY_PARIGHT;
			case SDLK_PERIOD : 		return TIKEY_PERIOD;
			case SDLK_COMMA : 		return TIKEY_COMMA;
			case SDLK_KP_PLUS : 	return TIKEY_PLUS;
			case SDLK_KP_MULTIPLY : return TIKEY_MULTIPLY;
			case SDLK_KP_DIVIDE : 	return TIKEY_DIVIDE;    
			case SDLK_SLASH : 		return TIKEY_DIVIDE;
			case SDLK_KP_MINUS : 	return TIKEY_MINUS;			
			case SDLK_MINUS : 		return TIKEY_NEGATE;
			case SDLK_EQUALS : 		return TIKEY_EQUALS;
			case SDLK_LESS : 		return TIKEY_NEGATE;			
			
			// Specific
			case SDLK_F9  : 		return TIKEY_APPS;
			case SDLK_SEMICOLON : 	return TIKEY_THETA;
			case SDLK_TAB : 		return TIKEY_STORE;
			case SDLK_CAPSLOCK : 	return TIKEY_HAND;
			case SDLK_PAGEDOWN : 	return TIKEY_MODE;
			case SDLK_BACKSLASH : 	return TIKEY_LN;			
			case SDLK_INSERT : 		return TIKEY_SIN;
			case SDLK_HOME : 		return TIKEY_COS;
			case SDLK_PAGEUP : 		return TIKEY_TAN;
			case SDLK_DELETE : 		return TIKEY_CLEAR;
			case SDLK_SCROLLOCK : 	return TIKEY_ON;			

			default : return TIKEY_NU;
		}
    }
  	else
    {
      	iAlpha = 0;
      	switch(key) 
		{
			// Alphabetical
			case SDLK_a : iAlpha = 1; 	return TIKEY_EQUALS;
			case SDLK_b : iAlpha = 1; 	return TIKEY_PALEFT;
			case SDLK_c : iAlpha = 1; 	return TIKEY_PARIGHT;
			case SDLK_d : iAlpha = 1; 	return TIKEY_COMMA;
			case SDLK_e : iAlpha = 1; 	return TIKEY_DIVIDE;
			case SDLK_f : iAlpha = 1; 	return TIKEY_F;
			case SDLK_g : iAlpha = 1; 	return TIKEY_7;
			case SDLK_h : iAlpha = 1; 	return TIKEY_8;
			case SDLK_i : iAlpha = 1; 	return TIKEY_9;
			case SDLK_j : iAlpha = 1; 	return TIKEY_MULTIPLY;
			case SDLK_k : iAlpha = 1; 	return TIKEY_EE;
			case SDLK_l : iAlpha = 1; 	return TIKEY_4;
			case SDLK_m : iAlpha = 1; 	return TIKEY_5;
			case SDLK_n : iAlpha = 1; 	return TIKEY_6;
			case SDLK_o : iAlpha = 1; 	return TIKEY_MINUS;
			case SDLK_p : iAlpha = 1; 	return TIKEY_STORE;
			case SDLK_q : iAlpha = 1; 	return TIKEY_1;
			case SDLK_r : iAlpha = 1; 	return TIKEY_2;
			case SDLK_s : iAlpha = 1; 	return TIKEY_3;
			case SDLK_t : 				return TIKEY_T;
			case SDLK_u : iAlpha = 1; 	return TIKEY_PLUS;
			case SDLK_v : iAlpha = 1; 	return TIKEY_0;
			case SDLK_w : iAlpha = 1; 	return TIKEY_PERIOD;
			case SDLK_x : 				return TIKEY_X;
			case SDLK_y : 				return TIKEY_Y;
			case SDLK_z : 				return TIKEY_Z;
			
			// Numerical
			case SDLK_KP0 : return TIKEY_0;
			case SDLK_KP1 : return TIKEY_1;
			case SDLK_KP2 : return TIKEY_2;
			case SDLK_KP3 : return TIKEY_3;
			case SDLK_KP4 : return TIKEY_4;
			case SDLK_KP5 : return TIKEY_5;
			case SDLK_KP6 : return TIKEY_6;
			case SDLK_KP7 : return TIKEY_7;
			case SDLK_KP8 : return TIKEY_8;
			case SDLK_KP9 : return TIKEY_9;
			case SDLK_0 : 	return TIKEY_0;
			case SDLK_1 : 	return TIKEY_1;
			case SDLK_2 : 	return TIKEY_2;
			case SDLK_3 : 	return TIKEY_3;
			case SDLK_4 : 	return TIKEY_4;
			case SDLK_5 : 	return TIKEY_5;
			case SDLK_6 : 	return TIKEY_6;
			case SDLK_7 : 	return TIKEY_7;
			case SDLK_8 : 	return TIKEY_8;
			case SDLK_9 : 	return TIKEY_9;
			
			// Arrows
			case SDLK_UP : 		return TIKEY_UP;
			case SDLK_LEFT : 	return TIKEY_LEFT;
			case SDLK_RIGHT : 	return TIKEY_RIGHT;
			case SDLK_DOWN : 	return TIKEY_DOWN;
			
			// Functions
			case SDLK_F1 : return TIKEY_F1;
			case SDLK_F2 : return TIKEY_F2;
			case SDLK_F3 : return TIKEY_F3;
			case SDLK_F4 : return TIKEY_F4;
			case SDLK_F5 : return TIKEY_F5;
			case SDLK_F7 : return TIKEY_F7;
			case SDLK_F8 : return TIKEY_F8;
			
			// Standard
			case SDLK_RETURN : 	return TIKEY_ENTER1;
			case SDLK_KP_ENTER: return TIKEY_ENTER2;
			case SDLK_LSHIFT : 	return TIKEY_SHIFT;
			case SDLK_RSHIFT : 	return TIKEY_SHIFT;
			case SDLK_RCTRL : 	return TIKEY_DIAMOND;
			case SDLK_LCTRL : 	return TIKEY_DIAMOND;
			case SDLK_LALT : 	return TIKEY_2ND;
			case SDLK_RALT : 	return TIKEY_2ND;

			case SDLK_SPACE : 		return TIKEY_SPACE;
			case SDLK_ESCAPE : 		return TIKEY_ESCAPE;
			case SDLK_BACKSPACE : 	return TIKEY_BACKSPACE;
			case SDLK_LEFTPAREN : 	return TIKEY_PALEFT;
			case SDLK_RIGHTPAREN : 	return TIKEY_PARIGHT;
			case SDLK_PERIOD : 		return TIKEY_PERIOD;
			case SDLK_COMMA : 		return TIKEY_COMMA;
			case SDLK_KP_PLUS : 	return TIKEY_PLUS;
			case SDLK_KP_MULTIPLY : return TIKEY_MULTIPLY;
			case SDLK_KP_DIVIDE : 	return TIKEY_DIVIDE;    
			case SDLK_KP_MINUS : 	return TIKEY_MINUS;
			case SDLK_MINUS : 		return TIKEY_NEGATE;
			case SDLK_BACKSLASH : 	return TIKEY_ON;
			case SDLK_SLASH : 		return TIKEY_DIVIDE;
			case SDLK_EQUALS : 		return TIKEY_EQUALS;
			case SDLK_LESS : 		return TIKEY_NEGATE;
			
			// Specific
			case SDLK_F6 : 			return TIKEY_CATALOG;
			case SDLK_F9  : 		return TIKEY_APPS;
			case SDLK_TAB : 		return TIKEY_STORE;
			case SDLK_CAPSLOCK : 	return TIKEY_ALPHA;			
			case SDLK_PAGEDOWN : 	return TIKEY_MODE;
			// missing key here
			case SDLK_PAGEUP : 		return TIKEY_EE;	
			case SDLK_DELETE : 		return TIKEY_CLEAR;
			case SDLK_SCROLLOCK : return TIKEY_ON;
		
			default : return TIKEY_NU;
		}
    }

    return 0;
}

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
void gui_popup_menu(void);

int hid_update_keys(void) 
{
  	SDL_Event event;
  	int i;

  	iKeyWasPressed = 0;
  
  	while(SDL_PollEvent(&event)) 
    {
      	if(event.type == SDL_MOUSEBUTTONDOWN) 
		{
	  		if(event.button.button==1) 
	    	{
	      		i = pos_to_key(event.button.x,event.button.y);
	      		if(i >= 0) 
				{
		  			if(event.button.button == 1) 
		    		{
		      			lastKey = i;
		      			ti68k_kbd_set_key(i,1);
		      			iKeyWasPressed = 1;
		    		}
				}
	    	}
			else if(event.button.button == 3)
	    	{
	      		if(!bFullscreen) 
				{
					// flush event
		      		SDL_WaitEvent(&event);
					// and popup menu
			  		gui_popup_menu();
				}
	      		else
					hid_switch_windowed();
	    	}
		}
      	else if(event.button.button == 3) 
		{
	  		printf("$*#!?\n");
		}
      	else if(event.type == SDL_MOUSEBUTTONUP) 
		{
	  		if(event.button.button==1) 
	    	{
	      		if(lastKey!=-1) 
				{
		  			ti68k_kbd_set_key(lastKey,0);
		  			lastKey = -1;
				}
	    	}
		}
      	else if(event.type == SDL_KEYDOWN) 
		{
	  		if((event.key.keysym.mod & KMOD_RALT || 
	      		event.key.keysym.mod & KMOD_LALT) && 
	     		(event.key.keysym.sym==SDLK_RETURN)) 
	    	{
	      		if (bFullscreen)
					hid_switch_windowed();
	      		else
					hid_switch_fullscreen();
	    	}
	  		else if(event.key.keysym.sym == SDLK_PRINT)
	    	{
	      		hid_screenshot(NULL);
	    	}
	  		else if(event.key.keysym.sym == SDLK_F10)
	    	{
				display_tifile_dbox();
	    	}
			else if(event.key.keysym.sym == SDLK_F11)
	    	{
                ti68k_debug_break();
	    	}
			else if(event.key.keysym.sym == SDLK_F12)
	    	{
	      		display_romversion_dbox();
	    	}
	  		else
	    	{
	      		iKeyWasPressed = 1;
	      		if(iAlpha)
					ti68k_kbd_set_key(TIKEY_ALPHA, 1);
	      		ti68k_kbd_set_key(sdl_to_ti(event.key.keysym.sym), 1);
	    	}
		}
      	else if(event.type == SDL_KEYUP) 
		{
	  		if(iAlpha)
	    	{
	      		ti68k_kbd_set_key(TIKEY_ALPHA, 0);
	      		iAlpha = 0;
	    	}
	  		ti68k_kbd_set_key(sdl_to_ti(event.key.keysym.sym), 0);
		}
      	else if(event.type == SDL_QUIT) 
		{
	  		//DISPLAY("SDL_QUIT\n");
	  		ti68k_exit();
			gtk_main_quit();

	  		exit(0);
		}
      	else if(event.type == SDL_VIDEORESIZE)
		{
	  		iScale = MAX(event.resize.w / skin_infos.width, 
						 event.resize.h / skin_infos.height);
	  		iScale--;
	  		if(iScale > 1) 
	    		iScale = 1;
	  		else if(iScale < 0)
	    		iScale = 0;
	  
	  		DISPLAY("SDL_VIDEORESIZE: w=%i h=%i -> iScale = %i\n", 
		  		event.resize.w, event.resize.h, iScale);
	  		hid_quit_subsystem();
	  		hid_init_subsystem();
		}
	  	else if(event.type == SDL_VIDEOEXPOSE)
	  	{
		  	redraw_skin();
	  	}
    }

  	return iKeyWasPressed;
}


/* 
   Converts the mouse position into a TIKEY_xx code
   Checks if the mouse cursor is within a rectangle defined in 
   the keyDefs array
*/
static int pos_to_key(int x, int y) 
{
  	int i;
  	RECT *rcRectKeys = skin_infos.keys_pos;
  
  	for(i = 0; i<80 ;i++)
    {
      	if((x >= rcRectKeys[i].left << iScale) && 
	 		(x < rcRectKeys[i].right << iScale) && 
	 		(y >= rcRectKeys[i].top << iScale) && 
	 		(y < rcRectKeys[i].bottom << iScale)) 
		
		return key_mapping[i];
    }
  
  	return -1;
}


/*********************/
/* Screen management */
/*********************/

/* The value v is between l and h and can not be outside */
#define filter(v, l, h) (v<l ? l : (v>h ? h : v))

/* Computes the 16 grays level colors and allocates a colormap */
static void set_colors(void)
{
  	int i;
  	int sr, sg, sb;
  	int er, eg, eb;
  	int r, g ,b;

  	sr = (whitePixel & 0xff0000) >> 8;
  	sg = (whitePixel & 0x00ff00);
  	sb = (whitePixel & 0x0000ff) << 8;
  	
  	er = (blackPixel & 0xff0000) >> 8;
  	eg = (blackPixel & 0x00ff00);
  	eb = (blackPixel & 0x0000ff) << 8;

  	if(iContrast < NGS) 
    {
      	sr = sr - (sr-er)*(NGS - iContrast)/13;
      	sg = sg - (sg-eg)*(NGS - iContrast)/13;
      	sb = sb - (sb-eb)*(NGS - iContrast)/13;
    }
  	else 
    {
      	er = er - (er-sr)*(iContrast - NGS)/13;
      	eg = eg - (eg-sg)*(iContrast - NGS)/13;
      	eb = eb - (eb-sb)*(iContrast - NGS)/13;
    }
  
  	r = sr;
  	g = sg;
  	b = sb;

  	if(iScrState == SCREEN_ON) 
    {
      	for(i = 0; i <= (iGrayPlanes+1); i++) 
		{
	  		sdlPal[i].r = filter(r, 0x0000, 0xA800) >> 8;
	  		sdlPal[i].g = filter(g, 0x0000, 0xB400) >> 8;
	  		sdlPal[i].b = filter(b, 0x3400, 0xA800) >> 8;
	 
	  		SDL_SetColors(sdlWindow, &(sdlPal[i]), i, 1);
	  		SDL_SetColors(sdlLcdSrc, &(sdlPal[i]), i, 1);

	  		r -= ((sr-er) / (iGrayPlanes+1));
	  		g -= ((sg-eg) / (iGrayPlanes+1));
	  		b -= ((sb-eb) / (iGrayPlanes+1));
		}
    }
}


static int hid_set_contrast(int c)
{
  	if(tihw.calc_type == TI89)
    	c = 31-c;

  	iNewContrast = (c+iLastContrast) / 2;
  	iLastContrast = c;
  
  	return 0;
}

/*
  Called when the user presses the ON or 2nd+ON key
*/
static void hid_lcd_on_off(int i) 
{
	if(i) 
	{
		iScrState = SCREEN_ON;
	} 
	else 
	{
		iScrState = SCREEN_OFF;
		redraw_skin(); 	// to clear LCD 
	}
}


/*
  Update the LCD screen by converting and blitting LCD memory
*/
int hid_update_lcd(void)
{
	SDL_Rect src_rect, dst_rect;
	int i, j, k;
	Uint8 *pLcdMem = tihw.lcd_ptr;
  
	if(iGrayPlanes != params.grayplanes) 
    {
		iGrayPlanes = params.grayplanes;
		set_colors();
    }

	if(iScrState == SCREEN_OFF)
		return 0;

	if(iContrast != iNewContrast) 
    {
		iContrast = iNewContrast;
		set_colors();
    }

	// Convert the bitmap screen to a bytemap screen */
	if(!iGrayPlanes || !iCurrPlane) 
    { 
		// no gray scale or init gray plane
		for(j=0, k=0; k<iScrH; k++)
		{
			for(i=0; i<iScrW>>3; i++, pLcdMem++) 
			{
				pLcdBuf[j++] = convtab[((*pLcdMem)<<1)  ];
				pLcdBuf[j++] = convtab[((*pLcdMem)<<1)+1];
			}
		}
    }
	else 
    { 
		// compute gray scale
		for(j=0, k=0; k<iScrH; k++)
		{
			for(i=0; i<iScrW>>3; i++, pLcdMem++) 
			{
				pLcdBuf[j++] += convtab[((*pLcdMem)<<1)  ];
				pLcdBuf[j++] += convtab[((*pLcdMem)<<1)+1];
			}
		}
    }

	// Display computed gray scale if required
	if(iCurrPlane++ >= iGrayPlanes) 
	{
		// Copy the LCD into 
		Uint8 *ptr = (Uint8 *)pLcdBuf;
		Uint8 *ptr8 = (Uint8 *)sdlLcdSrc->pixels;
      
		for(j = 0; j < iScrH; j++) 
		{
			for (i = 0; i < iScrW; i++) 
			{
				ptr8[(j*iLcdLineSize + i) << iScale] = *ptr;
				if(iScale != 0) 
				{
					ptr8[((j*iLcdLineSize + i) << iScale) + 1] = *ptr;
					memcpy(ptr8 + (j*iLcdLineSize << iScale) + iLcdLineSize, 
						ptr8 + (j*iLcdLineSize << iScale), iLcdLineSize);
				}
				ptr++;
			}
		}
      
		iCurrPlane = 0;
      
		// Convert surface from 8 bits depth to any depth
		sdlLcdDst = SDL_DisplayFormat(sdlLcdSrc);
		if(sdlLcdDst == NULL) 
		{
			SDL_ComplainAndExit();
		}
      
		// Copy surface into window
		src_rect.x = 0; 
		src_rect.y = 0; 
		src_rect.w = iLcdW; 
		src_rect.h = iLcdH;

		if(params.background) 
		{
			dst_rect.x = (short)(skin_infos.lcd_pos.left << iScale); 
			dst_rect.y = (short)(skin_infos.lcd_pos.top << iScale);
		}
		else 
		{
			dst_rect.x = 0;
			dst_rect.y = 0;
		}      

		if (SDL_BlitSurface(sdlLcdDst, &src_rect, sdlWindow, &dst_rect) < 0) 
		{
			SDL_FreeSurface(sdlLcdDst);
			SDL_ComplainAndExit();
		}
      
		SDL_FreeSurface(sdlLcdDst);
		SDL_UpdateRect(sdlWindow, dst_rect.x, dst_rect.y, iLcdW, iLcdH); 
	}

	return 0;
}

/* 
   Redraw the skin into window but don't reload skin file
*/
static void redraw_skin(void) 
{
  	SDL_Surface *sdlSkin;
  	SDL_Surface *sdlImage;
  	SDL_Rect rect;
  	int i, j;
  	Uint8 r, g, b;
  	Uint8 *ptr;

  	if(!params.background) 
    	return;
  
  	// Load the skin colormap
  	for(i=0; i<skin_infos.ncolors; i++)
    {
	      sdlPal[i+NGS].r = skin_infos.cmap[0][i];
	      sdlPal[i+NGS].g = skin_infos.cmap[1][i];
	      sdlPal[i+NGS].b = skin_infos.cmap[2][i];
    }

  	// and allocate colors
  	SDL_SetColors(sdlWindow, &(sdlPal[NGS]), NGS, skin_infos.ncolors);

  	// Fill the surface with the skin image
  	sdlSkin = SDL_CreateRGBSurface(SDL_HWSURFACE, iWinW, iWinH, DEFAULT_BPP, 
				   255, 255, 255, 255);
  	SDL_SetColors(sdlSkin, &(sdlPal[NGS]), NGS, skin_infos.ncolors);
 
  	ptr = (Uint8 *)sdlSkin->pixels;
  	for(j = 0; j < (int)skin_infos.height; j++)
    {
      	for (i = 0; i < (int)skin_infos.width; i++) 
		{
	  		r = skin_infos.cmap[0][skin_infos.img[j*skin_infos.width + i]];
	  		g = skin_infos.cmap[1][skin_infos.img[j*skin_infos.width + i]];
	  		b = skin_infos.cmap[2][skin_infos.img[j*skin_infos.width + i]];
	  		

			ptr[(j*iWinLineSize + i) << iScale] = NGS + skin_infos.img[j*skin_infos.width+i];
	  		if(iScale != 0) 
	  		{
	    		ptr[((j*iWinLineSize + i) << iScale) + 1] = NGS + skin_infos.img[j*skin_infos.width+i];
	    		memcpy(ptr + (j*iWinLineSize << iScale) + iWinLineSize, 
		   			ptr + (j*iWinLineSize << iScale), iWinLineSize);
	  		}
		}
    }

  	// Convert surface from 8 bits depth to any depth
  	sdlImage = SDL_DisplayFormat(sdlSkin);
  	if(sdlImage == NULL) 
  	{
    	SDL_ComplainAndExit();
  	}

  	// Copy surface into window
  	rect.x = 0; 
  	rect.y = 0;
  
  	if (SDL_BlitSurface(sdlSkin, NULL, sdlWindow, &rect) < 0) 
  	{
    	SDL_FreeSurface(sdlImage);
    	SDL_ComplainAndExit();
  	}
  
  	SDL_FreeSurface(sdlImage);
  	SDL_FreeSurface(sdlSkin);
  
  	SDL_UpdateRect(sdlWindow, 0, 0, 0, 0);
}	

/* Bitmap to bytemap conversion table */
static void compute_convtable(void) 
{
  	int i, j;
  	Uint8 k;
  	Uint8 *tmptab = (Uint8 *)convtab;

  	for(i=0, j=0; i<256; i++) 
    {
      	for(k = 1<<7; k; k>>=1)
		{
			tmptab[j++] = (i & k) ? 1 : 0;
		}
    }
}

void hid_set_callbacks(void)
{
    ti68k_gui_set_callbacks(
			       hid_init,
			       hid_exit,
			       hid_update_lcd,
			       hid_update_keys,
			       hid_lcd_on_off,
			       hid_set_contrast
			       );
}


/*
  Show skin (the skin is not reload from file)
*/
void hid_switch_with_skin(void)
{
  	SDL_FreeSurface(sdlWindow);
  
	sdlWindow = SDL_SetVideoMode(iWinW, iWinH, DEFAULT_BPP, DEFAULT_FLAGS);
  	if(sdlWindow == NULL) 
    {
		gchar *s = g_strdup_printf("SDL: can't set video mode: %s\n", SDL_GetError());
	  	tiemu_error(0, s);
	  	g_free(s);
		return;
    }
  	iWinLineSize = (sdlWindow->pitch) >> (iBpp >> 1);
  
  	params.background = 1;

  	redraw_skin();
}

/*
  Hide skin
*/
void hid_switch_without_skin(void)
{
  	SDL_FreeSurface(sdlWindow);
  
  	sdlWindow = SDL_SetVideoMode(iWinW, iWinH, DEFAULT_BPP, DEFAULT_FLAGS);
  	if(sdlWindow == NULL) 
    {
      	gchar *s = g_strdup_printf("SDL: can't set video mode: %s\n", SDL_GetError());
	  	tiemu_error(0, s);
	  	g_free(s);
		return;
    }
  	iWinLineSize = (sdlWindow->pitch) >> (iBpp >> 1);
  
  	params.background = 0;
}

/*
  Load a different skin (load skin from file)
*/
void hid_change_skin(const char *filename)
{
  	hid_restart_subsystem();
}

/*
  Show in fullscreen mode
*/
void hid_switch_fullscreen(void) 
{
  	SDL_FreeSurface(sdlWindow);
  
	sdlWindow = SDL_SetVideoMode(iWinW, iWinH, DEFAULT_BPP, DEFAULT_FLAGS | SDL_FULLSCREEN);
  	if(sdlWindow == NULL)
    {
		gchar *s = g_strdup_printf("SDL: can't switch to fullscreen: %s\n", SDL_GetError());
	  	tiemu_error(0, s);
	  	g_free(s);
      	
		if (!bWindowedFailed)
			hid_switch_windowed();
      	else
			exit(1);

      	bFullscreenFailed = 1;
  	}
  
  	bFullscreen = 1;
  	bFullscreenFailed = 0;

  	redraw_skin();
}

/*
  Show in windowed mode
*/
void hid_switch_windowed(void)
{
  	SDL_FreeSurface(sdlWindow);
  
	sdlWindow = SDL_SetVideoMode(iWinW, iWinH, DEFAULT_BPP, DEFAULT_FLAGS);
  	if(sdlWindow == NULL) 
    {
      	gchar *s = g_strdup_printf("SDL: can't switch from fullscreen: %s\n", SDL_GetError());
	  	tiemu_error(0, s);
	  	g_free(s);
      	
		if (!bFullscreenFailed)
			hid_switch_fullscreen();
      	else
			exit(1);

      	bFullscreenFailed = 1;
    }
  
  	bFullscreen = 0;
  	bWindowedFailed = 0;

  	redraw_skin();
}

/*
  Scale ratio 1:1
*/
void hid_switch_normal_view(void)
{
  	iScale = 0;
  	hid_restart_subsystem();
}

/*
  Scale ratio 2:1
 */
void hid_switch_large_view(void)
{
  	iScale = 1;
  	hid_restart_subsystem();
}

/*
  Do a screenshot of skin and/or LCD
*/

static void init_sdl_pixel_format(SDL_PixelFormat *fmt)
{
    // RGB 24 bits (note that R & B are swapped! Why?)
	fmt->palette = NULL;
	fmt->BitsPerPixel = 24;
	fmt->BytesPerPixel = 3;
	fmt->Rmask = 0x0000ff;
	fmt->Gmask = 0x00ff00;
	fmt->Bmask = 0xff0000;
	fmt->Amask = 0;
	fmt->Rshift = 0;
	fmt->Gshift = 8;
	fmt->Bshift = 16;
	fmt->Ashift = 0;
	fmt->Rloss = 0;
	fmt->Gloss = 0;
	fmt->Bloss = 0;
	fmt->Aloss = 0;
	fmt->colorkey = 0;
	fmt->alpha = 0;
}

int hid_screenshot(char *filename)
{
	gchar *outfile;
	gchar *ext = "???";
	gchar *type = "???";
	//int gap = iScrW - iLcdW;

	SDL_Surface *sdlCapture = NULL;
	SDL_PixelFormat fmt;
	Uint8 *pixels;
	GdkPixbuf *pixbuf = { 0 };
	gboolean result = FALSE;
	GError *error = NULL;

	if(filename == NULL) {
		switch(options2.format) {
			case IMG_JPG: ext = "jpg"; type = "jpeg"; break;
			case IMG_PNG: ext = "png"; type = "png";  break;
			case IMG_ICO: ext = "ico"; type = "ico";  break;
			default: type = "???"; break;
		}
      
		outfile = g_strdup_printf("%s%03i.%s", options2.file, options2.counter, ext);
	} else {
		outfile = g_strdup(filename);
	}

	DISPLAY("Screenshot to %s... ", outfile);

    init_sdl_pixel_format(&fmt);

	if((options2.size == IMG_LCD) && (options2.type == IMG_BW)) {
        Uint8 *ptr;
        int x, y;

        sdlCapture = SDL_ConvertSurface(sdlLcdSrc, &fmt, SDL_SWSURFACE);

        ptr = (Uint8 *)pLcdBuf;
        pixels = (Uint8 *)sdlCapture->pixels;

        for(y = 0; y < iScrH; y++) 
	    {
	        for (x = 0; x < iScrW; x++) 
	        {
                pixels[(y*sdlCapture->pitch + 3*x + 0) << iScale] = *ptr ? 0x00 : 0xff;
                pixels[(y*sdlCapture->pitch + 3*x + 1) << iScale] = *ptr ? 0x00 : 0xff;
                pixels[(y*sdlCapture->pitch + 3*x + 2) << iScale] = *ptr ? 0x00 : 0xff;

                ptr++;
            }
        }

	} else if((options2.size == IMG_LCD) && (options2.type == IMG_COL)) {
        sdlCapture = SDL_ConvertSurface(sdlLcdSrc, &fmt, SDL_SWSURFACE);

	} else if(options2.size == IMG_SKIN) {
		sdlCapture = SDL_ConvertSurface(sdlWindow, &fmt, SDL_SWSURFACE);
	}

        pixels = (Uint8 *)sdlCapture->pixels;

		pixbuf = gdk_pixbuf_new_from_data(pixels, GDK_COLORSPACE_RGB, FALSE,
				8, sdlCapture->w, sdlCapture->h, sdlCapture->pitch, NULL, NULL);

		//result = gdk_pixbuf_save(pixbuf, outfile, type, &error, "quality", "100", NULL);
		result = gdk_pixbuf_save(pixbuf, outfile, type, &error, NULL);
		if (result == FALSE) {
			DISPLAY("Failed to save pixbuf file: %s: %s\n", outfile, error->message);
			g_error_free(error);

		SDL_FreeSurface(sdlCapture);
	}

	DISPLAY("Done !\n");
	options2.counter++;
	g_free(filename);

	return 0;
}
