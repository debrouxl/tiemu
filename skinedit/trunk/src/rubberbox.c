/*
 *   skinedit - a skin editor for the TiEmu emulator
 *   Copyright (C) 2002 Julien BLACHE <jb@tilp.info>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <stdint.h>
#include <string.h>

#include <SDL/SDL.h>
#include <gtk/gtk.h>
#include "gtksdl.h"

#include "support.h"
#include "struct.h"

#include "main_intf.h"
#include "boxes_intf.h"

#include "rubberbox.h"
#include "utils.h"

extern struct skinInfos skin_infos;

static SDL_Rect old_rect; /* previous rect */
static SDL_Rect tmp_rect; /* new rect */
static int ox, oy;        /* origin */

extern unsigned int lcd_mouse_motion; /* main_cb.c */
extern unsigned int lcd_button_press; /* main_cb.c */


gboolean
mouse_motion(GtkWidget *drawingarea, GdkEventMotion *event, gpointer action)
{
  int x, y; /* pointer coords */
  GdkModifierType state; 

  if (event->is_hint) 
    {
      gdk_window_get_pointer(event->window, &x, &y, &state); 
    }
  else 
    { 
      x = event->x; 
      y = event->y; 
      state = event->state; 
    } 

  if ((state & GDK_BUTTON1_MASK) == 0) /* button 1 is not pressed */
    return FALSE;

  /* be careful with GDK ... */

  if (x > skin_infos.width)
    x = skin_infos.width;
  
  if (y > skin_infos.height)
    y = skin_infos.height;

  if (x < 0)
    x = 0;
  
  if (y < 0)
    y = 0;

    
  if (x >= ox)
    {
      tmp_rect.x = ox;
      tmp_rect.w = (x - ox);

      if (y >= oy)
	{
	  tmp_rect.y = oy;
	  tmp_rect.h = (y - oy);
	}
      else
	{
	  /* remap Y orig */
	  tmp_rect.y = y;
	  tmp_rect.h = oy - y;
	}
    }
  else
    {
      /* remap X orig */
      tmp_rect.x = x;
      tmp_rect.w = ox - x;

      if (y >= oy)
	{
	  tmp_rect.y = oy;
	  tmp_rect.h = (y - oy);
	}
      else
	{
	  /* remap Y orig */
	  tmp_rect.y = y;
	  tmp_rect.h = oy - y;
	}
    }

  draw_rubberbox(NULL, tmp_rect);

  if (GPOINTER_TO_UINT(action) == ACTION_LCD_COORDS)
    {
      sbar_print(_("LCD size : %d x %d"), tmp_rect.w, tmp_rect.h);
    }

  return FALSE;
}

gboolean
button_press(GtkWidget *sdl_eventbox, GdkEventButton *event, gpointer action)
{
  if (event->button == 1) /* left button */
    {
      ox = event->x;
      oy = event->y;
      
      erase_rubberbox(NULL);
    }
  else if (event->button == 3) /* right button */
    {
      if (GPOINTER_TO_UINT(action) == ACTION_LCD_COORDS)
	{
	  skin_infos.lcd_pos.top = tmp_rect.y;
	  skin_infos.lcd_pos.left = tmp_rect.x;
	  skin_infos.lcd_pos.bottom = tmp_rect.y + tmp_rect.h;
	  skin_infos.lcd_pos.right = tmp_rect.x + tmp_rect.w;

	  gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
				lcd_mouse_motion);

	  gtk_signal_disconnect(GTK_OBJECT(sdl_eventbox),
				lcd_button_press);

	  lcd_mouse_motion = 0;
	  lcd_button_press = 0;

	  erase_rubberbox(NULL);

	  sbar_print(_("Saved LCD coordinates"));
	}
      else
	{
	  skin_infos.keys_pos[clist_row].top = tmp_rect.y;
	  skin_infos.keys_pos[clist_row].left = tmp_rect.x;
	  skin_infos.keys_pos[clist_row].bottom = tmp_rect.y + tmp_rect.h;
	  skin_infos.keys_pos[clist_row].right = tmp_rect.x + tmp_rect.w;

	  gtk_clist_moveto(GTK_CLIST(lookup_widget(GTK_WIDGET(list_keys_dialog), "list_keys")),
			   (((clist_row + 1) < clist_max) ? (clist_row + 1) : 0), 0, 0.5, 0);

	  gtk_clist_select_row(GTK_CLIST(lookup_widget(GTK_WIDGET(list_keys_dialog), "list_keys")),
			       (((clist_row + 1) < clist_max) ? (clist_row + 1) : 0), 0);
	}
    }
  
  return FALSE;
}

void put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue);
void get_pixel (GdkPixbuf *pixbuf, int x, int y, guchar *red, guchar *green, guchar *blue);


void
draw_rubberbox(GtkWidget *drawing_area, SDL_Rect rect)
{
#if ROMS
  SDL_Surface *s;
  SDL_Rect c;
  SDL_Rect oc;
  uint16_t *img_orig;
  uint16_t *SDLpixels;
  int i;

  img_orig = skin_infos.img_orig;
  s = sdl_area->surface;
  c = rect;
  oc = old_rect;

  if ((c.x > s->w) || (c.y > s->h) || ((c.x + c.w) > s->w) || ((c.y + c.h) > s->h))
    return;

  if(SDL_MUSTLOCK(s)) 
    {
      if(SDL_LockSurface(s) < 0)
	return;
    }

  memcpy(s->pixels, img_orig, (2 * s->w * s->h));

  /* vertical lines at c.x and (c.x + c.w), c.y < y < (c.y + c.h) */
  /* !!! must multiply by 2 because uint16_t !!! */

  for (i = c.y; i < (c.y + c.h); i++)
    {
      SDLpixels = (uint16_t *)(s->pixels) + (i * 2*s->w + 2*c.x);
      *SDLpixels = ~(*SDLpixels);

      SDLpixels = (uint16_t *)(s->pixels) + (i * 2*s->w + 2*(c.x + c.w));
      *SDLpixels = ~(*SDLpixels);
    }


  /* horizontal lines at c.y and (c.y + c.h), c.x < x < (c.x + c.w) */
  /* !!! must multiply by 2 because uint16_t !!! */

  for (i = 0; i < c.w; i++)
    {
      SDLpixels = (uint16_t *)(s->pixels) + (c.y * 2*s->w + 2*i + 2*c.x);
      *SDLpixels = ~(*SDLpixels);

      SDLpixels = (uint16_t *)(s->pixels) + ((c.y + c.h) * 2*s->w + 2*i + 2*c.x);
      *SDLpixels = ~(*SDLpixels);
    }


  if (SDL_MUSTLOCK(s)) 
    {
      SDL_UnlockSurface(s);
    }

  /*
   * 2 calls to SDL_UpdateRect :
   * + erase the old rect
   * + draw the new one
   *
   * We could composite a new rectangle with the new and old ones,
   * but this would consume more resources than these 2 calls.
   * Plus it would complicate the code. *sigh*
   */

  SDL_UpdateRect(s,
		 oc.x, oc.y,
		 ((oc.w + oc.x + 2) <= s->w) ? (oc.w + 2) : oc.w,  /* add 2 to really erase the lines (right, bottom) ... */
		 ((oc.h + oc.y + 2) <= s->h) ? (oc.h + 2) : oc.h); /* ... but be careful */

  SDL_UpdateRect(s,
		 c.x, c.y,
		 ((c.w + c.x + 2) <= s->w) ? (c.w + 2) : c.w,  /* add 2 to really erase the lines (right, bottom) ... */
		 ((c.h + c.y + 2) <= s->h) ? (c.h + 2) : c.h); /* ... but be careful */

  tmp_rect = rect; /* when called from callbacks.c (for LCD or keys) */
  old_rect = rect; /* save coords */
#else
  SDL_Rect c;
  SDL_Rect oc;
  uint16_t *SDLpixels;
  int x, y;	//i
  GdkRectangle update_rect;
  guchar r, g, b;
  
  c = rect;
  oc = old_rect;

  if ((c.x > skin_infos.width) || 
      (c.y > skin_infos.height) || 
      ((c.x + c.w) > skin_infos.width) || 
      ((c.y + c.h) > skin_infos.height))
    return;
    
  /* vertical lines at c.x and (c.x + c.w), c.y < y < (c.y + c.h) */
  /* !!! must multiply by 2 because uint16_t !!! */
  for (y = c.y; y < (c.y + c.h); y++)
    {
      get_pixel (pixbuf, c.x, y, &r, &g, &b);
      r = ~r; g = ~g; b = ~b;
      put_pixel (pixbuf, c.x, y, r, g, b);
      
      get_pixel (pixbuf, c.x + c.w, y, &r, &g, &b);
      r = ~r; g = ~g; b = ~b;
      put_pixel (pixbuf, c.x + c.w, y, r, g, b);
    }

  /* horizontal lines at c.y and (c.y + c.h), c.x < x < (c.x + c.w) */
  /* !!! must multiply by 2 because uint16_t !!! */

  for (x = c.x; x < c.x + c.w; x++)
    {
      get_pixel (pixbuf, x, c.y, &r, &g, &b);
      r = ~r; g = ~g; b = ~b;
      put_pixel (pixbuf, x, c.y, r, g, b);
      
      get_pixel (pixbuf, x, c.y + c.h, &r, &g, &b);
      r = ~r; g = ~g; b = ~b;
      put_pixel (pixbuf, x, c.y + c.h, r, g, b);
    } 

/*
   * 2 calls to SDL_UpdateRect :
   * + erase the old rect
   * + draw the new one
   *
   * We could composite a new rectangle with the new and old ones,
   * but this would consume more resources than these 2 calls.
   * Plus it would complicate the code. *sigh*
   */
   
  update_rect.x = oc.x;
  update_rect.y = oc.y;
  update_rect.width = ((oc.w + oc.x + 2) <= skin_infos.width) ? (oc.w + 2) : oc.w;  /* add 2 to really erase the lines (right, bottom) ... */
  update_rect.height = ((oc.h + oc.y + 2) <= skin_infos.height) ? (oc.h + 2) : oc.h; /* ... but be careful */

  gdk_draw_pixbuf(drawing_area->window,
		  drawing_area->style->fg_gc[GTK_WIDGET_STATE(drawing_area)],
		  pixbuf, 
		  update_rect.x, update_rect.y,
                  update_rect.x, update_rect.y,
                  update_rect.width, update_rect.height,
		  GDK_RGB_DITHER_NONE, 0, 0);

  update_rect.x = c.x;
  update_rect.y = c.y;
  update_rect.width = ((c.w + c.x + 2) <= skin_infos.width) ? (c.w + 2) : c.w;  /* add 2 to really erase the lines (right, bottom) ... */
  update_rect.height = ((c.h + c.y + 2) <= skin_infos.height) ? (c.h + 2) : c.h; /* ... but be careful */
  
  gdk_draw_pixbuf(drawing_area->window,
		  drawing_area->style->fg_gc[GTK_WIDGET_STATE(drawing_area)],
		  pixbuf, 
		  update_rect.x, update_rect.y,
                  update_rect.x, update_rect.y,
                  update_rect.width, update_rect.height,
		  GDK_RGB_DITHER_NONE, 0, 0);

  tmp_rect = rect; /* when called from callbacks.c (for LCD or keys) */
  old_rect = rect; /* save coords */
  
  put_pixel(pixbuf, 5, 5, 0x55, 0xaa, 0x55);

#endif  
}

void
erase_rubberbox(GtkWidget *drawing_area)
{
#if ROMS
  uint16_t *img_orig;
  SDL_Surface *s;

  img_orig = skin_infos.img_orig;

  s = sdl_area->surface;
  if(SDL_MUSTLOCK(s)) 
    {
      if(SDL_LockSurface(s) < 0)
	return;
    }

  memcpy(s->pixels, img_orig, (2 * s->w * s->h));

  if (SDL_MUSTLOCK(s)) 
    {
      SDL_UnlockSurface(s);
    }
  
  SDL_UpdateRect(s, 0, 0, 0, 0);

  memset(&tmp_rect, 0, sizeof(SDL_Rect));
  memset(&old_rect, 0, sizeof(SDL_Rect));
#else
  // copy original image
  g_object_unref(pixbuf);
  pixbuf = gdk_pixbuf_copy(orig);
  
  // force redraw
  //gtk_signal_emit_by_name(GTK_OBJECT(drawing_area), "expose_event");
#endif
}


static void
put_pixel (GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
  g_assert (!gdk_pixbuf_get_has_alpha (pixbuf));
  g_assert (n_channels == 3);

  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  p[0] = red;
  p[1] = green;
  p[2] = blue;
  p[3] = 0;	//alpha;
}

static void
get_pixel (GdkPixbuf *pixbuf, int x, int y, guchar *red, guchar *green, guchar *blue)
{
  int width, height, rowstride, n_channels;
  guchar *pixels, *p;

  n_channels = gdk_pixbuf_get_n_channels (pixbuf);

  g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
  g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
  g_assert (!gdk_pixbuf_get_has_alpha (pixbuf));
  g_assert (n_channels == 3);

  width = gdk_pixbuf_get_width (pixbuf);
  height = gdk_pixbuf_get_height (pixbuf);

  g_assert (x >= 0 && x < width);
  g_assert (y >= 0 && y < height);

  rowstride = gdk_pixbuf_get_rowstride (pixbuf);
  pixels = gdk_pixbuf_get_pixels (pixbuf);

  p = pixels + y * rowstride + x * n_channels;
  *red = p[0];
  *green = p[1];
  *blue = p[2];
  //p[3] = alpha;
}