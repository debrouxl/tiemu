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
#endif				/*  */

#include <gtk/gtk.h>
#include <glade/glade.h>

#include "intl.h"
#include "paths.h"
#include "skinops.h"
#include "support.h"
#include "ti68k_int.h"
#include "struct.h"
#include "tie_error.h"
#include "hid.h"

/* Types */

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

} RGBA;

#define BPP 8               // 8 bits per pixel
#define NGS 16              // Number of gray scales

#define SCREEN_ON   1
#define SCREEN_OFF	0

/* Variables */

extern GtkWidget *wnd;
extern GtkWidget *area;

GdkPixbuf *lcd = NULL;
GdkPixbuf *skn = NULL;
GdkPixmap *pixmap = NULL;

uint32_t convtab[512];      // Planar to chunky conversion table
RGBA     grayscales[16];    // Gray scales rgb values

int iContrast = NGS;          // current contrast level
int iLastContrast = 0;        // previous contrast level
int iNewContrast = NGS;       // new contrast level
int iGrayPlanes = -1;         // number of grayscales to emulate
int iCurrPlane = 0;           // ?
int iScrState = 0;            // screen state

int iScrW, iScrH;
uint32_t *pLcdBuf;
uint32_t convtab[];

void compute_convtable(void) 
{
  	int i, j;
  	uint8_t k;
  	uint8_t *tab = (uint8_t *)convtab;

  	for(i=0, j=0; i<256; i++) 
    {
      	for(k = 1<<7; k; k>>=1)
		{
			tab[j++] = (i & k) ? 1 : 0;
		}
    }
}

/* The value v is between l and h and can not be outside */
#define filter(v, l, h) (v<l ? l : (v>h ? h : v))

/* Computes the 16 grays level colors and allocates a colormap */
void set_colors(void)
{
  	int i;
  	int sr, sg, sb;
  	int er, eg, eb;
  	int r, g ,b;
    uint32_t white = skin_infos.lcd_white;
    uint32_t black = skin_infos.lcd_black;

  	sr = (white & 0xff0000) >> 8;
  	sg = (white & 0x00ff00);
  	sb = (white & 0x0000ff) << 8;
  	
  	er = (black & 0xff0000) >> 8;
  	eg = (black & 0x00ff00);
  	eb = (black & 0x0000ff) << 8;

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
	  		grayscales[i].r = filter(r, 0x0000, 0xA800) >> 8;
	  		grayscales[i].g = filter(g, 0x0000, 0xB400) >> 8;
	  		grayscales[i].b = filter(b, 0x3400, 0xA800) >> 8;

	  		r -= ((sr-er) / (iGrayPlanes+1));
	  		g -= ((sg-eg) / (iGrayPlanes+1));
	  		b -= ((sb-eb) / (iGrayPlanes+1));
		}
    }
}

/* 
   Redraw the skin into window but don't reload skin file
*/
void redraw_skin(void) 
{
    GdkRect rect;

  	if(!params.background) 
    	return;
  
	if(skn == NULL)
	    return;

    gdk_draw_pixbuf(pixmap, wnd->style->fg_gc[GTK_WIDGET_STATE(wnd)],
		  skn, 0, 0, 0, 0, -1, -1,
		  GDK_RGB_DITHER_NONE, 0, 0);
  
    rect.x = 0;
    rect.y = 0;
    rect.w = skin_infos.width;
    rect.h = skin_infos.height;
    gtk_widget_draw(area, (GdkRectangle *)&rect);
}

int hid_update_lcd(void)
{
	int i, j, k;
	uint8_t *pLcdMem = tihw.lcd_ptr;
	uint8_t *ptr = (uint8_t *)pLcdBuf;
	GdkRect src, dst;

	int width, height, rowstride, n_channels;
	guchar *pixels, *p;

    iScrW = tihw.lcd_w;
    iScrH = tihw.lcd_h;

	n_channels = gdk_pixbuf_get_n_channels (lcd);
	width = gdk_pixbuf_get_width (lcd);
	height = gdk_pixbuf_get_height (lcd);
	rowstride = gdk_pixbuf_get_rowstride (lcd);
	pixels = gdk_pixbuf_get_pixels (lcd);

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

    if(iCurrPlane++ >= iGrayPlanes) 
	{
		// Copy the LCD into 
		for(j = 0; j < iScrH; j++) 
		{
			for (i = 0; i < iScrW; i++, ptr++) 
			{
				p = pixels + j * rowstride + i * n_channels;

				p[0] = grayscales[*ptr].r;
				p[1] = grayscales[*ptr].g;
				p[2] = grayscales[*ptr].b;
				p[3] = 0;
			}
		}

        iCurrPlane = 0;

        // Copy surface into window
        src.x = 0;
        src.y = 0;
		src.w = tihw.log_w > 240 ? 240 : tihw.log_w;
		src.h = tihw.log_h > 128 ? 128 : tihw.log_h;

        if(params.background) 
		{
			dst.x = skin_infos.lcd_pos.left; 
			dst.y = skin_infos.lcd_pos.top;
		}
		else 
		{
			dst.x = 0;
			dst.y = 0;
		}  
        dst.w = width;
        dst.h = height;

        gdk_draw_pixbuf(pixmap, wnd->style->fg_gc[GTK_WIDGET_STATE(wnd)],
		  lcd, src.x, src.y, dst.x, dst.y, -1, -1,
		  GDK_RGB_DITHER_NONE, 0, 0);

		gtk_widget_draw(area, (GdkRectangle *)&dst);
    }

    return -1;
}

void hid_lcd_on_off(int i) 
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

int hid_set_contrast(int c)
{
    if((tihw.calc_type == TI89) || (tihw.calc_type == TI89t))
    	c = 31-c;

  	iNewContrast = (c+iLastContrast) / 2;
  	iLastContrast = c;

    return 0;
}