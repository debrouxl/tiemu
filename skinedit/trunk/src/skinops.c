/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifndef __WIN32__
#include <byteswap.h>
#endif

#include <gtk/gtk.h>

#include "support.h"
#include "struct.h"
#include "main_intf.h"
#include "skinops.h"
#include "utils.h"



int
load_skin(const char *path)
{
  FILE *fp = NULL;
  char buf[17];
  int ret = 0;

  fp = fopen(path, "rb");

  if (fp == NULL)
    {
      return -1;
    }

  /*
   * Determine the type of skin being loaded
   */

  if (fread(buf, 16, 1, fp) < 1)
    {
      ret = -1;
      goto exit;
    }
  
  if (buf[0] == 'V') /* VTi skin, string is only 8 bytes long */
    buf[7] = 0;

  if (strncmp(buf, "VTIv2.1", 7) == 0)
    ret = load_skin_old_vti(fp);
  else if (strncmp(buf, "VTIv2.5", 7) == 0)
    ret = load_skin_vti(fp);
  else
    ret = load_skin_tiemu(fp);

exit:
  fclose(fp);

  if (ret != 0)
    return ret;

  set_calc_keymap();

  sbar_print(_("Loaded %s (image size : %d x %d, skin version : %s)"),
             g_basename(path), skin_infos.width, skin_infos.height, buf);

  skin_infos.skin_path = strdup(path);
  
  return ret;
}


int
load_skin_old_vti(FILE *fp)
{
  uint32_t calc;
  int i;
  int ret = -1;

  fseek(fp, 8, SEEK_SET);

  skin_infos.name = (char *)malloc(65);

  if (skin_infos.name != NULL)
    {
      memset(skin_infos.name, 0, 65);
      if (fread(skin_infos.name, 64, 1, fp) < 1) goto exit;

      if (strlen(skin_infos.name) == 0)
        {
          free(skin_infos.name);
          skin_infos.name = NULL;
        }
      else
        {
          skin_infos.name = realloc(skin_infos.name, strlen(skin_infos.name) + 1);
        }
    }

  if (fread(&calc, 4, 1, fp) < 1) goto exit;

  if (fread(&skin_infos.colortype, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_white, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_black, 4, 1, fp) < 1) goto exit;

  if (fread(&skin_infos.lcd_pos.left, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.top, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.right, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.bottom, 4, 1, fp) < 1) goto exit;

  for (i = 0; i < 80; i++)
    {
      if (fread(&skin_infos.keys_pos[i].left, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].top, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].right, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].bottom, 4, 1, fp) < 1) goto exit;
    }

#if G_BYTE_ORDER == G_BIG_ENDIAN
  /*
   * VTI skins are usually designed on a little endian architecture
   */
  
  byteswap_vti_skin(&calc);
#endif

  vti_calc_type_to_string(calc);

  skin_infos.type = SKIN_TYPE_OLD_VTI;

  fseek(fp, 1384, SEEK_SET);

  ret = load_image(fp);

exit:
  return ret;
}


int
load_skin_vti(FILE *fp)
{
  uint32_t calc;
  int i;
  int ret = -1;

  fseek(fp, 8, SEEK_SET);

  skin_infos.name = (char *)malloc(65);

  if (skin_infos.name != NULL)
    {
      memset(skin_infos.name, 0, 65);
      if (fread(skin_infos.name, 64, 1, fp) < 1) { free(skin_infos.name); goto exit; }

      if (strlen(skin_infos.name) == 0)
        {
          free(skin_infos.name);
          skin_infos.name = NULL;
        }
      else
        {
          skin_infos.name = realloc(skin_infos.name, strlen(skin_infos.name) + 1);
        }
    }

  skin_infos.author = (char *)malloc(65);

  if (skin_infos.author != NULL)
    {
      memset(skin_infos.author, 0, 65);
      if (fread(skin_infos.author, 64, 1, fp) < 1) goto exit;

      if (strlen(skin_infos.author) == 0)
        {
          free(skin_infos.author);
          skin_infos.author = NULL;
        }
      else
        {
          skin_infos.author = realloc(skin_infos.author, strlen(skin_infos.author) + 1);
        }
    }

  if (fread(&calc, 4, 1, fp) < 1) goto exit;

  if (fread(&skin_infos.colortype, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_white, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_black, 4, 1, fp) < 1) goto exit;

  if (fread(&skin_infos.lcd_pos.left, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.top, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.right, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.bottom, 4, 1, fp) < 1) goto exit;

  for (i = 0; i < 80; i++)
    {
      if (fread(&skin_infos.keys_pos[i].left, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].top, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].right, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].bottom, 4, 1, fp) < 1) goto exit;
    }

#if G_BYTE_ORDER == G_BIG_ENDIAN
  /*
   * VTI skins are usually designed on a little endian architecture
   */

  byteswap_vti_skin(&calc);
#endif
  
  vti_calc_type_to_string(calc);

  skin_infos.type = SKIN_TYPE_VTI;  

  fseek(fp, 1448, SEEK_SET);

  ret = load_image(fp);
exit:
  if (ret < 0)
    {
      free(skin_infos.name);
      free(skin_infos.author);
    }
  return ret;
}

int
load_skin_tiemu(FILE *fp)
{
  uint32_t i;
  uint32_t endian;
  uint32_t jpeg_offset;
  uint32_t length;
  int ret = -1;

  fseek(fp, 16, SEEK_SET);
  
  if (fread(&endian, 4, 1, fp) < 1) goto exit;

  if (fread(&jpeg_offset, 4, 1, fp) < 1) goto exit;

  if (endian != ENDIANNESS_FLAG)
    jpeg_offset = bswap_32(jpeg_offset);

  /*
   * Skin name
   */
  if (fread(&length, 4, 1, fp) < 1) goto exit;

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > 0)
    {
      skin_infos.name = (char *)malloc(length + 1);

      if (skin_infos.name == NULL)
        {
          return -1;
        }

      memset(skin_infos.name, 0, length + 1);

      if (fread(skin_infos.name, length, 1, fp) < 1) { free(skin_infos.name); goto exit; }
    }


  /*
   * Skin author
   */

  if (fread(&length, 4, 1, fp) < 1) goto exit;

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > 0)
    {
      skin_infos.author = (char *)malloc(length + 1);

      if (skin_infos.author == NULL)
      {
        return -1;
      }

      memset(skin_infos.author, 0, length + 1);

      if (fread(skin_infos.author, length, 1, fp) < 1) goto exit;
    }

  /*
   * LCD colors
   */

  if (fread(&skin_infos.colortype, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_white, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_black, 4, 1, fp) < 1) goto exit;

   /*
   * Calc type
   */

  if (fread(skin_infos.calc, 8, 1, fp) < 1) goto exit;

  /*
   * LCD position
   */

  if (fread(&skin_infos.lcd_pos.left, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.top, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.right, 4, 1, fp) < 1) goto exit;
  if (fread(&skin_infos.lcd_pos.bottom, 4, 1, fp) < 1) goto exit;

  /* number of RECT struct to read */
  if (fread(&length, 4, 1, fp) < 1) goto exit;

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > SKIN_KEYS)
    return -1;

  for (i = 0; i < length; i++)
    {
      if (fread(&skin_infos.keys_pos[i].left, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].top, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].right, 4, 1, fp) < 1) goto exit;
      if (fread(&skin_infos.keys_pos[i].bottom, 4, 1, fp) < 1) goto exit;
    }

  if (endian != ENDIANNESS_FLAG)
    {
      skin_infos.colortype = bswap_32(skin_infos.colortype);
      skin_infos.lcd_white = bswap_32(skin_infos.lcd_white);
      skin_infos.lcd_black = bswap_32(skin_infos.lcd_black);
      
      skin_infos.lcd_pos.top = bswap_32(skin_infos.lcd_pos.top);
      skin_infos.lcd_pos.left = bswap_32(skin_infos.lcd_pos.left);
      skin_infos.lcd_pos.bottom = bswap_32(skin_infos.lcd_pos.bottom);
      skin_infos.lcd_pos.right = bswap_32(skin_infos.lcd_pos.right);

      for (i = 0; i < length; i++)
        {
          skin_infos.keys_pos[i].top = bswap_32(skin_infos.keys_pos[i].top);
          skin_infos.keys_pos[i].bottom = bswap_32(skin_infos.keys_pos[i].bottom);
          skin_infos.keys_pos[i].left = bswap_32(skin_infos.keys_pos[i].left);
          skin_infos.keys_pos[i].right = bswap_32(skin_infos.keys_pos[i].right);
        }
    }

  skin_infos.type = SKIN_TYPE_TIEMU;

  fseek(fp, jpeg_offset, SEEK_SET);
  
  ret = load_image(fp);
exit:
  if (ret < 0)
    {
      free(skin_infos.name);
      free(skin_infos.author);
    }
  return ret;
}

GtkWidget *main_wnd;

int
load_image(FILE *fp)
{
  GdkPixbufLoader *loader;
  GError *error = NULL;

  /*
   * Extract image from skin
   */
  loader = gdk_pixbuf_loader_new();
  while(!feof(fp))
    {
      int c = fgetc(fp);
      if (c != EOF)
        {
          unsigned char buf = c;
          if(!gdk_pixbuf_loader_write(loader, &buf, 1, &error))
            {
              fprintf(stderr, "Failed to load pixbuf from skin: %s\n", error->message);
              g_error_free(error);
              return -1;
            }
        }
    }
  if(!gdk_pixbuf_loader_close(loader, &error))
    {
      fprintf(stderr, "Failed to load pixbuf from skin: %s\n", error->message);
      g_error_free(error);
      return -1;
    }

  /*
   * Destroy previous pixbuf
   */
  if(pixbuf != NULL)
    {
      g_object_unref(pixbuf);
      pixbuf = NULL;
    }

  /*
   * Feed the original pixbuf with our image
   */
  skin_infos.img_orig = gdk_pixbuf_loader_get_pixbuf(loader);
  if (skin_infos.img_orig == NULL) 
    {
      fprintf(stderr, "Failed to load pixbuf from skin: %s\n", error->message);
      g_error_free(error);
      return -1;
    }

  /*
   * Set image and drawing area sizes
   */
  skin_infos.width = gdk_pixbuf_get_width(skin_infos.img_orig);
  skin_infos.height = gdk_pixbuf_get_height(skin_infos.img_orig);

  gtk_drawing_area_size(GTK_DRAWING_AREA(drawingarea1), skin_infos.width, skin_infos.height);
/*
  geometry.min_width = -1;
  geometry.min_height = -1;
  geometry.max_width = -1;    //skin_infos.width;
  geometry.max_height = -1; //skin_infos.height;
  geometry.base_height = -1;
  geometry.base_width = -1;
  
  gtk_window_set_geometry_hints(GTK_WINDOW(main_wnd), GTK_WIDGET(drawingarea1), &geometry, GDK_HINT_MAX_SIZE);
*/

  /*
   * Display image in the back-end pixbuf
   */
  pixbuf = gdk_pixbuf_copy(skin_infos.img_orig);
  //gtk_widget_draw (GTK_DRAWING_AREA(drawingarea1), (GdkRectangle *)&update_rect);

  return 0;
}


static int
write_header(FILE *fp)
{
  uint32_t endian = ENDIANNESS_FLAG;
  uint32_t jpeg_offset;
  uint32_t length;
  int i;
  unsigned char id[16] = TIEMU_SKIN_ID;
  int ret = -1;

  if (fwrite(id, 16, 1, fp) < 1) goto exit;

  if (fwrite(&endian, 4, 1, fp) < 1) goto exit;

  /* write the jpeg_offset, reserving 4 bytes */
  if (fwrite(&jpeg_offset, 4, 1, fp) < 1) goto exit;


  if (skin_infos.name != NULL)
    length = strlen(skin_infos.name);
  else
    length = 0;

  if (fwrite(&length, 4, 1, fp) < 1) goto exit;
  
  if (length > 0)
    if (fwrite(skin_infos.name, length, 1, fp) < 1) goto exit;
  

  if (skin_infos.author != NULL)
    length = strlen(skin_infos.author);
  else
    length = 0;

  if (fwrite(&length, 4, 1, fp) < 1) goto exit;

  if (length > 0)
    if (fwrite(skin_infos.author, length, 1, fp) < 1) goto exit;


  if (fwrite(&skin_infos.colortype, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_white, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_black, 4, 1, fp) < 1) goto exit;

  if (fwrite(skin_infos.calc, 8, 1, fp) < 1) goto exit;

  if (fwrite(&skin_infos.lcd_pos.left, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_pos.top, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_pos.right, 4, 1, fp) < 1) goto exit;
  if (fwrite(&skin_infos.lcd_pos.bottom, 4, 1, fp) < 1) goto exit;

  /* write the number of RECT structs */
  length = SKIN_KEYS;
  if (fwrite(&length, 4, 1, fp) < 1) goto exit;

  for (i = 0; i < SKIN_KEYS; i++)
    {
      if (fwrite(&skin_infos.keys_pos[i].left, 4, 1, fp) < 1) goto exit;
      if (fwrite(&skin_infos.keys_pos[i].top, 4, 1, fp) < 1) goto exit;
      if (fwrite(&skin_infos.keys_pos[i].right, 4, 1, fp) < 1) goto exit;
      if (fwrite(&skin_infos.keys_pos[i].bottom, 4, 1, fp) < 1) goto exit;
    }

  /* get the current position */
  jpeg_offset = ftell(fp);

  /* go back to the jpeg_offset location */
  fseek(fp, 20, SEEK_SET);

  if (fwrite(&jpeg_offset, 4, 1, fp) < 1) goto exit;

  /* back to end of file */
  fseek(fp, jpeg_offset, SEEK_SET);

  ret = 0;

exit:
  return ret;
}


int
write_skin(void)
{
  FILE *fp = NULL;
  FILE *jpeg = NULL;
  unsigned char *jpeg_data = NULL;
  unsigned int jpeg_length;
  int ret = -1;

  /*
   * write the skin to skin_infos.skin_path
   */

  switch(skin_infos.type)
    {
      case SKIN_TYPE_NEW:
        fp = fopen(skin_infos.skin_path, "wb");

        if (fp != NULL)
          {
            jpeg = fopen(skin_infos.jpeg_path, "rb");
            if (jpeg == NULL)
              {
                fclose(fp);
                return -1;
              }

            if (write_header(fp) < 0) goto exit1;

            jpeg_data = read_image(jpeg, &jpeg_length);

            if (jpeg_data == NULL)
              {
                fclose(jpeg);
                fclose(fp);
                return -1;
              }

            if (fwrite(jpeg_data, jpeg_length, 1, fp) < 1) goto exit1;

            ret = 0;

exit1:
            free(jpeg_data);

            fclose(fp);
            fclose(jpeg);
            skin_infos.type = SKIN_TYPE_TIEMU;
          }
        else
          {
            return -1;
          }
        break;

      case SKIN_TYPE_TIEMU:
        fp = fopen(skin_infos.skin_path, "rb");

        if (fp != NULL)
          {
            jpeg_data = read_image(fp, &jpeg_length);

            if (jpeg_data == NULL)
              {
                fclose(fp);
                return -1;
              }

            fp = freopen(skin_infos.skin_path, "wb", fp);

            if (fp == NULL)
              {
                return -1;
              }

            if (write_header(fp) < 0) goto exit2;

            if (fwrite(jpeg_data, jpeg_length, 1, fp) < 1) goto exit2;

            ret = 0;

exit2:
            free(jpeg_data);

            fclose(fp);
          }
        else
          {
            return -1;
          }
        break;

      case SKIN_TYPE_VTI:
      case SKIN_TYPE_OLD_VTI:
        fp = fopen(skin_infos.skin_path, "rb");
     
        if (fp != NULL)
          {
            if (skin_infos.type == SKIN_TYPE_VTI)
              {
                fseek(fp, 1448, SEEK_SET);
              }
            else
              {
                fseek(fp, 1384, SEEK_SET);
              }
         
            jpeg_data = read_image(fp, &jpeg_length);

            if (jpeg_data == NULL)
              {
                fclose(fp);
                return -1;
              }

            fp = freopen(skin_infos.skin_path, "wb", fp);

            if (fp == NULL)
              {
                return -1;
              }

            if (write_header(fp) < 0) goto exit3;

            if (fwrite(jpeg_data, jpeg_length, 1, fp) < 1) goto exit3;

            ret = 0;

exit3:
            fclose(fp);
            free(jpeg_data);

            skin_infos.type = SKIN_TYPE_TIEMU;
          }
        else
          {
            return -1;
          }
        break;

      default: /* should not get there */
        return -1;
    }

  if (ret != -1)
    {
      sbar_print(_("Skin saved (%s)"), g_basename(skin_infos.skin_path));
    }

  return ret;
}


int
write_skin_as(const char *dest)
{
  FILE *fp = NULL;
  FILE *jpeg = NULL;
  unsigned char *jpeg_data = NULL;
  unsigned int jpeg_length;
  int ret = -1;

  if ((skin_infos.skin_path != NULL) && (strcmp(dest, skin_infos.skin_path) == 0))
    return write_skin();

  switch(skin_infos.type)
    {
      case SKIN_TYPE_NEW:
        skin_infos.skin_path = strdup(dest);
        return write_skin();

      case SKIN_TYPE_TIEMU:
      case SKIN_TYPE_VTI:
      case SKIN_TYPE_OLD_VTI:
        fp = fopen(dest, "wb");

        if (fp != NULL)
          {
            jpeg = fopen(skin_infos.skin_path, "rb");

            if (jpeg == NULL)
              {
                fclose(fp);
                return -1;
              }

            jpeg_data = read_image(jpeg, &jpeg_length);

            if (jpeg_data == NULL)
              {
                fclose(fp);
                fclose(jpeg);
                return -1;
              }

            if (write_header(fp) < 0) goto exit;

            if (fwrite(jpeg_data, jpeg_length, 1, fp) < 1) goto exit;

            ret = 0;
exit:
            fclose(fp);
            fclose(jpeg);
            free(jpeg_data);

            if (skin_infos.skin_path != NULL)
              {
                free(skin_infos.skin_path);
              }

            skin_infos.skin_path = strdup(dest);
            skin_infos.type = SKIN_TYPE_TIEMU;
          }
        else
          {
            return -1;
          }
        break;

      default: /* should not get there */
        return -1;
    }

  if (ret != -1)
    {
      sbar_print(_("Skin saved (%s)"), g_basename(dest));
    }

  return ret;
}


unsigned char *
read_image(FILE *fp, unsigned int *length)
{
  char *buf = NULL;
  char *data = NULL;
  unsigned int l = 0;

  uint32_t endian;
  uint32_t offset;

  *length = 0;

  /*
   * differentiate vti/oldvti/tiemu/raw jpeg
   */

  buf = (char *)malloc(16);
  if (buf == NULL)
    return NULL;
  if (fread(buf, 16, 1, fp) < 1) { free(buf); return NULL; }

  if (strncmp(buf, "VTIv2.1", 7) == 0)
    fseek(fp, 1384, SEEK_SET);
  else if (strncmp(buf, "VTIv2.5", 7) == 0)
    fseek(fp, 1448, SEEK_SET);
  else if (strncmp(buf, "TiEmu v2", 8) == 0)
  {
    fseek(fp, 16, SEEK_SET);

    if (fread(&endian, 4, 1, fp) < 1) { free(buf); return NULL; }

    if (fread(&offset, 4, 1, fp) < 1) { free(buf); return NULL; }

    if (endian != ENDIANNESS_FLAG)
      offset = bswap_32(offset);

    fseek(fp, offset, SEEK_SET);
  }
  else
    fseek(fp, 0, SEEK_SET);

  free(buf);
  buf = NULL;

  while (feof(fp) == 0)
    {
      buf = realloc(data, *length + 2048);
      
      if (buf == NULL)
    {
      if (data != NULL)
        free(data);
      
      return NULL;
    }
      
      data = buf;
      buf = NULL;
      
      l = fread(data+(*length), 1, 2048, fp);
      
      *length += l;
    }

  return (unsigned char *)data;
}
