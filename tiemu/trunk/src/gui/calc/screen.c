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
#include "calc.h"

/* Types */

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

uint32_t *lcd_bytmap;				// LCD buffer (color-mapped as grayscale)
PIX_INFOS pi;

static uint32_t convtab[512];      	// planar to chunky conversion table
static RGB      grayscales[16];		// gray scales rgb values (colormap)
static int lcd_state = 0;           // screen state

static int contrast = NGS;          // current contrast level
static int old_contrast = 0;        // previous contrast level
static int new_contrast = NGS;		// new contrast level

static int max_plane = -1;         	// number of grayscales to emulate
static int cur_plane = 0;           // current grayscale plane

/* Compute conversion table */
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

/* Computes the 16 grays level colors */
void compute_grayscale(void)
{
  	int i;
  	int sr, sg, sb;
  	int er, eg, eb;
  	int r, g ,b;
    uint32_t white = skin_infos.lcd_white;  // 0xcfe0ce
    uint32_t black = skin_infos.lcd_black;  // 0x222e31

    //printf("B&W: %08lx %08lx\n", skin_infos.lcd_black, skin_infos.lcd_white);

  	sr = (white & 0xff0000) >> 8;
  	sg = (white & 0x00ff00);
  	sb = (white & 0x0000ff) << 8;
  	
  	er = (black & 0xff0000) >> 8;
  	eg = (black & 0x00ff00);
  	eb = (black & 0x0000ff) << 8;

  	if(contrast < NGS) 
    {
      	sr = sr - (sr-er) * (NGS - contrast)/13;
      	sg = sg - (sg-eg) * (NGS - contrast)/13;
      	sb = sb - (sb-eb) * (NGS - contrast)/13;
    }
  	else 
    {
      	er = er - (er-sr)*(contrast - NGS)/13;
      	eg = eg - (eg-sg)*(contrast - NGS)/13;
      	eb = eb - (eb-sb)*(contrast - NGS)/13;
    }
  
  	r = sr;
  	g = sg;
  	b = sb;

  	if(lcd_state == SCREEN_ON) 
    {
      	for(i = 0; i <= (max_plane+1); i++) 
		{
	  		grayscales[i].r = filter(r, 0x0000, 0xA800) >> 8;
	  		grayscales[i].g = filter(g, 0x0000, 0xB400) >> 8;
	  		grayscales[i].b = filter(b, 0x3400, 0xA800) >> 8;

	  		r -= ((sr-er) / (max_plane+1));
	  		g -= ((sg-eg) / (max_plane+1));
	  		b -= ((sb-eb) / (max_plane+1));
		}
    }
}

/* Redraw the skin into window but don't reload skin file */
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

/* Update LCD screen part */
int hid_update_lcd(void)
{
	int i, j, k;
	uint8_t *lcd_bitmap = tihw.lcd_ptr;
	uint8_t *lcd_buf = (uint8_t *)lcd_bytmap;
	GdkRect src, dst;
	guchar *p;

    if(!pixmap || !lcd)
        return 0;
	
    if(max_plane != params.grayplanes) 
    {
		max_plane = params.grayplanes;
		compute_grayscale();
    }

	if(lcd_state == SCREEN_OFF)
		return 0;

	if(contrast != new_contrast) 
    {
		contrast = new_contrast;
		compute_grayscale();
    }

	// Convert the bitmap screen to a bytemap screen */
	if(!max_plane || !cur_plane) 
    { 
		// no gray scale or init gray plane
		for(j = 0, k = 0; k < LCDMEM_H; k++)
		{
			for(i = 0; i < LCDMEM_W/8; i++, lcd_bitmap++) 
			{
				lcd_bytmap[j++] = convtab[(*lcd_bitmap << 1)  ];
				lcd_bytmap[j++] = convtab[(*lcd_bitmap << 1)+1];
			}
		}
    }
	else 
    { 
		// compute gray scale
		for(j = 0, k = 0; k < LCDMEM_H; k++)
		{
			for(i = 0; i < LCDMEM_W/8; i++, lcd_bitmap++) 
			{
				lcd_bytmap[j++] += convtab[(*lcd_bitmap << 1)  ];
				lcd_bytmap[j++] += convtab[(*lcd_bitmap << 1)+1];
			}
		}
    }

    if(cur_plane++ >= max_plane) 
	{
		// Copy LCD from buffer to pixbuf
		for(j = 0; j < LCDMEM_H; j++) 
		{
			for (i = 0; i < LCDMEM_W; i++) 
			{
				p = pi.pixels + j * pi.rowstride + i * pi.n_channels;
				
				p[0] = grayscales[*lcd_buf].r;
				p[1] = grayscales[*lcd_buf].g;
				p[2] = grayscales[*lcd_buf].b;
				lcd_buf++;
			}
		}

        cur_plane = 0;

        // Copy surface into window
        src.x = 0;
        src.y = 0;
		src.w = tihw.log_w > tihw.lcd_w ? tihw.lcd_w : tihw.log_w;
		src.h = tihw.log_h > tihw.lcd_h ? tihw.lcd_h : tihw.log_h;

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
        dst.w = src.w;
        dst.h = src.h;

        gdk_draw_pixbuf(pixmap, wnd->style->fg_gc[GTK_WIDGET_STATE(wnd)],
		  lcd, src.x, src.y, dst.x, dst.y, src.w, src.h,
		  GDK_RGB_DITHER_NONE, 0, 0);

		gtk_widget_draw(area, (GdkRectangle *)&dst);
    }

    return -1;
}

/* Set LCD state */
void hid_lcd_on_off(int i) 
{
    if(i) 
	{
		lcd_state = SCREEN_ON;
	} 
	else 
	{
		lcd_state = SCREEN_OFF;
		redraw_skin(); 	// to clear LCD 
	}
}

/* Set LCD contrast */
int hid_set_contrast(int c)
{
    if((tihw.calc_type == TI89) || (tihw.calc_type == TI89t))
    	c = 31-c;

  	new_contrast = (c+old_contrast) / 2;
  	old_contrast = c;

    return 0;
}
