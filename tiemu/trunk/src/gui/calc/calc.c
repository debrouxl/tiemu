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
#include "dbg_all.h"
#include "screenshot.h"

GtkWidget *wnd = NULL;
GtkWidget *area = NULL;

extern GdkPixbuf *lcd;
extern GdkPixbuf *skn;
extern GdkPixmap *pixmap;

extern const char* key_mapping;
extern const char  sknKey92[];
extern const char  sknKey89[];

extern uint32_t *lcd_bytmap;
extern PIX_INFOS pi;

gint display_main_wnd(void)
{
	GladeXML *xml;

	xml = glade_xml_new
		(tilp_paths_build_glade("calc-2.glade"), "calc_wnd",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	wnd = glade_xml_get_widget(xml, "calc_wnd");
	area = glade_xml_get_widget(xml, "drawingarea1");

    gtk_widget_show(area);
	gtk_widget_show(wnd);

	return 0;
}

GLADE_CB void
on_calc_wnd_destroy                    (GtkObject       *object,
                                        gpointer         user_data)
{
    //gtk_main_quit();
}

GLADE_CB gboolean
on_calc_wnd_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    return TRUE;	// block destroy
}

GLADE_CB gboolean
on_drawingarea1_configure_event        (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
    return FALSE;
}

GLADE_CB gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
	if(pixmap == NULL)
	    return FALSE;

    gdk_draw_pixmap(
        widget->window,
		widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		pixmap,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height);

	return FALSE;
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

G_LOCK_EXTERN(lcd_flag);
extern volatile int lcd_flag;
extern volatile int debugger;
static gint tid = -1;

static gint hid_refresh (gpointer data)
{
    if(lcd_flag || (tihw.calc_type == HW2))
    {
		// TI92+: jackycar, TI89: baballe
        hid_update_lcd();
        G_LOCK(lcd_flag);
        lcd_flag = 0;
		//printf("<");
        G_UNLOCK(lcd_flag);

        // Toggles every FS (every time the LCD restarts at line 0)
        tihw.io2[0x1d] |= (1 << 7);
    }

    return TRUE;
}

void compute_convtable(void);
void compute_grayscale(void);
void redraw_skin(void);

int  hid_init(void)
{
    SKIN_INFOS *si = &skin_infos;

    // Found a skin
	match_skin(tihw.calc_type);

    // Init the planar/chunky conversion table for LCD
  	compute_convtable();

    // Load skin
    if(skin_load(options.skin_file) == -1) 
    {
	    gchar *s = g_strdup_printf("unable to load this skin: <%s>\n", options.skin_file);
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }
    skn = skin_infos.image;
  
	// Set keymap depending on calculator type
    switch(tihw.calc_type)
    {
    case TI92:
    case TI92p:
    case V200:
        key_mapping = sknKey92;
        break;
    case TI89:
    case TI89t:
      	key_mapping = sknKey89;
        break;
    default:
        {
	  	gchar *s = g_strdup_printf("no skin found for this calc\n");
	  	tiemu_error(0, s);
	  	g_free(s);
	  	return -1;
        }
	}
        
    // Allocate the TI screen buffer
	lcd_bytmap = malloc(LCDMEM_W * LCDMEM_H);

    // Allocate the lcd pixbuf
    lcd = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, LCDMEM_W, LCDMEM_H);
    if(lcd == NULL)
    {
        gchar *s = g_strdup_printf("unable to create LCD pixbuf.\n");
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }
    
    pi.n_channels = gdk_pixbuf_get_n_channels (lcd);
	pi.width = gdk_pixbuf_get_width (lcd);
	pi.height = gdk_pixbuf_get_height (lcd);
	pi.rowstride = gdk_pixbuf_get_rowstride (lcd);
	pi.pixels = gdk_pixbuf_get_pixels (lcd);

	// Create main window
	display_main_wnd();

    // Allocate the backing pixmap
    pixmap = gdk_pixmap_new(wnd->window, si->width, si->height, -1);
    if(pixmap == NULL)
    {
        gchar *s = g_strdup_printf("unable to create backing pixbuf.\n");
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }

    // Get window size depending on windowed/fullscreen
  	if(params.background)
	{
        gtk_drawing_area_size(GTK_DRAWING_AREA(area), si->width, si->height);
		gtk_window_resize(GTK_WINDOW(wnd), si->width, si->height);
	}
    else
	{
        gtk_drawing_area_size(GTK_DRAWING_AREA(area), tihw.lcd_w, tihw.lcd_h);  
		gtk_window_resize(GTK_WINDOW(wnd), tihw.lcd_w, tihw.lcd_h);
	}
    
    // Draw the skin and compute grayscale palette
  	compute_grayscale();
  	redraw_skin();

    // Install LCD refresh: 100 FPS (10 ms)
    tid = g_timeout_add((params.lcd_rate == -1) ? 10 : params.lcd_rate, 
		(GtkFunction)hid_refresh, NULL);

    return 0;
}

int  hid_exit(void)
{

    // Uninstall LCD refresh
    g_source_remove(tid);

	// Release resources
    if(lcd != NULL)
    {
        //g_object_unref(lcd);
        lcd = NULL;
    }

    if(pixmap != NULL)
    {
        g_object_unref(pixmap);
        pixmap = NULL;
    }

    // Destroy window
    gtk_widget_destroy(wnd);

    return 0;
}

int hid_switch_with_skin(void)
{
    params.background = 1;
    gtk_drawing_area_size(GTK_DRAWING_AREA(area), skin_infos.width, skin_infos.height);
	gtk_window_resize(GTK_WINDOW(wnd), skin_infos.width, skin_infos.height);

	redraw_skin();

    return 0;
}

int hid_switch_without_skin(void)
{
    params.background = 0;
    gtk_drawing_area_size(GTK_DRAWING_AREA(area), tihw.lcd_w, tihw.lcd_h);
	gtk_window_resize(GTK_WINDOW(wnd), tihw.lcd_w, tihw.lcd_h);

    return 0;
}

int hid_change_skin(const char *filename)
{
    int ret1, ret2;
	
	ret1 = hid_exit();
	ret2 = hid_init();
	
	return ret1 | ret2;
}

static gint fullscreen = 0;

int hid_switch_fullscreen(void)
{
	gdk_window_fullscreen(wnd->window);
    fullscreen = !0;

	return 0;
}

int hid_switch_windowed(void)
{
	gdk_window_unfullscreen(wnd->window);
    fullscreen = 0;

	return 0;
}

int hid_switch_normal_view(void)
{
    if(fullscreen)
        hid_switch_windowed();

    return 0;
}

int hid_switch_large_view(void)
{
    return 0;
}

int  hid_screenshot(char *filename)
{
	gchar *outfile;
	gchar *ext = "???";
	gchar *type = "???";

	GdkPixbuf *pixbuf = { 0 };
	gboolean result = FALSE;
	GError *error = NULL;

	if(filename == NULL) 
	{
		switch(options2.format) 
		{
			case IMG_JPG: ext = "jpg"; type = "jpeg"; break;
			case IMG_PNG: ext = "png"; type = "png";  break;
			case IMG_ICO: ext = "ico"; type = "ico";  break;
			default: type = "???"; break;
		}
      
		outfile = g_strdup_printf("%s%03i.%s", options2.file, options2.counter, ext);
	} 
	else 
	{
		outfile = g_strdup(filename);
	}

	DISPLAY("Screenshot to %s... ", outfile);

	if((options2.size == IMG_LCD) && (options2.type == IMG_BW)) 
	{
		// get pixbuf from buffer	
        return 0;
	} 
	else if((options2.size == IMG_LCD) && (options2.type == IMG_COL)) 
	{
        // get pixbuf from grayscale lcd
		pixbuf = gdk_pixbuf_copy(lcd);
	} 
	else if(options2.size == IMG_SKIN) 
	{
		// get pixbuf from backing pixmap
		pixbuf = gdk_pixbuf_get_from_drawable(
					NULL, wnd->window, NULL,
					0, 0, 0, 0, skin_infos.width, skin_infos.height);
	}

	//result = gdk_pixbuf_save(pixbuf, outfile, type, &error, "quality", "100", NULL);
	result = gdk_pixbuf_save(pixbuf, outfile, type, &error, NULL);
	if (result == FALSE) 
	{
		DISPLAY("Failed to save pixbuf file: %s: %s\n", outfile, error->message);
		g_error_free(error);
	}
    g_object_unref(pixbuf);

	DISPLAY("Done !\n");
	options2.counter++;
	g_free(filename);

	return 0;
}