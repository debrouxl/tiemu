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
static int ox, oy; /* origin */

extern unsigned int lcd_mouse_motion; /* main_cb.c */
extern unsigned int lcd_button_press; /* main_cb.c */


gboolean
mouse_motion(GtkWidget *sdl_eventbox, GdkEventMotion *event, gpointer action)
{
  int x, y; /* pointer coords */

  SDL_Surface *s;

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

  s = GTK_SDL(GTK_BIN(sdl_eventbox)->child)->surface;

  /* be careful with GDK ... */

  if (x > s->w)
    x = s->w;
  
  if (y > s->h)
    y = s->h;

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

  draw_rubberbox(GTK_SDL(GTK_BIN(sdl_eventbox)->child), tmp_rect);

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
      
      erase_rubberbox(GTK_SDL(GTK_BIN(sdl_eventbox)->child));
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

	  erase_rubberbox(GTK_SDL(GTK_BIN(sdl_eventbox)->child));

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


void
draw_rubberbox(GtkSDL *sdl_area, SDL_Rect rect)
{
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
      SDLpixels = (uint16_t *)(s->pixels + (i * 2*s->w + 2*c.x));
      *SDLpixels = ~(*SDLpixels);

      SDLpixels = (uint16_t *)(s->pixels + (i * 2*s->w + 2*(c.x + c.w)));
      *SDLpixels = ~(*SDLpixels);
    }


  /* horizontal lines at c.y and (c.y + c.h), c.x < x < (c.x + c.w) */
  /* !!! must multiply by 2 because uint16_t !!! */

  for (i = 0; i < c.w; i++)
    {
      SDLpixels = (uint16_t *)(s->pixels + (c.y * 2*s->w + 2*i + 2*c.x));
      *SDLpixels = ~(*SDLpixels);

      SDLpixels = (uint16_t *)(s->pixels + ((c.y + c.h) * 2*s->w + 2*i + 2*c.x));
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
  
}

void
erase_rubberbox(GtkSDL *sdl_area)
{

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
}
