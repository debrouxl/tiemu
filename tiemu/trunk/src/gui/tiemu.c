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

static GdkPixbuf *pixbuf = NULL;
static GdkPixbuf *img = NULL;

GtkWidget *da;

static int load_pixbuf(void)
{
	GError *error = NULL;
	const gchar *filename = "C:\\msvc\\tilp\\skins\\ti92.jpg";
	gint w, h;

	img = gdk_pixbuf_new_from_file(filename, &error);
	if(img == NULL)
		return -1;

	w = gdk_pixbuf_get_width(img);
	h = gdk_pixbuf_get_height(img);

	//gtk_drawing_area_size(GTK_DRAWING_AREA(da), w, h);
	//pixbuf = gdk_pixbuf_copy(img);

	gtk_drawing_area_size(GTK_DRAWING_AREA(da), w/2, h/2);
	pixbuf = gdk_pixbuf_scale_simple(img, w/2, h/2, GDK_INTERP_NEAREST);
	
	return 0;
}

gint display_main_wnd(void)
{
	GladeXML *xml;
	GtkWidget *dbox;
	gpointer data;

	xml = glade_xml_new
		(tilp_paths_build_glade("tiemu-2.glade"), "main_wnd",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	dbox = glade_xml_get_widget(xml, "main_wnd");

	da = data = glade_xml_get_widget(xml, "drawingarea1");

	load_pixbuf();
	compute_convtable();

	gtk_widget_show(dbox);

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
on_drawingarea1_button_press_event     (GtkWidget       *widget,
                                        GdkEventButton  *event,
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


GLADE_CB gboolean
on_drawingarea1_key_press_event        (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{

  return FALSE;
}


GLADE_CB gboolean
on_drawingarea1_key_release_event      (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{

  return FALSE;
}


GLADE_CB gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
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
}

extern int iScrW, iScrH;
extern uint32_t *pLcdBuf;
uint32_t convtab[];

int gtk_update_lcd(void)
{
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
}