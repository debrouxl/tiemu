/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin
 *  Copyright (c) 2005, Julien Blache
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
 *  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif				/*  */

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

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
#include "kbd_mapper.h"
#include "printl.h"

GtkWidget *main_wnd = NULL;
GtkWidget *area = NULL;

SKIN_INFOS skin_infos = { 0 };

extern GdkPixbuf*	lcd;
extern GdkPixbuf*	skn;
extern GdkPixmap*	pixmap;

extern Pc2TiKey*    kbd_keymap;
extern const char*	skn_keymap;
extern const char	sknKey92[];
extern const char	sknKey89[];

extern uint32_t*	lcd_bytmap;

extern LCD_INFOS	li;
extern SCL_INFOS	si;

extern LCD_RECT		ls;
extern LCD_RECT		lr;
extern SKN_RECT		sr;
extern WND_RECT		wr;

// part 1: set scale factor
static void set_scale(int view_mode)
{
	if(view_mode == VIEW_NORMAL)
	{
		options.scale = si.r = 1.0;
		options.skin = 1;
	}
	else if(view_mode == VIEW_LARGE)
	{
		options.scale = si.r = 2.0;
		options.skin = 1;
	}
	else if(view_mode == VIEW_FULL)
	{
		GdkScreen* screen = gdk_screen_get_default();
		gint sw = gdk_screen_get_width(screen);
		gint sh = gdk_screen_get_height(screen);

		si.r = (float)sw / lr.w;
		si.r = (float)sh / lr.h;
		//printf("%i %i %f\n", sw, lr.w, si.r);
		//printf("%i %i %f\n", sh, lr.h, si.r);

		options.scale = si.r = (float)1.0;	// restricted to 3.0, too CPU intensive !
		options.skin = 0;
	}
}

// part 2: compute sizes
static void set_infos(void)	// set window & lcd sizes
{
	// LCD rectangle (source: skin)
	ls.x = (int)(si.r * skin_infos.lcd_pos.left); 
	ls.y = (int)(si.r * skin_infos.lcd_pos.top);
	ls.w = (int)(si.r * tihw.lcd_w);
	ls.h = (int)(si.r * tihw.lcd_h);

	// LCD rectangle (target: window)
	if(options.skin) 
	{
		lr.x = ls.x; 
		lr.y = ls.y;
	}
	else 
	{
		lr.x = 0;
		lr.y = 0;
	}  
	lr.w = (int)(si.r * tihw.lcd_w);
	lr.h = (int)(si.r * tihw.lcd_h);


	// SKN rectangle
	sr.x = sr.y = 0;
	sr.w = (int)(si.r * skin_infos.width);
	sr.h = (int)(si.r * skin_infos.height);

	// WND rectangle (= LCD or SKN depending on w/ or w/o skin)
	wr.x = wr.y = 0;
	if(options.skin)
	{
		wr.w = sr.w;
		wr.h = sr.h;
	}
	else
	{
		wr.w = lr.w;
		wr.h = lr.h;
	}

#if 0
	printf("LCD src: %3i %3i %3i %3i\n", ls.x, ls.y, ls.w, ls.h);
	printf("LCD dst: %3i %3i %3i %3i\n", lr.x, lr.y, lr.w, lr.h);
	printf("SKN    : %3i %3i %3i %3i\n", sr.x, sr.y, sr.w, sr.h);
	printf("WND    : %3i %3i %3i %3i\n", wr.x, wr.y, wr.w, wr.h);
#endif
}

// part 3: set changes on window
static void set_window(int full_redraw) 
{
	if(main_wnd->window == NULL)
		return;

	// resize window and drawing area
	if(full_redraw)
		gtk_window_resize(GTK_WINDOW(main_wnd), wr.w, wr.h);
	else
		gdk_window_resize(main_wnd->window, wr.w, wr.h);

	// reallocate backing pixmap
	if(pixmap != NULL)
	{
		// free current backing pixmap
		g_object_unref(pixmap);
		pixmap = NULL;

		// and allocate a new one
		pixmap = gdk_pixmap_new(main_wnd->window, wr.w, wr.h, -1);
		if(pixmap == NULL)
		{
			gchar *s = g_strdup_printf("unable to create backing pixmap.\n");
			tiemu_error(0, s);
			g_free(s);
			return;
		}
	}
}

static void set_constraints(int mode)
{
	// Allows resizing of window with a constant aspect ratio.
	// Very annoying and not very useful under Windows but required under Linux.
	// But, enabling it make not possible to switch to real large view (x2).
#ifndef __WIN32__
	if(1)
	{
		GdkGeometry geom = { 0 };
		GdkWindowHints mask = GDK_HINT_MIN_SIZE | GDK_HINT_ASPECT;
		double r = (float)sr.w / sr.h;
		double o = r * 0.10;	// 10%

		geom.min_width = 100;
		geom.min_height = 100;
		//geom.max_width = 1024;
		//geom.max_height = 768;
		//geom.base_width = 0;
		//geom.base_height = 0;
		//geom.width_inc = 5;
		//geom.height_inc = 5;
		if(mode)
		{
		    geom.min_aspect = r; //r - o;
		    geom.max_aspect = r; //r + o;
		    gtk_window_set_geometry_hints(GTK_WINDOW(main_wnd), area, &geom, mask);
		}
		else
		{
		    gtk_window_set_geometry_hints(GTK_WINDOW(main_wnd), area, &geom, 
						  GDK_HINT_MIN_SIZE); 
		}
		
		//printf("%i %i %1.2f", sr.w, sr.h, r);		
	}
#endif
}

// Main wnd by loading glade xml file or by executing glade generated code
gint display_main_wnd(void)
{
	GladeXML *xml;
	gchar *title;

	xml = glade_xml_new
		(tilp_paths_build_glade("calc-2.glade"), "calc_wnd",
		 PACKAGE);
	if (!xml)
		g_error(_("%s: GUI loading failed !\n"), __FILE__);
	glade_xml_signal_autoconnect(xml);
	
	main_wnd = glade_xml_get_widget(xml, "calc_wnd");
	area = glade_xml_get_widget(xml, "drawingarea1");

	gtk_window_move(GTK_WINDOW(main_wnd), options3.calc.rect.x, options3.calc.rect.y);
	set_constraints(1);
	gtk_widget_realize(main_wnd);	// set drawing area valid

	// set window title (useful for TIGCC-IDE for instance)
	// Note: lpWindowName is "TiEmu (%s)" and lpClassName is "gdkWindowToplevel"
	title = g_strdup_printf("TiEmu (%s)", ti68k_calctype_to_string(tihw.calc_type));
	gtk_window_set_title(GTK_WINDOW(main_wnd), title);
	g_free(title);

	return 0;
}

GLADE_CB void
on_calc_wnd_destroy                    (GtkObject       *object,
                                        gpointer         user_data)
{
#ifdef __IPAQ__
    on_exit_without_saving_state1_activate(NULL, NULL);
#else
	return;
#endif
}

GLADE_CB gboolean
on_calc_wnd_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
#ifdef __IPAQ__
    return FALSE;
#else
    return TRUE;	// block destroy
#endif
}

extern void redraw_skin(void);

GLADE_CB gboolean
on_drawingarea1_configure_event        (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
	float factor;
	
	// compute scaling factor
	if(options.skin)	
		factor = (float)event->width / (float)skin_infos.width;
	else
		factor = (float)event->width / (float)tihw.lcd_w;

#if 0
	printf("on_drawingarea1_configure_event: %i %i %i %i\n", 
		event->x, event->y, event->width, event->height);
	printf("factor: %1.2f\n", factor);
#endif

	// if normal or large view then exits
	if((factor == 1.0) || (factor == 2.0))
		return FALSE;

	// set scaling factor
	options.scale = si.r = factor;
	options.view = VIEW_CUSTOM;

	// compute sizes
	set_infos();

	// and set window size
	set_window(0);
	redraw_skin();

    return FALSE;
}

GLADE_CB gboolean
on_drawingarea1_expose_event           (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
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
	SKIN_INFOS *sk = &skin_infos;
	int ok;
	gchar *skin_name, *s;

	s = g_strdup(ti68k_calctype_to_string(calc_type));
	skin_name = g_ascii_strdown(s, strlen(s));
	g_free(s);

	if(!strcmp(skin_name, "ti92+") || !strcmp(skin_name, "ti89t"))
		skin_name[4] = '\0';

#ifdef __IPAQ__
	s = g_strconcat("ipaq_", skin_name, NULL);
	g_free(skin_name);
	skin_name = s;
#endif

	// filename is "", load default skin
	if(!strcmp(g_basename(options.skin_file), ""))
	{
		g_free(options.skin_file);
		options.skin_file = g_strdup_printf("%s%s.skn", 
					    inst_paths.skin_dir, skin_name);
		g_free(skin_name);
		return -1;
	}

	// load skin header
	if(skin_read_header(sk, options.skin_file) == -1)
	{
		g_free(options.skin_file);
      	options.skin_file = g_strdup_printf("%s%s.skn", 
					    inst_paths.skin_dir, skin_name);
	    g_free(skin_name);
	    return -1;
	}

	// is skin compatible
	switch(tihw.calc_type)
	{
	    case TI92:
		case TI92p:
            ok = !strcmp(sk->calc, SKIN_TI92) || !strcmp(sk->calc, SKIN_TI92P);
		break;
	    case TI89:
        case TI89t:
            ok = !strcmp(sk->calc, SKIN_TI89) || !strcmp(sk->calc, SKIN_TI89T);
		break;
		case V200:
			ok = !strcmp(sk->calc, SKIN_V200);
		break;
	    default: 
            ok = 0;
		break;
	}

	if(!ok)
	{
		g_free(options.skin_file);
      	options.skin_file = g_strdup_printf("%s%s.skn", 
			inst_paths.skin_dir, skin_name);

	    //tiemu_error(0, _("skin incompatible with the current calc model. Falling back to default skin."));
	    g_free(skin_name);
		return -1;
	}

    g_free(skin_name);
	return 0;
}

static int match_keymap(int calc_type)
{
	gchar *keys_name, *s;
    int ct, ok;

	s = g_strdup(ti68k_calctype_to_string(calc_type));
	keys_name = g_ascii_strdown(s, strlen(s));

	if(!strcmp(keys_name, "ti92+") || !strcmp(keys_name, "ti89t"))
		keys_name[4] = '\0';
	if(!strcmp(keys_name, "v200plt"))
		strcpy(keys_name, "ti92");

	// filename is "", load default keymap
	if(!strcmp(g_basename(options.keys_file), ""))
	{
		g_free(options.keys_file);
		options.keys_file = g_strdup_printf("%s%s.map", 
					    inst_paths.skin_dir, keys_name);
	}

	// load keymap header
    ct = keymap_read_header(options.keys_file);
	if(ct == -1)
	{
		g_free(options.keys_file);
      	options.keys_file = g_strdup_printf("%s%s.map", 
					    inst_paths.skin_dir, keys_name);
	    g_free(keys_name);
	    return -1;
	}

    // is keymap compatible
	switch(tihw.calc_type)
	{
	    case TI92:
		case TI92p:
        case V200:
            ok = (ct == TI92) || (ct == TI92p) || (ct == V200);
		break;
	    case TI89:
        case TI89t:
            ok = (ct == TI89) || (ct == TI89t);
		break;
	    default: 
            ok = 0;
		break;
	}

	if(!ok)
	{
		g_free(options.keys_file);
      	options.keys_file = g_strdup_printf("%s%s.map", 
			inst_paths.skin_dir, keys_name);

	    //tiemu_error(0, _("keymap incompatible with the current calc model. Falling back to default keymap."));
	    g_free(keys_name);
		return -1;
	}

    g_free(keys_name);
	return 0;
}

G_LOCK_EXTERN(lcd_flag);
extern volatile int lcd_flag;
extern volatile int debugger;
static guint tid = -1;

extern int lcd_hook_hw2(int);

static gint hid_refresh (gpointer data)
{
    if(lcd_flag || (tihw.hw_type >= HW2))
    {
		// TI92+: jackycar, TI89: baballe
	    hid_update_lcd();
        G_LOCK(lcd_flag);
        lcd_flag = 0;
        G_UNLOCK(lcd_flag);

		if(tihw.hw_type >= HW2)
			lcd_hook_hw2(TRUE);
    }

    return TRUE;
}

void compute_convtable(void);
void compute_grayscale(void);

int  hid_init(void)
{
    // Found a PC keyboard keymap
    match_keymap(tihw.calc_type);

    // Load kbd keymap
    if(keymap_load(options.keys_file) == -1)
    {
	    gchar *s = g_strdup_printf("unable to load this keymap: <%s>\n", options.keys_file);
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }

    // Found a skin
	match_skin(tihw.calc_type);

    // Load skin (2 parts)
    if(skin_load(&skin_infos, options.skin_file) == -1) 
    {
	    gchar *s = g_strdup_printf("unable to load this skin: <%s>\n", options.skin_file);
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }

	// Allocate the skn pixbuf (if needed)
	skn = skin_infos.image;
  
	// Set skin keymap depending on calculator type
    switch(tihw.calc_type)
    {
    case TI92:
    case TI92p:
    case V200:
        skn_keymap = sknKey92;
        break;
    case TI89:
    case TI89t:
      	skn_keymap = sknKey89;
        break;
    default:
        {
	  	gchar *s = g_strdup_printf("no skin found for this calc\n");
	  	tiemu_error(0, s);
	  	g_free(s);
	  	return -1;
        }
	}

	// Set window/LCD sizes
	si.r = options.scale;
	set_scale(options.view);
	set_infos();

    // Allocate the TI screen buffer
	lcd_bytmap = (uint32_t *)malloc(LCDMEM_W * LCDMEM_H);

    // Allocate the lcd pixbuf
    lcd = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, LCDMEM_W, LCDMEM_H);
    if(lcd == NULL)
    {
        gchar *s = g_strdup_printf("unable to create LCD pixbuf.\n");
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }

	// Used by TI89 (the LCD view is clipped from memory view)
	si.l = gdk_pixbuf_new_subpixbuf(lcd, 0, 0, tihw.lcd_w, tihw.lcd_h);
    
	// Constants for LCD update (speed-up)
    li.n_channels = gdk_pixbuf_get_n_channels (lcd);
	li.width = gdk_pixbuf_get_width (lcd);
	li.height = gdk_pixbuf_get_height (lcd);
	li.rowstride = gdk_pixbuf_get_rowstride (lcd);
	li.pixels = gdk_pixbuf_get_pixels (lcd);

	// Create main window
	display_main_wnd();

    // Allocate the backing pixmap (used for drawing and refresh)
    pixmap = gdk_pixmap_new(main_wnd->window, wr.w, wr.h, -1);
    if(pixmap == NULL)
    {
        gchar *s = g_strdup_printf("unable to create backing pixmap.\n");
	    tiemu_error(0, s);
	    g_free(s);
	    return -1;
    }
    
    // Draw the skin and compute grayscale palette
	set_window(1);
	redraw_skin();
  	compute_grayscale();

    // Init the planar/chunky conversion table for LCD
  	compute_convtable();

    // Install LCD refresh: 100 FPS (10 ms)
    tid = g_timeout_add((params.lcd_rate == -1) ? 50 : params.lcd_rate, 
		(GtkFunction)hid_refresh, NULL);

	gtk_widget_show(main_wnd);	// show wnd here

	if(options.view == VIEW_FULL)
		gdk_window_fullscreen(main_wnd->window);
	
    return 0;
}

int  hid_exit(void)
{

    // Uninstall LCD refresh
    g_source_remove(tid);

	// Release resources
    if(lcd != NULL)
    {
        g_object_unref(lcd);
        lcd = NULL;
		g_object_unref(si.l);
		si.l = NULL;
    }

    if(pixmap != NULL)
    {
        g_object_unref(pixmap);
        pixmap = NULL;
    }

    // Destroy window
    gtk_widget_destroy(main_wnd);

    return 0;
}

void hid_lcd_rate_set(void)
{
	g_source_remove(tid);

	tid = g_timeout_add((params.lcd_rate == -1) ? 50 : params.lcd_rate, 
		(GtkFunction)hid_refresh, NULL);
}

int hid_switch_with_skin(void)
{
    options.skin = 1;
	set_infos();
	set_window(1);
	redraw_skin();

    return 0;
}

int hid_switch_without_skin(void)
{
    options.skin = 0;
	set_infos();
	set_window(1);
	redraw_skin();

    return 0;
}

int hid_change_skin(const char *filename)
{
    int ret1, ret2;
	
	ret1 = hid_exit();
	ret2 = hid_init();
	
	return ret1 | ret2;
}

int hid_switch_fullscreen(void)
{
	if(options.view != VIEW_FULL)
	{
		set_scale(options.view = VIEW_FULL);
		set_infos();
		set_window(1);
		redraw_skin();
		gdk_window_fullscreen(main_wnd->window);
	}

	return 0;
}

int hid_switch_normal_view(void)
{
	if(options.view != VIEW_NORMAL)
	{
		set_scale(options.view = VIEW_NORMAL);
		set_infos();
		set_window(1);
		redraw_skin();
		gdk_window_unfullscreen(main_wnd->window);
	}

    return 0;
}

int hid_switch_large_view(void)
{
	if(options.view != VIEW_LARGE)
	{
		set_scale(options.view = VIEW_LARGE);		
		set_infos();
		set_window(1);
		redraw_skin();
		gdk_window_unfullscreen(main_wnd->window);
	}

    return 0;
}

int  hid_screenshot(char *filename)
{
	gchar *outfile;
	gchar *ext = "";
	gchar *type = "";

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
 		    case IMG_EPS: ext = "eps"; type = "eps";  break;
 		    case IMG_PDF: ext = "pdf"; type = "pdf";  break;
			default: ext = "png"; type = "png";  break;
		}
      
		outfile = g_strdup_printf("%s%03i.%s", options2.file, options2.counter, ext);
	} 
	else 
	{
		outfile = g_strdup(filename);
	}

	printl(0, "Screenshot to %s... ", outfile);

	if((options2.size == IMG_LCD) && (options2.type == IMG_BW)) 
	{
		// get pixbuf from TI memory (LCD buffer)
		pixbuf = hid_copy_lcd();
	} 
	else if((options2.size == IMG_LCD) && (options2.type == IMG_COL)) 
	{
        // get pixbuf from grayscale lcd
		pixbuf = gdk_pixbuf_copy(si.l);
	} 
	else if((options2.size == IMG_SKIN) && (options2.type == IMG_COL))
	{
		// get pixbuf from backing pixmap
		pixbuf = gdk_pixbuf_get_from_drawable(NULL, pixmap, NULL, 0, 0, 0, 0, wr.w, wr.h);
	}
	else
	{
		printl(0, "Unsupported screenshot options combination, screenshot aborted.\n");
		g_free(filename);
		return 0;
        }

	switch (options2.format)
	{
	case IMG_EPS:
		result = tiemu_screen_write_eps(outfile, pixbuf, &error);
		break;
	case IMG_PDF:
		result = tiemu_screen_write_pdf(outfile, pixbuf, &error);
		break;
	default:
		result = gdk_pixbuf_save(pixbuf, outfile, type, &error, NULL);
		break;
	}

	if (result == FALSE) 
	{
		printl(0, "Failed to save pixbuf file: %s: %s\n", outfile, error->message);
		g_error_free(error);
	}
	g_object_unref(pixbuf);

	printl(0, "Done !\n");
	options2.counter++;
	g_free(filename);

	return 0;
}

GLADE_CB gboolean
on_calc_wnd_window_state_event         (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
/*
    GdkEventWindowState *wstate = (GdkEventWindowState *)event;
    GdkWindowState state = wstate->new_window_state;
    GdkWindowState mask = wstate->changed_mask;

    //printf("%04X %04X %i\n", wstate->new_window_state, wstate->changed_mask, GDK_WINDOW_STATE_ICONIFIED);

    if((mask & GDK_WINDOW_STATE_ICONIFIED) && (state & GDK_WINDOW_STATE_ICONIFIED))
        gtk_debugger_hide_all(!0);
        
    else if((mask & GDK_WINDOW_STATE_ICONIFIED) && !(state & GDK_WINDOW_STATE_ICONIFIED))
        gtk_debugger_show_all(!0);
*/
    return FALSE;
}
