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


GtkWidget *wnd = NULL;
GtkWidget *area = NULL;

gint display_main_wnd(void)
{
	GladeXML *xml;

	xml = glade_xml_new
		(tilp_paths_build_glade("tiemu-2.glade"), "main_wnd",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	wnd = glade_xml_get_widget(xml, "main_wnd");
	area = glade_xml_get_widget(xml, "drawingarea1");

	gtk_widget_show(wnd);

	return 0;
}



GLADE_CB gboolean
on_drawingarea1_configure_event        (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{

  return FALSE;
}


GLADE_CB gboolean
on_drawingarea1_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


GLADE_CB gboolean
on_drawingarea1_destroy_event          (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}

static void redraw_skin(void) ;

GLADE_CB gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
    /*
	GdkRectangle rect;

	if(pixbuf == NULL)
	    return FALSE;

	memcpy(&rect, &(event->area), sizeof(GdkRectangle));
  
	gdk_draw_pixbuf(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
		  pixbuf, 
		  rect.x, rect.y,
		  rect.x, rect.y,
		  rect.width, rect.height,
		  GDK_RGB_DITHER_NONE, 0, 0);

	return TRUE;
  */
    return FALSE;
}

extern int iScrW, iScrH;
extern uint32_t *pLcdBuf;
uint32_t convtab[];

int gtk_update_lcd(void)
{
    /*
	int i, j, k;
	uint8_t *pLcdMem = tihw.lcd_ptr;
	uint8_t *ptr = (uint8_t *)pLcdBuf;
	GdkRectangle update_rect;

	int width, height, rowstride, n_channels;
	guchar *pixels, *p;

	n_channels = gdk_pixbuf_get_n_channels (pixbuf);
	width = gdk_pixbuf_get_width (pixbuf);
	height = gdk_pixbuf_get_height (pixbuf);
	rowstride = gdk_pixbuf_get_rowstride (pixbuf);
	pixels = gdk_pixbuf_get_pixels (pixbuf);

	//
		for(j=0, k=0; k<iScrH; k++)
		{
			for(i=0; i<iScrW>>3; i++, pLcdMem++) 
			{
				pLcdBuf[j++] = convtab[((*pLcdMem)<<1)  ];
				pLcdBuf[j++] = convtab[((*pLcdMem)<<1)+1];
			}
		}

	//		
		for(j = 0; j < iScrH; j++) 
		{
			for (i = 0; i < iScrW; i++) 
			{
				p = pixels + j * rowstride + i * n_channels;

				p[0] = *ptr ? 0x00 : 0xff;
				p[1] = *ptr ? 0x00 : 0xff;
				p[2] = *ptr ? 0x00 : 0xff;
				p[3] = 0;

				ptr++;
			}
		}

		update_rect.x = update_rect.y = 0;
		update_rect.width = iScrW;
		update_rect.height = iScrH;

		gtk_widget_draw(da, (GdkRectangle *)(&update_rect));
        */
    return -1;
}


/* Compat */

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

GdkPixbuf *lcd;
GdkPixbuf *skn;
GdkPixbuf *img;

uint32_t convtab[512];      // Planar to chunky conversion table
RGBA     grayscales[16];    // Gray scales rgb values

const char* key_mapping = NULL; // key mapping
extern const char sknKey92[];   // in tikeys.c
extern const char sknKey89[];
extern const char sknKeyV2[];

int iContrast = NGS;          // current contrast level
int iLastContrast = 0;        // previous contrast level
int iNewContrast = NGS;       // new contrast level
int iGrayPlanes = -1;         // number of grayscales to emulate
int iCurrPlane = 0;           // ?
int iScrState = 0;            // screen state


static void compute_convtable(void) 
{
  	int i, j;
  	uint8_t k;
  	uint8_t *tmptab = (uint8_t *)convtab;

  	for(i=0, j=0; i<256; i++) 
    {
      	for(k = 1<<7; k; k>>=1)
		{
			tmptab[j++] = (i & k) ? 1 : 0;
		}
    }
}

/* The value v is between l and h and can not be outside */
#define filter(v, l, h) (v<l ? l : (v>h ? h : v))

/* Computes the 16 grays level colors and allocates a colormap */
static void set_colors(void)
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

static int match_skin(int calc_type)
{
	SKIN_INFOS si;
	int ok;
	gchar *skn_name, *s;

	s = g_strdup(ti68k_calctype_to_string(calc_type));
	skn_name = g_ascii_strdown(s, strlen(s));

	// filename is "", load default skin
	if(!strcmp(g_basename(options.skin_file), ""))
	{
		g_free(options.skin_file);
		options.skin_file = g_strdup_printf("%s%s.skn", 
					    inst_paths.skin_dir, skn_name);
		g_free(skn_name);
		return -1;
	}

	// load skin header
	if(skin_read_header(options.skin_file, &si) == -1)
	{
		g_free(options.skin_file);
      	options.skin_file = g_strdup_printf("%s%s.skn", 
					    inst_paths.skin_dir, skn_name);
	g_free(skn_name);
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
        case TI89t:
            ok = !strcmp(si.calc, SKIN_TI89) || !strcmp(si.calc, SKIN_TI89T);
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
			inst_paths.skin_dir, skn_name);

	//tiemu_error(0, _("skin incompatible with the current calc model. Falling back to default skin."));
	g_free(skn_name);
		return -1;
	}

g_free(skn_name);
	return 0;
}

/* 
   Redraw the skin into window but don't reload skin file
*/
static void redraw_skin(void) 
{
    GdkRect rect;

  	if(!params.background) 
    	return;
  
	if(skn == NULL)
	    return;
  
	gdk_draw_pixbuf(wnd->window, wnd->style->fg_gc[GTK_WIDGET_STATE(wnd)],
		  skn, 0, 0, 0, 0, -1, -1,
		  GDK_RGB_DITHER_NONE, 0, 0);

    rect.x = rect.y = 0;
    rect.w = skin_infos.width;
    rect.h = skin_infos.height;
    gtk_widget_draw(area, (GdkRectangle *)&rect);
}

int  hid_init(void)
{
    // Create main window
    if(wnd == NULL)
        display_main_wnd();

    // Found a skin
	match_skin(tihw.calc_type);

    // Init the planar/chunky conversion table for LCD
  	compute_convtable();

    // Allocate the TI screen buffer
    lcd = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, BPP, 240, 128);
    if(lcd == NULL)
    {
        gchar *s = g_strdup_printf("unable to create LCD pixbuf.\n");
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }

    if(skin_load(options.skin_file) == -1) 
    {
	    gchar *s = g_strdup_printf("unable to load this skin: <%s>\n", options.skin_file);
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }
    skn = skin_infos.image;
  
	// Set keymap depending on calculator type
	if((tihw.calc_type == TI92) || (tihw.calc_type == TI92p))
      	key_mapping = sknKey92;
	else if ((tihw.calc_type == TI89) || (tihw.calc_type == TI89t))
      	key_mapping = sknKey89;
	else if(tihw.calc_type == V200)
      	key_mapping = sknKeyV2;
	else
	{
	  	gchar *s = g_strdup_printf("no skin found for this calc\n");
	  	tiemu_error(0, s);
	  	g_free(s);
	  	return -1;
	}

    // Get window size depending on windowed/fullscreen
  	if(params.background)
        gtk_drawing_area_size(GTK_DRAWING_AREA(area), skin_infos.width, skin_infos.height);
    else
        gtk_drawing_area_size(GTK_DRAWING_AREA(area), tihw.lcd_w, tihw.lcd_h);  
    
    // Allocate the backing pixbuf
    img = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, BPP, skin_infos.width, skin_infos.height);
    if(img == NULL)
    {
        gchar *s = g_strdup_printf("unable to create LCD pixbuf.\n");
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }

    // Draw the skin and compute grayscale palette
  	set_colors();
  	redraw_skin();

    return 0;
}

int  hid_exit(void)
{
    if(lcd != NULL)
    {
        g_object_unref(lcd);
        lcd = NULL;
    }

    return 0;
}

int hid_update_keys(void)
{
    return 0;
}

int hid_update_lcd(void)
{
    return 0;
}

static void hid_lcd_on_off(int i) 
{
}

static int hid_set_contrast(int c)
{
    return 0;
}

void hid_switch_with_skin(void)
{
}

void hid_switch_without_skin(void)
{
}

void hid_change_skin(const char *filename)
{
}

void hid_switch_fullscreen(void)
{
}

void hid_switch_windowed(void)
{
}

void hid_switch_normal_view(void)
{
}

void hid_switch_large_view(void)
{
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

int  hid_screenshot(char *filename)
{
    return 0;
}