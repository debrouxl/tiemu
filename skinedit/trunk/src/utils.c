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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#if !defined(__WIN32__) && !defined(__MACOSX__)
#include <byteswap.h>
#endif

#ifndef __MACOSX__
#include <glib.h>
#include "main_intf.h"
#endif

#include "struct.h"
#include "utils.h"
#include "defs.h"

extern struct skinInfos skin_infos;


void
set_calc_keymap(void)
{
  if (strcmp(skin_infos.calc, CALC_TI73) == 0)
    {
      skin_infos.keymap = 0;
    }
  else if (strcmp(skin_infos.calc, CALC_TI82) == 0)
    {
      skin_infos.keymap = 1;
    }
  else if (strcmp(skin_infos.calc, CALC_TI83) == 0)
    {
      skin_infos.keymap = 1;
    }
  else if (strcmp(skin_infos.calc, CALC_TI83P) == 0)
    {
      skin_infos.keymap = 1;
    }
  else if (strcmp(skin_infos.calc, CALC_TI85) == 0)
    {
      skin_infos.keymap = 2;
    }
  else if (strcmp(skin_infos.calc, CALC_TI86) == 0)
    {
      skin_infos.keymap = 2;
    }
  else if (strcmp(skin_infos.calc, CALC_TI89) == 0)
    {
      skin_infos.keymap = 3;
    }
  else if (strcmp(skin_infos.calc, CALC_TI92) == 0)
    {
      skin_infos.keymap = 4;
    }
  else if (strcmp(skin_infos.calc, CALC_TI92P) == 0)
    {
      skin_infos.keymap = 4;
    }
  else if (strcmp(skin_infos.calc, CALC_V200) == 0)
    {
      skin_infos.keymap = 4;
    }
}


void
clear_skin_infos(void)
{
  if (skin_infos.name != NULL)
    free(skin_infos.name);
  
  if (skin_infos.author != NULL)
    free(skin_infos.author);
  
  if (skin_infos.skin_path != NULL)
    free(skin_infos.skin_path);
  
  if (skin_infos.jpeg_path != NULL)
    free(skin_infos.jpeg_path);

  if (skin_infos.img_orig != NULL)
    {
      g_object_unref(skin_infos.img_orig);
      skin_infos.img_orig = NULL;
    }

  memset(&skin_infos, 0, sizeof(struct skinInfos));
}


void
vti_calc_type_to_string(uint32_t calc)
{
  switch(calc)
    {
      case 73:
	strcpy(skin_infos.calc, CALC_TI73);
	break;
      case 82:
	strcpy(skin_infos.calc, CALC_TI82);
	break;
      case 83:
	strcpy(skin_infos.calc, CALC_TI83);
	break;
      case 84:
	strcpy(skin_infos.calc, CALC_TI83P);
	break;
      case 85:
	strcpy(skin_infos.calc, CALC_TI85);
	break;
      case 86:
	strcpy(skin_infos.calc, CALC_TI86);
	break;
      case 89:
	strcpy(skin_infos.calc, CALC_TI89);
	break;
      case 92:
	strcpy(skin_infos.calc, CALC_TI92);
	break;
      case 94:
	strcpy(skin_infos.calc, CALC_TI92P);
	break;
      default: /* should not get there */
	break;
    }
}



#if (G_BYTE_ORDER == G_BIG_ENDIAN) || defined(__MACOSX__)
void
byteswap_vti_skin(uint32_t *calc)
{
  int i;

  skin_infos.colortype = bswap_32(skin_infos.colortype);
  skin_infos.lcd_white = bswap_32(skin_infos.lcd_white);
  skin_infos.lcd_black = bswap_32(skin_infos.lcd_black);
  
  *calc = bswap_32(*calc);
  
  skin_infos.lcd_pos.top = bswap_32(skin_infos.lcd_pos.top);
  skin_infos.lcd_pos.left = bswap_32(skin_infos.lcd_pos.left);
  skin_infos.lcd_pos.bottom = bswap_32(skin_infos.lcd_pos.bottom);
  skin_infos.lcd_pos.right = bswap_32(skin_infos.lcd_pos.right);
  
  for (i = 0; i < 80; i++)
    {
      skin_infos.keys_pos[i].top = bswap_32(skin_infos.keys_pos[i].top);
      skin_infos.keys_pos[i].bottom = bswap_32(skin_infos.keys_pos[i].bottom);
      skin_infos.keys_pos[i].left = bswap_32(skin_infos.keys_pos[i].left);
      skin_infos.keys_pos[i].right = bswap_32(skin_infos.keys_pos[i].right);
    }
}
#endif



#ifndef __MACOSX__
void
sbar_print(char *fmt, ...)
{
  char message[2048];

  va_list ap;

  va_start(ap, fmt);
  vsprintf(message, fmt, ap);
  va_end(ap);

  gtk_statusbar_pop(GTK_STATUSBAR(statusbar), 1);
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), 1, message);
}


void
set_lcd_color(GtkWidget *area, uint32_t color)
{
  unsigned char buf[3 * 25 * 20];
  unsigned char r, g, b;
  int i, j;

  GdkGC *context;
  GdkPixmap *pic;

  gdk_rgb_init();

  pic = gdk_pixmap_new(area->window, 25, 20, -1);

  context = gdk_gc_new(pic);

  r = (color >> 16) & 0xff;
  g = (color >> 8) & 0xff;
  b = color & 0xff;

  for (i = 0; i < 25; i++)
    for (j = 0; j < 20; j++)
      {
	buf[3 * (25*j + i)] = r;
	buf[3 * (25*j + i) + 1] = g;
	buf[3 * (25*j + i) + 2] = b;
      }

  gdk_draw_rgb_image(pic, context, 0, 0, 25, 20, GDK_RGB_DITHER_NORMAL, buf, 3*25);

  gdk_window_set_back_pixmap(area->window, pic, FALSE);

  gdk_gc_destroy(context);
  gdk_pixmap_unref(pic);
  
  gdk_window_clear(area->window);
}
#endif /* !__MACOSX__ */
