/*  GtkTiEmu - a TI emulator
 *  x11specific.c: Linux display specific part
 *  Tiger was originally written by Jonas Minsberg
 *  Copyright (C) 2000, Thomas Corvazier, Romain Lievin
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

/* 
   This source comes from the xspecific.c file of (x)tiger. I modified
   and adapted so that it can work with GTK. Moreover, I corrected some
   bugs with the management of the display in 24 colors mode.
*/

/*
  Uncomment it for displaying the right button menu in a second box instead of 
  the screen box
*/
//#define EXT_WIN

/* General */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

/* X11 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

/* 
   MIT Shared Memory Extension for X:
   Requires X11R6 minimum and the client/server must
   be on the SAME machine (no network !)
*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

/* GTK */
#include <gtk/gtk.h>
#include "interface.h"
#include "support.h"
#include "img_fmt.h"

/* Dependencies with the lib68k */
#include <tiemu/lib68k.h>
#include <tiemu/sysdeps.h>
#include <tiemu/keydef.h>
#include <tiemu/globinfo.h>

/* Misc */
#include "specific.h"
#include "skn_loader.h"
#include "callbacks.h"
#include "struct.h"


/***********************/
/* X related variables */
/***********************/

Display *Dsp;
Window Wnd;
Screen *Scr;
Colormap DefaultCMap;
XImage *pXLcdImg;
XImage *pXWinImg;
GC DefaultGC;
unsigned long White, Black;

int iDepth, iBpp;
int iScale = 1;

XShmSegmentInfo SHMInfo;
unsigned char *pXLcdBuf; // contains the X formatted image of the TI92 screen

unsigned long lcdPal[256]; // contains the greyscale palette
unsigned long winPal[256]; // idem


/*****************************************/
/* Various variables and short functions */
/*****************************************/

int iXWin, iYWin, iXLcd, iYLcd;
int iLineLength;
int iXMax = 240, iYMax = 128; // TI89 screen is a clipped TI92 screen

KeyArea keyAreaTab[MAX_KEY];
ULONG *pScreenBuf = NULL; // TI screen (bytemapped)
int iContrast = 16;
ULONG convtab[512]; /* Planar->Chunky conversion table */
UBYTE x2ti[512]; /* Converted keystates */
volatile int iKeyWasPressed;
int iLastContrast = 0;
int iNewContrast = 16;
int iGrayPlanes = -1;
int iCurrPlane = 0;
int iEmuState = 0;
int iLearnKeys = -1;
int iLastKey = -1;
char *sTitle = "GtkTiEmu by Tom & Roms (taken from Minnberg's XTiger)";

int gtk_main_window();

/*************/
/* Init/Exit */
/*************/

/* Bitmap to bytemap conversion table */
void compute_convtable(void) 
{
  int i,j;
  UBYTE k;
  UBYTE *tmptab = (UBYTE *)convtab;

  for(i=0, j=0; i<256; i++) 
    {
      for(k = 1<<7; k; k>>=1)
	{
	  if(i & k)
	    tmptab[j++] = 1;
	  else
	    tmptab[j++] = 0;
	}
    }
}

/*
  This is the first function to call.
  It initialize some constants, the TI keys, load the background 
  and so on...
 */
int alloc_background(void);
static int init_specific(void) 
{
  XEvent E;
  int i, j;
  
  /* Load the skin */
  if (getCalcType()&TI92)
    {
      loadSkin(SHARE_DIR "/skins/" "ti92.skn");
      iXLcd = 240; iYLcd = 128; iLineLength = 240;
    }  
  else
    {
      loadSkin(SHARE_DIR "/skins/" "ti89.skn");
      iXLcd = 160; iYLcd = 100; iLineLength = 240;
    }  

  if(glob_inf.background) 
    {
      iYWin = ySkin;
      iXWin = xSkin;
    }
  else 
    {
      iYWin = iYLcd;
      iXWin = iXLcd;
    }

  /* Init the planar/chunky conversion table */  
  compute_convtable();

  /* Allocate the TI screen buffer */
  pScreenBuf = malloc(iXMax * iYMax);

  /* Init the X key -> TI key conversion table */
  for(i=0; i<512; i++)
    x2ti[i] = 0;
  for(i=0; i<256; i++)
    x2ti[i] = x_to_ti(i);
  for(i=0; i<256; i++)
    x2ti[i+256] = x_to_ti(i+65288);

  /* Open display */
  Dsp=XOpenDisplay(NULL);
  if(!Dsp) return -1;

  Scr = DefaultScreenOfDisplay(Dsp);
  White = WhitePixelOfScreen(Scr);
  Black = BlackPixelOfScreen(Scr);
  DefaultGC = DefaultGCOfScreen(Scr);
  DefaultCMap = DefaultColormapOfScreen(Scr);
  iDepth = DefaultDepthOfScreen(Scr);
  iBpp = (iDepth == 24 ? 32 : iDepth);

  Wnd = XCreateSimpleWindow(Dsp, RootWindowOfScreen(Scr), 0, 0, 
			    iScale*iXWin, iScale*iYWin, 
			    10, Black, White);

  if(!Wnd) return(0);
  else
    {
      XSizeHints Hints;
      XWMHints WMHints;
      
      Hints.flags = PSize|PMinSize|PMaxSize;
      Hints.min_width = iXWin; 
      Hints.max_width = Hints.base_width = iScale * iXWin;
      Hints.min_height = iYWin;
      Hints.max_height = Hints.base_height = iScale * iYWin;
      WMHints.input = True;
      WMHints.flags = InputHint;
      XSetWMHints(Dsp, Wnd, &WMHints);
      XSetWMNormalHints(Dsp, Wnd, &Hints);
      XStoreName(Dsp, Wnd, sTitle);
    }
  
  XSelectInput(Dsp, Wnd, FocusChangeMask | ExposureMask | KeyPressMask | 
	       KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
  XMapRaised(Dsp, Wnd);
  XClearWindow(Dsp, Wnd);
  XWindowEvent(Dsp, Wnd, ExposureMask,&E);
  
  pXLcdImg = XShmCreateImage(Dsp, DefaultVisualOfScreen(Scr), iDepth,
			   ZPixmap, NULL, &SHMInfo, 
			   iScale*iXLcd, iScale*iYLcd);
  if(!pXLcdImg) return -1;
 
  SHMInfo.shmid = shmget(IPC_PRIVATE, pXLcdImg->bytes_per_line*pXLcdImg->height*iScale,
			 IPC_CREAT|0777);
  if(SHMInfo.shmid < 0) return -1;
    
  SHMInfo.shmaddr=shmat(SHMInfo.shmid,0,0);
  pXLcdImg->data = SHMInfo.shmaddr;
  pXLcdBuf = (unsigned char *)(pXLcdImg->data);
  if(!pXLcdBuf) return -1;
    
  SHMInfo.readOnly=False;
  if(!XShmAttach(Dsp,&SHMInfo)) return -1;

  /* Compute greyscale colormap */
  for(j=0; j<16; j++)
    {
      lcdPal[j] = 0;
    }
  set_colors();
  
  /* If background, display skin */
  if(glob_inf.background) 
    {
      alloc_background();
      redraw_skin();
    }

  gtk_main_window();
  
  return 0;
}


/* Quit the emulator and save the keyboard map */
static int exit_specific(void) 
{
  FILE *fp;
  int i;

  //DISPLAY("exit_specific\n"); destroy event does not call exit_specific -> bug !
  /* Free greyscale palette */
  for(i=0; i<16; i++)
    if(lcdPal[i] != 0) 
      {
	XFreeColors(Dsp,DefaultCMap,&lcdPal[i],1,0);
	lcdPal[i] = 0;
      }
  
  /* If skin, free */
  if(glob_inf.background) 
    {
      for(i = 0; i < skinNumberOfColors; i++)
	{
	  XFreeColors(Dsp,DefaultCMap,&winPal[i],1,0);     
	}
    }
  
  if(Dsp && Wnd) 
    {
      XShmDetach(Dsp,&SHMInfo);
      if(SHMInfo.shmaddr) shmdt(SHMInfo.shmaddr);
      if(SHMInfo.shmid>=0) shmctl(SHMInfo.shmid,IPC_RMID,0);
    }
  if(Dsp) 
    {
      XFreeGC(Dsp, DefaultGC);
      XCloseDisplay(Dsp); // segfault at this moment !
    }

  gtk_main_quit();

  if((fp = fopen("ti92.keys", "wb"))) 
    {
      fwrite(&x2ti, sizeof(x2ti[0]), 512, fp);
      fclose(fp);
    }

  return 0;
}


/***********************/
/* Keyboard management */
/***********************/

/* Converts a keyboard key (an X event) into a TI key */
int x_to_ti(int key) 
{
  switch(key) 
    {
      // TI keys
    case XK_a : return TIKEY_A;
    case XK_b : return TIKEY_B;
    case XK_c : return TIKEY_C;
    case XK_d : return TIKEY_D;
    case XK_e : return TIKEY_E;
    case XK_f : return TIKEY_F;
    case XK_g : return TIKEY_G;
    case XK_h : return TIKEY_H;
    case XK_i : return TIKEY_I;
    case XK_j : return TIKEY_J;
    case XK_k : return TIKEY_K;
    case XK_l : return TIKEY_L;
    case XK_m : return TIKEY_M;
    case XK_n : return TIKEY_N;
    case XK_o : return TIKEY_O;
    case XK_p : return TIKEY_P;
    case XK_q : return TIKEY_Q;
    case XK_r : return TIKEY_R;
    case XK_s : return TIKEY_S;
    case XK_t : return TIKEY_T;
    case XK_u : return TIKEY_U;
    case XK_v : return TIKEY_V;
    case XK_w : return TIKEY_W;
    case XK_x : return TIKEY_X;
    case XK_y : return TIKEY_Y;
    case XK_z : return TIKEY_Z;

    case XK_KP_0 : return TIKEY_0;
    case XK_KP_1 : return TIKEY_1;
    case XK_KP_2 : return TIKEY_2;
    case XK_KP_3 : return TIKEY_3;
    case XK_KP_4 : return TIKEY_4;
    case XK_KP_5 : return TIKEY_5;
    case XK_KP_6 : return TIKEY_6;
    case XK_KP_7 : return TIKEY_7;
    case XK_KP_8 : return TIKEY_8;
    case XK_KP_9 : return TIKEY_9;

    case XK_0 : return TIKEY_0;
    case XK_1 : return TIKEY_1;
    case XK_2 : return TIKEY_2;
    case XK_3 : return TIKEY_3;
    case XK_4 : return TIKEY_4;
    case XK_5 : return TIKEY_5;
    case XK_6 : return TIKEY_6;
    case XK_7 : return TIKEY_7;
    case XK_8 : return TIKEY_8;
    case XK_9 : return TIKEY_9;

    case XK_Up : return TIKEY_UP;
    case XK_Left : return TIKEY_LEFT;
    case XK_Right : return TIKEY_RIGHT;
    case XK_Down : return TIKEY_DOWN;
    case XK_F1 : return TIKEY_F1;
    case XK_F2 : return TIKEY_F2;
    case XK_F3 : return TIKEY_F3;
    case XK_F4 : return TIKEY_F4;
    case XK_F5 : return TIKEY_F5;
    case XK_F6 : return TIKEY_F6;
    case XK_F7 : return TIKEY_F7;
    case XK_F8 : return TIKEY_F8;
    case XK_Return : return TIKEY_ENTER1;
    case XK_KP_Enter : return TIKEY_ENTER2;
    case XK_Shift_L : return TIKEY_SHIFT;
    case XK_Shift_R : return TIKEY_SHIFT;
    case XK_Control_L : return TIKEY_DIAMOND;
    case XK_Control_R : return TIKEY_DIAMOND;
    case XK_Alt_L : return TIKEY_2ND;
    case XK_Alt_R : return TIKEY_2ND;
    case XK_Caps_Lock : return TIKEY_HAND;
    case XK_Tab : return TIKEY_STORE;
    case XK_space : return TIKEY_SPACE;
    case XK_Escape : return TIKEY_ESCAPE;
    case XK_BackSpace : return TIKEY_BACKSPACE;
      
    case XK_Aring :
    case XK_Udiaeresis: 
    case XK_udiaeresis: 
    case XK_bracketleft : return TIKEY_PALEFT;
      
    case XK_asciitilde: 
    case XK_asciicircum:
    case XK_bracketright : return TIKEY_PARIGHT;
    case XK_period : return TIKEY_PERIOD;
    case XK_comma : return TIKEY_COMMA;
    case XK_KP_Add : return TIKEY_PLUS;
    case XK_KP_Multiply : return TIKEY_MULTIPLY;
    case XK_KP_Divide : return TIKEY_DIVIDE;    
    case XK_KP_Subtract : return TIKEY_MINUS;
    case XK_minus : return TIKEY_NEGATE;
    case XK_backslash : return TIKEY_ON;
    case XK_quoteright : return TIKEY_MULTIPLY;
    case XK_slash : return TIKEY_DIVIDE;
    case XK_semicolon : return TIKEY_THETA;
    case XK_equal : return TIKEY_EQUALS;
    case XK_quoteleft : return TIKEY_POWER;
    case XK_less : return TIKEY_NEGATE;
    case XK_KP_Decimal : return TIKEY_PERIOD;
    case XK_Insert : return TIKEY_SIN;
    case XK_Delete : return TIKEY_CLEAR;
    case XK_Home : return TIKEY_COS;
    case XK_End : return TIKEY_LN;
    case XK_Page_Up : return TIKEY_TAN;
    case XK_Page_Down : return TIKEY_MODE;
    case XK_Scroll_Lock : return TIKEY_ON;

      // PC keys
    case XK_F9 : return OPT_DEBUGGER;
    case XK_F10 : return OPT_SCREENCAPTURE; //return OPT_LOADFILE;
      //case XK_F11 : return OPT_QUIT;  // F11 & F12 are used by WindowMaker
      //case XK_F12 : return OPT_QUIT 
    case XK_Print : return TIKEY_APPS;
    default : return TIKEY_NU;
    }
  
  return 0;
}

/* 
   Check if a keyboard key or a mouse button was pressed and updates
   the TI keyboard consequently
*/
/*static*/ int update_keys(void) 
{
  XEvent event;
  UWORD j;
  int i;

  iKeyWasPressed = 0;
   
  while(XCheckWindowEvent(Dsp, Wnd, ExposureMask | ButtonPressMask 
			  | ButtonReleaseMask | KeyPressMask 
			  | KeyReleaseMask, &event)) 
    {
      if(event.type == Expose && glob_inf.background) 
	{
	  /* Expose event: redraw ! */
	  redraw_skin();
	}
      else
	{
	  if(event.type == ButtonPress && glob_inf.background) 
	    {
	      /* Mouse button pressed: convert it into TI key */
	      /*
		printf("%d, %d\n", event.xbutton.x, event.xbutton.y);
	      */
	      i = pos_to_key(event.xbutton.x, event.xbutton.y);
	      if(i>=0) 
		{
		  if(event.xbutton.button == Button1) 
		    {
		      iKeyWasPressed = 1;
		      setActiveKey(i, 1);
		      iLastKey = i;
		    }
		  else 
		    {
		      if(event.xbutton.button == Button3) 
			{
			  XStoreName(Dsp, Wnd, "Hit key to bind...");
			  iLearnKeys = i;
			}
		    }
		}
	    }
	  else
	    {
	      if(event.type == ButtonRelease) 
		{
		  /* Mouse button released */
		  if(iLastKey != -1)
		    {
		      setActiveKey(iLastKey, 0);
		      iLastKey = -1;
		    }
		  /* Right button menu */
#ifndef EXT_WIN
		  if(event.xbutton.button == 3)
		    {
		      XNextEvent(Dsp, &event); // flush event !!!
		      gtk_menu_popup(GTK_MENU(create_popup_menu()),
				     NULL, NULL, NULL, NULL,
				     event.xbutton.button,
				     event.xbutton.time);
		      suspend();
		    }
#endif
		}
	      else 
		{
		  if(event.type == KeyPress || event.type == KeyRelease) 
		    {
		      /* Keyboard button typed */
		      j = XLookupKeysym((XKeyEvent *)&event, 0);
		      if(j >= 65288)
			{
			  j-=(65288-256);
			}
		      
		      //printf("Got keycode %d = %d\n", j, x2ti[j]);
		      
		      if(iLearnKeys >= 0) 
			{
			  XStoreName(Dsp, Wnd, sTitle);
			  if(event.type == KeyPress)
			    {
			      x2ti[j] = iLearnKeys;
			    }
			  iLearnKeys = -1;
			}
		      else 
			{
			  setActiveKey(x2ti[j], 
				       (event.type == KeyPress ? 1 : 0));
			  if(event.type == KeyPress)
			    {
			      iKeyWasPressed = 1;
			    }
			}
		    }
		}
	    }
	}
    }
    
  if(iKeyWasPressed) 
    {
      if(isKeyPressed(OPT_SCREENCAPTURE))
	{
	  setActiveKey(OPT_SCREENCAPTURE, 0);
	  do_screenshot(options.img_format, options.img_type, options.img_size, NULL);
	  unsuspend();
	}
      else
	if(isKeyPressed(OPT_DEBUGGER))
	  {
	    setActiveKey(OPT_DEBUGGER, 0);
	    launchDebugger();
	  }
	else
	  {
	    /*
	    if(keyStates[OPT_QUIT])
	      { 
		specialflags |= SPCFLAG_BRK;
	      }
	    */
	  }
    }
  
  return iKeyWasPressed;
}


/* 
   Converts the mouse position into a TIKEY_xx code
   Checks if the mouse cursor is within a rectangle defined in the keyDefs
   array
*/
int pos_to_key(int x, int y) 
{
  int i;
  
  for(i = 0; i<80 ;i++)
    {
      if ( (x >= rcRectKeys[i].left) && (x < rcRectKeys[i].right) &&
	   (y >= rcRectKeys[i].top) && (y < rcRectKeys[i].bottom) ) 
	return sknKey[i];
    }
  
  return -1;
}


/*********************/
/* Screen management */
/*********************/

/* The value v is between l and h and can not be outside */
#define filter(v, l, h) (v<l ? l : (v>h ? h : v))

/* Computes the 16 grays level colors and allocates a colormap */
void set_colors(void)
{
  XColor color;

  int i;
  int sr = 0xA800, sg = 0xB400, sb = 0xA800;
  int er = 0x0000, eg = 0x0000, eb = 0x3400;
  int r,g,b;

  color.flags = DoRed | DoGreen | DoBlue;

  if(iContrast < 16) 
    {
      sr = sr - (sr-er)*(16 - iContrast)/13;
      sg = sg - (sg-eg)*(16 - iContrast)/13;
      sb = sb - (sb-eb)*(16 - iContrast)/13;
    }
  else 
    {
      er = er - (er-sr)*(iContrast - 16)/13;
      eg = eg - (eg-sg)*(iContrast - 16)/13;
      eb = eb - (eb-sb)*(iContrast - 16)/13;
    }
  
  r = sr, g = sg, b = sb;
  
  for(i=0; i<16; i++)
    if(lcdPal[i] != 0) 
      {
	XFreeColors(Dsp,DefaultCMap,&lcdPal[i],1,0);
	lcdPal[i] = 0;
      }
  
  if(iEmuState & ESTAT_SCREENON) 
    {
      for(i=0; i<=iGrayPlanes+1; i++) 
	{
	  color.red   = filter(r, 0x0000, 0xA800);
	  color.green = filter(g, 0x0000, 0xB400);
	  color.blue  = filter(b, 0x3400, 0xA800);

	  lcdPal[i] = XAllocColor(Dsp,DefaultCMap,&color) ? color.pixel : 
	    (i&2 ? White : Black);

	  r-=((sr-er)/(iGrayPlanes+1));
	  g-=((sg-eg)/(iGrayPlanes+1));
	  b-=((sb-eb)/(iGrayPlanes+1));
	}
    }
  //exit(0);
}

/* Set the contrast */
static int set_contrast(int c)
{
  iNewContrast = (c+iLastContrast) / 2;
  iLastContrast = c;

  return 0;
}

/* Screen ON of course */
static int screen_on(void) 
{
  iEmuState |= ESTAT_SCREENON;
  return 0;
}

/* Screen OFF */
static int screen_off(void) 
{
  iEmuState &= ~ESTAT_SCREENON;
  return 0;
}

/* Set screen pointer */
/*
void set_screen_ptr(UBYTE *ptr) 
{
  pLcdBuf = ptr;
}
*/

void update_window(void) {}

/* 
   Update the screen starting at the TI screen buffer by filling the
   Xshm segment
*/
static int update_screen(void) 
{
  UBYTE *ptr = getLcdPtr();
  UBYTE *ptr8;
  UWORD *ptr16;
  ULONG *ptr32;
  int i, j, k;
 
  if(iGrayPlanes != glob_inf.grayPlanes) 
    {
      iGrayPlanes = glob_inf.grayPlanes;
      set_colors();
    }

  if(!(iEmuState & ESTAT_SCREENON)) return 0;

  if(iContrast != iNewContrast) 
    {
      iContrast = iNewContrast;
      set_colors();
    }

  // Convert the bitmap screen to a bytemap screen */
  if(!iGrayPlanes || !iCurrPlane) 
    { // no gray scale or init gray plane
      for(j=0, k=0; k<iYLcd; k++)
	for(i=0; i<iXMax>>3; i++, ptr++) 
	  {
	    pScreenBuf[j++] = convtab[((*ptr)<<1)  ];
	    pScreenBuf[j++] = convtab[((*ptr)<<1)+1];
	  }
    }
  else 
    { // compute gray scale
      for(j=0, k=0; k<iYLcd; k++)
	for(i=0; i<iXMax>>3; i++, ptr++) 
	  {
	    pScreenBuf[j++] += convtab[((*ptr)<<1)  ];
	    pScreenBuf[j++] += convtab[((*ptr)<<1)+1];
	  }
    }

  // Display computed gray scale if required
  if(iCurrPlane++ >= iGrayPlanes) 
    {
      int add2line = iLineLength - iXLcd;

      switch(iDepth) 
	{
	case 8:
	  ptr = (UBYTE *)pScreenBuf - add2line;
	  ptr8 = (UBYTE *)pXLcdBuf;
	  for(j=0; j<iYLcd; j++) 
	    {
	      for (i=0;i<iXLcd;i++) 
		{
		  *ptr8++ = lcdPal[*ptr++];
		}
	      ptr += add2line;
	    }
	  break;
	case 16:
	  ptr = (UBYTE *)pScreenBuf;
	  ptr16 = (UWORD *)pXLcdBuf;
	  for(j=0; j<iYLcd; j++) 
	    {
	      for (i=0;i<iXLcd;i++) 
		{
		  *ptr16++ = lcdPal[*ptr++];
		}
	      ptr += add2line;
	    }
	  break;
	case 24:
	case 32:
	  ptr = (UBYTE *)pScreenBuf;
	  ptr32 = (ULONG *)pXLcdBuf;
	  for(j=0; j<iYLcd; j++) 
	    {
	      for (i=0;i<iXLcd;i++) 
		{
		  *ptr32++ = lcdPal[*ptr++];
		}
	      ptr += add2line;
	    }
	  break;
	}
      
      iCurrPlane = 0;
      
      if(glob_inf.background)
	XShmPutImage(Dsp, Wnd, DefaultGC, pXLcdImg, 0, 0,
		     iScale*rcLcd.left, iScale*rcLcd.top, 
		     iScale*iXLcd, iScale*iYLcd, False);
      else
	XShmPutImage(Dsp, Wnd, DefaultGC, pXLcdImg, 0, 0,
		     iScale*0, iScale*0, 
		     iScale*iXLcd, iScale*iYLcd, False);
      XFlush(Dsp);    
    }

  return 0;
}


/* Create and allocate the background image starting at the pixmap */
int alloc_background(void)
{
   int i,j;
   XColor color;
   unsigned long l;   
   char *pWinImg = NULL;
   color.flags = DoRed | DoGreen | DoBlue;

   /* Load the skin colormap and allocate colors */
   for(i=0; i<skinNumberOfColors; i++)
     {
       color.red = 0xff00 & (skinColormap[0][i] << 8);
       color.green = 0xff00 & (skinColormap[1][i] << 8);
       color.blue = 0xff00 & (skinColormap[2][i] << 8);
       
       // Allocate if 8 planes color mode
       winPal[i] = XAllocColor(Dsp, DefaultCMap, &color) ?
         color.pixel : Black;
       if(winPal[i] == Black)
	 {
	   fprintf(stderr, "Unable to allocate a color in the X colormap.\n");
	   fprintf(stderr, "Program aborted.\n");
	   exit(-1);
	 }
     }

   // Allocate the background image buffer
   pWinImg = malloc(iYWin * iXWin * (iBpp/8));

   // Fill the buffer with the pixmap
   for(i=0; i<ySkin; i++)
     {
       for(j=0; j<xSkin; j++) 
	 {
	   l = winPal[((unsigned char *)pSkin)[i*iXWin+j]];
	   switch(iDepth) 
	     {
	     case 8 :
	       ((UBYTE*)pWinImg)[(i*iXWin+j)] = l;
	       break;
	     case 16 :
	       ((UWORD*)pWinImg)[i*iXWin+j] = l;
	       break;
	     case 24 :
	     case 32 :
	       ((ULONG*)pWinImg)[i*iXWin+j] = l;
	       break;
	     }
	 }
     }

   // Free the skin buffer since no longer used
   free(pSkin);

   // Create the background image
   pXWinImg = XCreateImage(Dsp, DefaultVisualOfScreen(Scr), iDepth, 
			  ZPixmap, 0, pWinImg, 
			  iScale * iXWin, iScale * iYWin, iBpp, 
			  iScale * iXWin*iBpp/8);
   if(!pXWinImg)
     {
       fprintf(stderr, "Unable to create the background image.\n");
       exit(-1);
     }
   return 0;
}

/* Draw the background image (skin) into the window */
void redraw_skin(void) 
{
  if(pXWinImg) 
    {
      XPutImage(Dsp, Wnd, DefaultGC, pXWinImg, 
		0, 0, 0, 0, 
		iScale * iXWin, iScale * iYWin);
      XFlush(Dsp);
    }
}

/* Display or not display the skin */
void toggle_skin(void)
{
  fprintf(stderr, "toggle_skin\n");
  //glob_inf.background = !(glob_inf.background);
  redraw_skin();
  
}

void switch_without_skin()
{

}

void switch_with_skin(void)
{

}

/****************/
/* GTK routines */
/****************/

GtkWidget *window;

/*
  A GTK callback: displays a popup menu from the second window
 */
gboolean
button_press_event        (GtkWidget       *widget,
                           GdkEventButton  *event,
                           gpointer         user_data)
{
  /* Displays the popup menu */
  if(event->button == 3)
    {
      gtk_menu_popup(GTK_MENU(create_popup_menu()), 
		     NULL, NULL, NULL, NULL, 
		     event->button, event->time);
      suspend();
    }
  
  return FALSE;
}

/* The GTK window */
int gtk_main_window()
{
  GtkWidget *eventbox;
  GtkWidget *label;

  gtk_init (NULL, NULL);
    
  /* The main window */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "GtkTiEmu");
  gtk_widget_set_usize(window, 250, 50);

  /* The event box */
  eventbox = gtk_event_box_new ();
  gtk_container_add (GTK_CONTAINER (window), eventbox);
  GTK_WIDGET_SET_FLAGS (eventbox, GTK_CAN_FOCUS);
  GTK_WIDGET_SET_FLAGS (eventbox, GTK_CAN_DEFAULT);
  gtk_widget_set_events (eventbox, GDK_BUTTON_PRESS_MASK | GDK_KEY_PRESS_MASK);
  gtk_widget_show (eventbox);

  label = gtk_label_new ("Click here to display a menu");
  gtk_container_add (GTK_CONTAINER (eventbox), label);
  gtk_widget_show (label);
  
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
                      GTK_SIGNAL_FUNC (gtk_main_quit),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (eventbox), "button_press_event",
                      GTK_SIGNAL_FUNC (button_press_event),
                      NULL);
  gtk_widget_grab_focus (eventbox);
  gtk_widget_grab_default (eventbox);

#ifdef EXT_WIN  
  gtk_widget_show_all(window);
#endif

  return 0;
}

/*
  Allow faster display by suspending the emulator core until
  the user has finished with the popup menu
*/
static int suspended = 0;

int is_suspended()
{
  return suspended;
}

void suspend()
{
  suspended = 1;
}

void unsuspend()
{
  suspended = 0;
}

/*
  Update the GUI (GTK main loop)
*/
int update_gui()
{
  //update_screen();
  gtk_main_iteration_do(FALSE);

  return suspended;
}

int set_gui_callbacks(void)
{
  return setGuiCallbacks(init_specific,
			 exit_specific,
			 update_screen,
			 update_keys,
			 screen_on,
			 screen_off,
			 NULL,
			 set_contrast);
}


int do_screenshot(int format, int type, int size, char *filename)
{
  Image img;
  char outfile[MAXCHARS];
  char *ext = "???";
  FILE *fp;
  int row, col;
  UBYTE *ptr8;
  UWORD *ptr16;
  ULONG *ptr32;
  unsigned long l; // a pixel
  int add2line = iLineLength - iXLcd;
  int i, j;

  if(filename == NULL)
    {
      switch(format)
	{
	case IMG_XPM: ext = "xpm"; break;
	case IMG_PCX: ext = "pcx"; break;
	case IMG_JPG: ext = "jpg"; break;
	case IMG_BMP: ext = "bmp"; break;
	default: break;
	}
      sprintf(outfile, "%s%03i.%s", options.screen_file, 
	      options.screen_counter, ext);
    }
  else
    strcpy(outfile, filename);
  
  fp = fopen(outfile, "wb");
  if(fp == NULL)
    exit(-1);
  
  fprintf(stdout, "Screenshot to <%s>... ", outfile);
  //  
  // Write the TI screen in black & white
  //  
  if((type == IMG_BW) && (size == IMG_LCD))
    {
      img.height = iYLcd;
      img.width = iXLcd;
      img.inverted = !0;
      img.depth = 2;
      alloc_bitmap(&img);

      if(getCalcType() & TI92)
	{
	  memcpy(img.bitmap, getLcdPtr(), 
		 ((img.width * img.height) >> 3) * sizeof(byte));
	}
      else
	{
	  UBYTE *pLcd = getLcdPtr();
	  for(j=0; j<iYLcd; j++) 
	    {
	      for (i=0;i<(iXLcd>>3);i++) 
		{
		  (img.bitmap)[j*(iXLcd>>3)+i] = *pLcd++;
		}
	      pLcd += add2line>>3;
	    }
	}
    }
  //
  // Write only the TI screen in color
  //
  if((type == IMG_COL) && (size == IMG_LCD))
    {
      XImage *xi = XGetImage(Dsp, Wnd, rcLcd.left, rcLcd.top, 
			     iXLcd, iYLcd, 
			     0xffffff, ZPixmap); //0xffffff = rrggbb mask
      
      img.height = iYLcd;
      img.width = iXLcd;
      img.depth = 255;

      switch(iDepth) 
	{
	case 8: // rrggbb is 8 bits + palette
	  ptr8 = (UBYTE *)(xi->data);
	  alloc_bytemap(&img);
	  // backPal[256] & lcdPal[256] ??
	  break;
	case 16: // rrggbb is 5/6/5 bits
	  //ptr16 = (UWORD *)(xi->data);
	  ptr16 = (UWORD *)(pXLcdImg->data);
	  alloc_rgbmap(&img);
	  for(row=0; row<iYLcd; row++)
	    {
	      for(col=0; col<iXLcd; col++)
		{	
		  // 16 planes only      
		  l = ptr16[row*iXLcd+col];
		  (img.rgbmap)[3*(row*iXLcd+col)+0] = ((l & 0xf800) >> 11) << 3;
		  (img.rgbmap)[3*(row*iXLcd+col)+1] = ((l & 0x7e0) >> 5) << 2;
		  (img.rgbmap)[3*(row*iXLcd+col)+2] = ((l & 0x1f)) << 3;
		}
	    }
	  convert_rgbmap_to_bytemap(&img);
	  break;
	case 24:
	case 32: // rrggbb is 8/8/8 bits
	  ptr32 = (ULONG *)(xi->data);
	  alloc_rgbmap(&img);
	  for(row=0; row<iYLcd; row++)
	    {
	      for(col=0; col<iXLcd; col++)
		{
		  l = ptr32[row*iXLcd+col];
		  (img.rgbmap)[3*(row*iXLcd+col)+0] = ((l & 0xff0000) >> 16);
		  (img.rgbmap)[3*(row*iXLcd+col)+1] = ((l & 0x00ff00) >>  8);
		  (img.rgbmap)[3*(row*iXLcd+col)+2] = ((l & 0x0000ff) >>  0);
		}
	    }
	  convert_rgbmap_to_bytemap(&img);
	  break;
	}
    }
  //
  // Write the entire screen in color
  //  
  if(size == IMG_SKIN)
    {
      XImage *xi = XGetImage(Dsp, Wnd, 0, 0, iXWin, iYWin, 
			     0xffffff, ZPixmap); //0xffffff = rrggbb mask
      img.height = iYWin;
      img.width = iXWin;
      img.depth = 255;

      switch(iDepth) 
	{
	case 8: // rrggbb is 8 bits + palette
	  ptr8 = (UBYTE *)(xi->data);
	  alloc_bytemap(&img);
	  break;
	case 16: // rrggbb is 5/6/5 bits
	  ptr16 = (UWORD *)(xi->data);
	  alloc_rgbmap(&img);
	  for(row=0; row<iYWin; row++)
	    {
	      for(col=0; col<iXWin; col++)
		{
		  l = ptr16[row*iXWin+col];
		  (img.rgbmap)[3*(row*iXWin+col)+0] = ((l & 0xf800) >> 11) << 3;
		  (img.rgbmap)[3*(row*iXWin+col)+1] = ((l & 0x7e0) >> 5) << 2;
		  (img.rgbmap)[3*(row*iXWin+col)+2] = ((l & 0x1f)) << 3;
		}
	    }
	  convert_rgbmap_to_bytemap(&img);
	  break;
	case 24:
	case 32: // rrggbb is 8/8/8 bits
	  ptr32 = (ULONG *)(xi->data);
	  alloc_rgbmap(&img);
	  for(row=0; row<iYWin; row++)
	    {
	      for(col=0; col<iXWin; col++)
		{
		  l = ptr32[row*iXWin+col];
		  (img.rgbmap)[3*(row*iXWin+col)+0] = ((l & 0xff0000) >> 16);
		  (img.rgbmap)[3*(row*iXWin+col)+1] = ((l & 0x00ff00) >>  8);
		  (img.rgbmap)[3*(row*iXWin+col)+2] = ((l & 0x0000ff) >>  0);
		}
	    }
	  convert_rgbmap_to_bytemap(&img);
	  break;
	}
    }

  switch(format)
    { 
    case IMG_XPM: write_xpm_format(fp, &img); break;
    case IMG_PCX: write_pcx_format(fp, &img); break;
    case IMG_JPG: write_jpg_format(fp, &img); break;
    case IMG_BMP: fprintf(stderr, "Only available in the SDL version !\n");
    default: fprintf(stdout, "Invalid image format\n"); break;
    }

  fclose(fp);
  fprintf(stdout, "Done !\n");
  options.screen_counter++;

  return 0;
}


/* Cut&paste block */
/*
if((type == IMG_COL) && (size == IMG_LCD))
    {
#ifdef
      img.height = iYLcd;
      img.width = iXLcd;
      img.depth = 255;
      alloc_colormap(&img);
      for(k=0; k<256; k++)
	{
	  (img.colormap)[3*k+0] = lcdPal[k] & 0xf800;
	  (img.colormap)[3*k+1] = lcdPal[k] & 0x7e0;
	  (img.colormap)[3*k+2] = lcdPal[k] & 0x1f;
	}  
      ptr8 = (UBYTE *)pScreenBuf;
      alloc_bytemap(&img);
      for(row=0; row<iYLcd; row++)
	{
	  for(col=0; col<iXLcd; col++)
	    {
	      (img.bytemap)[row*iXLcd+col] = ptr[row*iXLcd+col];
	    }
	}
      // test purpose
#else
      alloc_colormap(&img);
      for(k=0; k<256; k++)
	{
	  (img.colormap)[3*k+0] = k - k % 32;
	  (img.colormap)[3*k+1] = (k / 32) * 4 + k - k % 32;
	  (img.colormap)[3*k+2] = k - k % 64;
	}
      alloc_bytemap(&img);
      for(k=0; k<img.width * img.height; k++) 
	img.bytemap[k] = k % (img.depth);
#endif
    }
*/

