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
#include <byteswap.h>

#include <SDL/SDL.h>
#include <gtk/gtk.h>
#include "gtksdl.h"
#include <jpeglib.h>

#include "support.h"
#include "struct.h"

#include "main_intf.h"

#include "skinops.h"
#include "utils.h"


extern struct skinInfos skin_infos;


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

  fread(buf, 16, 1, fp);
  
  if (buf[0] == 'V') /* VTi skin, string is only 8 bytes long */
    buf[7] = 0;

  if (strncmp(buf, "VTIv2.1", 7) == 0)
    ret = load_skin_old_vti(fp);
  else if (strncmp(buf, "VTIv2.5", 7) == 0)
    ret = load_skin_vti(fp);
  else
    ret = load_skin_tiemu(fp);

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

  fseek(fp, 8, SEEK_SET);

  skin_infos.name = (char *)malloc(65);

  if (skin_infos.name != NULL)
    {
      memset(skin_infos.name, 0, 65);
      fread(skin_infos.name, 64, 1, fp);

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

  fread(&calc, 4, 1, fp);

  fread(&skin_infos.colortype, 4, 1, fp);
  fread(&skin_infos.lcd_white, 4, 1, fp);
  fread(&skin_infos.lcd_black, 4, 1, fp);

  fread(&skin_infos.lcd_pos.left, 4, 1, fp);
  fread(&skin_infos.lcd_pos.top, 4, 1, fp);
  fread(&skin_infos.lcd_pos.right, 4, 1, fp);
  fread(&skin_infos.lcd_pos.bottom, 4, 1, fp);

  for (i = 0; i < 80; i++)
    {
      fread(&skin_infos.keys_pos[i].left, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].top, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].right, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].bottom, 4, 1, fp);
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

  return load_jpeg(fp);
}


int
load_skin_vti(FILE *fp)
{
  uint32_t calc;
  int i;

  fseek(fp, 8, SEEK_SET);

  skin_infos.name = (char *)malloc(65);

  if (skin_infos.name != NULL)
    {
      memset(skin_infos.name, 0, 65);
      fread(skin_infos.name, 64, 1, fp);

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
      fread(skin_infos.author, 64, 1, fp);

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

  fread(&calc, 4, 1, fp);

  fread(&skin_infos.colortype, 4, 1, fp);
  fread(&skin_infos.lcd_white, 4, 1, fp);
  fread(&skin_infos.lcd_black, 4, 1, fp);

  fread(&skin_infos.lcd_pos.left, 4, 1, fp);
  fread(&skin_infos.lcd_pos.top, 4, 1, fp);
  fread(&skin_infos.lcd_pos.right, 4, 1, fp);
  fread(&skin_infos.lcd_pos.bottom, 4, 1, fp);

  for (i = 0; i < 80; i++)
    {
      fread(&skin_infos.keys_pos[i].left, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].top, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].right, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].bottom, 4, 1, fp);
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

  return load_jpeg(fp);
}

int
load_skin_tiemu(FILE *fp)
{
  int i;
  uint32_t endian;
  uint32_t jpeg_offset;
  uint32_t length;

  fseek(fp, 16, SEEK_SET);
  
  fread(&endian, 4, 1, fp);

  fread(&jpeg_offset, 4, 1, fp);

  if (endian != ENDIANNESS_FLAG)
    jpeg_offset = bswap_32(jpeg_offset);


  fread(skin_infos.calc, 8, 1, fp);

  fread(&skin_infos.colortype, 4, 1, fp);
  fread(&skin_infos.lcd_white, 4, 1, fp);
  fread(&skin_infos.lcd_black, 4, 1, fp);


  /*
   * Skin name
   */
  fread(&length, 4, 1, fp);

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > 0)
    {
      skin_infos.name = (char *)malloc(length + 1);

      if (skin_infos.name == NULL)
	return -1;

      memset(skin_infos.name, 0, length + 1);

      fread(skin_infos.name, length, 1, fp);
    }


  /*
   * Skin author
   */

  fread(&length, 4, 1, fp);

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > 0)
    {
      skin_infos.author = (char *)malloc(length + 1);

      if (skin_infos.author == NULL)
	return -1;

      memset(skin_infos.author, 0, length + 1);

      fread(skin_infos.author, length, 1, fp);
    }

  fread(&skin_infos.lcd_pos.left, 4, 1, fp);
  fread(&skin_infos.lcd_pos.top, 4, 1, fp);
  fread(&skin_infos.lcd_pos.right, 4, 1, fp);
  fread(&skin_infos.lcd_pos.bottom, 4, 1, fp);

  /* number of RECT struct to read */
  fread(&length, 4, 1, fp);

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > SKIN_KEYS)
    return -1;

  for (i = 0; i < length; i++)
    {
      fread(&skin_infos.keys_pos[i].left, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].top, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].right, 4, 1, fp);
      fread(&skin_infos.keys_pos[i].bottom, 4, 1, fp);
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
  
  return load_jpeg(fp);
}

int
load_jpeg(FILE *fp)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  unsigned char *img, *img_ptr;
  int i, j;

  uint16_t *SDLpixels;
  unsigned char r, g, b;

  int ret = 0;


  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, fp);
  jpeg_read_header(&cinfo, TRUE);
  
  cinfo.scale_num = 1;
  cinfo.scale_denom = 1;

  /*
   * Colormapped output, only 128 colors
   */

  cinfo.quantize_colors = TRUE;
  cinfo.desired_number_of_colors = 128;
  
  jpeg_start_decompress(&cinfo);
  
  if (cinfo.output_components != 1) /* 1: palettized */ 
    return -1;

  /*
   * JPEG rounds down odd number to the lower even.
   */
  skin_infos.width = cinfo.output_width & 0xfffffffe;
  skin_infos.height = cinfo.output_height & 0xfffffffe;

  img = img_ptr = (unsigned char*) malloc(cinfo.output_width * cinfo.output_height);

  if(img == NULL)
    {
      jpeg_destroy_decompress(&cinfo);
      
      return -1;
    }

  /*
   * Load the JPEG
   */

  while(cinfo.output_scanline < cinfo.output_height)
    {
      img_ptr += skin_infos.width;
      jpeg_read_scanlines(&cinfo, &img_ptr, 1);
    }

  /* 
   * Initialise the SDL library
   */
  
  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr, _("Unable to init SDL: %s"), SDL_GetError());

      jpeg_finish_decompress(&cinfo);
      jpeg_destroy_decompress(&cinfo);
      free(img);

      return -1;
    }

  /*
   * Place a gtksdl in the eventbox
   */

  area = gtk_sdl_new(skin_infos.width, skin_infos.height, 16, SDL_HWSURFACE | SDL_HWPALETTE);
  sdl_area = GTK_SDL(area);
  
  gtk_container_add(GTK_CONTAINER(sdl_eventbox), GTK_WIDGET(area));
  gtk_widget_show(GTK_WIDGET(area));

  /*
   * gtksdl needs some time to be set up...
   */

  while (sdl_area->surface == NULL)
    {
      gtk_main_iteration();
    }

  /*
   * Feed the gtksdl with our jpg
   */
  
  if(SDL_MUSTLOCK(sdl_area->surface)) 
    {
      if(SDL_LockSurface(sdl_area->surface) < 0)
	return -1;
    }

  SDLpixels = (uint16_t *)(sdl_area->surface)->pixels;

  for (j = 0; j < skin_infos.height; j++)
    for (i = 0; i < skin_infos.width; i++)
      {
	r = cinfo.colormap[0][img[j * skin_infos.width + i]];
	g = cinfo.colormap[1][img[j * skin_infos.width + i]];
	b = cinfo.colormap[2][img[j * skin_infos.width + i]];
	
	*SDLpixels++ = SDL_MapRGB((sdl_area->surface)->format, r, g, b);
      }
  
  if (SDL_MUSTLOCK(sdl_area->surface)) 
    {
      SDL_UnlockSurface(sdl_area->surface);
    }


  skin_infos.img_orig = malloc(2 * (sdl_area->surface)->w * (sdl_area->surface)->h);

  if (skin_infos.img_orig != NULL)
    {
      memcpy(skin_infos.img_orig, (sdl_area->surface)->pixels, (2 * (sdl_area->surface)->w * (sdl_area->surface)->h));
      
      SDL_UpdateRect(sdl_area->surface, 0, 0, 0, 0);
    }
  else
    {
      gtk_widget_destroy(area);
      sdl_area = NULL;
      SDL_Quit();

      ret = -1;
    }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  free(img);

  return ret;
}


int
write_skin(void)
{
  FILE *fp = NULL;
  FILE *jpeg = NULL;
  unsigned char *jpeg_data = NULL;
  unsigned int jpeg_length;

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

	     write_header(fp);
	     
	     jpeg_data = read_jpeg(jpeg, &jpeg_length);

	     if (jpeg_data == NULL)
	       {
		 fclose(jpeg);
		 fclose(fp);
		 
		 return -1;
	       }

	     fwrite(jpeg_data, jpeg_length, 1, fp);

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

	     jpeg_data = read_jpeg(fp, &jpeg_length);

	     if (jpeg_data == NULL)
	       {
		 fclose(fp);

		 return -1;
	       }

	     fp = freopen(skin_infos.skin_path, "wb", fp);
	     
	     if (fp == NULL)
	       return -1;

	     write_header(fp);
	     
	     fwrite(jpeg_data, jpeg_length, 1, fp);

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
	       fseek(fp, 1448, SEEK_SET);
	     else
	       fseek(fp, 1384, SEEK_SET);
	     
	     jpeg_data = read_jpeg(fp, &jpeg_length);

	     if (jpeg_data == NULL)
	       {
		 fclose(fp);

		 return -1;
	       }
	     
	     fp = freopen(skin_infos.skin_path, "wb", fp);
	     
	     if (fp == NULL)
	       return -1;
	     
	     write_header(fp);
	     
	     fwrite(jpeg_data, jpeg_length, 1, fp);
	     
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

  sbar_print(_("Skin saved (%s)"), g_basename(skin_infos.skin_path));

  return 0;
}


int
write_skin_as(const char *dest)
{
  FILE *fp = NULL;
  FILE *jpeg = NULL;
  unsigned char *jpeg_data = NULL;
  unsigned int jpeg_length;

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

	     jpeg_data = read_jpeg(jpeg, &jpeg_length);

	     if (jpeg_data == NULL)
	       {
		 fclose(fp);
		 fclose(jpeg);

		 return -1;
	       }

	     write_header(fp);

	     fwrite(jpeg_data, jpeg_length, 1, fp);

	     fclose(fp);
	     fclose(jpeg);
	     free(jpeg_data);
	     
	     if (skin_infos.skin_path != NULL)
	       free(skin_infos.skin_path);

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

  sbar_print(_("Skin saved (%s)"), g_basename(dest));

  return 0;
}


void
write_header(FILE *fp)
{
  uint32_t endian = ENDIANNESS_FLAG;
  uint32_t jpeg_offset;
  uint32_t length;
  int i;
  unsigned char id[16] = TIEMU_SKIN_ID;

  fwrite(id, 16, 1, fp);

  fwrite(&endian, 4, 1, fp);  

  /* write the jpeg_offset, reserving 4 bytes */

  fwrite(&jpeg_offset, 4, 1, fp);

  fwrite(skin_infos.calc, 8, 1, fp);

  fwrite(&skin_infos.colortype, 4, 1, fp);
  fwrite(&skin_infos.lcd_white, 4, 1, fp);
  fwrite(&skin_infos.lcd_black, 4, 1, fp);


  if (skin_infos.name != NULL)
    length = strlen(skin_infos.name);
  else
    length = 0;

  fwrite(&length, 4, 1, fp);
  
  if (length > 0)
    fwrite(skin_infos.name, length, 1, fp);
  

  if (skin_infos.author != NULL)
    length = strlen(skin_infos.author);
  else
    length = 0;

  fwrite(&length, 4, 1, fp);
  
  if (length > 0)
    fwrite(skin_infos.author, length, 1, fp);


  fwrite(&skin_infos.lcd_pos.left, 4, 1, fp);
  fwrite(&skin_infos.lcd_pos.top, 4, 1, fp);
  fwrite(&skin_infos.lcd_pos.right, 4, 1, fp);
  fwrite(&skin_infos.lcd_pos.bottom, 4, 1, fp);

  /* write the number of RECT structs */
  length = SKIN_KEYS;
  fwrite(&length, 4, 1, fp);

  for (i = 0; i < SKIN_KEYS; i++)
    {
      fwrite(&skin_infos.keys_pos[i].left, 4, 1, fp);
      fwrite(&skin_infos.keys_pos[i].top, 4, 1, fp);
      fwrite(&skin_infos.keys_pos[i].right, 4, 1, fp);
      fwrite(&skin_infos.keys_pos[i].bottom, 4, 1, fp);
    }

  /* get the current position */
  jpeg_offset = ftell(fp);

  /* go back to the jpeg_offset location */
  fseek(fp, 20, SEEK_SET);

  fwrite(&jpeg_offset, 4, 1, fp);

  /* back to end of file */
  fseek(fp, jpeg_offset, SEEK_SET);
}


unsigned char *
read_jpeg(FILE *fp, unsigned int *length)
{
  unsigned char *buf = NULL;
  unsigned char *data = NULL;
  unsigned int l = 0;

  uint32_t endian;
  uint32_t offset;

  *length = 0;

  /*
   * differentiate vti/oldvti/tiemu/raw jpeg
   */

  buf = (char *)malloc(16);
  fread(buf, 16, 1, fp);

  if (strncmp(buf, "VTIv2.1", 7) == 0)
    fseek(fp, 1384, SEEK_SET);
  else if (strncmp(buf, "VTIv2.5", 7) == 0)
    fseek(fp, 1448, SEEK_SET);
  else if (strncmp(buf, "TiEmu v2", 8) == 0)
  {
    fseek(fp, 16, SEEK_SET);

    fread(&endian, 4, 1, fp);

    fread(&offset, 4, 1, fp);

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

  return data;
}
