/*  TiEmu - a TI emulator
 *  loader.c: loader for VTi & TiEmu skins
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
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
#include <config.h>
#endif

#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#ifndef __WIN32__
#include <stdint.h>
#include <byteswap.h>
#endif

#include <jpeglib.h>

#include "skin.h"
#include "paths.h"
#include "tilibs.h"
#include "interface.h"
#include "struct.h"
#include "./defs.h"


/* Keep information on currently loaded skin */
//skinInfos skin_infos = { 0 };
skinInfos skin = { 0 };
static int skin_loaded = 0;

/****************/
/* Internal use */
/****************/

/* Load a JPEG file from stream 'fp' */
static int load_jpeg(FILE *fp, skinInfos *skin)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    unsigned char *img;
    int j;

    // Init JPEG
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, TRUE);

    // Rescale image to half if necessary
    if ((cinfo.image_width > 600)
	|| (cinfo.image_height > 600)) {
	cinfo.scale_num = 1;
	cinfo.scale_denom = 2;
    }

    // Require a colormapped output with a limited number of colors
    cinfo.quantize_colors = TRUE;
    cinfo.desired_number_of_colors = MAX_COLORS;

    // Decompress JPEG
    jpeg_start_decompress(&cinfo);

    if (cinfo.output_components != 1)	// 1: palettized 
      return -1;

    // Copy the color palette
    skin->ncolors = cinfo.actual_number_of_colors;
    for (j = 0; j < cinfo.actual_number_of_colors; j++) {
	skin->cmap[0][j] = cinfo.colormap[0][j];
	skin->cmap[1][j] = cinfo.colormap[1][j];
	skin->cmap[2][j] = cinfo.colormap[2][j];
    }

    // Get skin size (JPEG rounds down odd number to the lower even)
    skin->width = cinfo.output_width + (cinfo.output_width & 3);
    skin->height = cinfo.output_height + (cinfo.output_height & 3);

    // Allocate image
    skin->img = img = (unsigned char *) malloc(skin->width * skin->height);
    if (img == NULL)
      {
	jpeg_destroy_decompress(&cinfo);
	return -1;
      }

    // Load jpeg image line per line
    while (cinfo.output_scanline < cinfo.output_height) 
      {
	img += skin->width;
	jpeg_read_scanlines(&cinfo, &img, 1);
      }
    
    // Close JPEG
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 0;
}


void
vti_calc_type_to_string(uint32_t calc, skinInfos *skin)
{
  switch(calc)
    {
      case 73:
	strcpy(skin->calc, SKIN_TI73);
	break;
      case 82:
	strcpy(skin->calc, SKIN_TI82);
	break;
      case 83:
	strcpy(skin->calc, SKIN_TI83);
	break;
      case 84:
	strcpy(skin->calc, SKIN_TI83P);
	break;
      case 85:
	strcpy(skin->calc, SKIN_TI85);
	break;
      case 86:
	strcpy(skin->calc, SKIN_TI86);
	break;
      case 89:
	strcpy(skin->calc, SKIN_TI89);
	break;
      case 92:
	strcpy(skin->calc, SKIN_TI92);
	break;
      case 94:
	strcpy(skin->calc, SKIN_TI92P);
	break;
      default: /* should not get there */
	break;
    }
}


#ifndef bswap_32
uint32_t
bswap_32(uint32_t a)
{
  return (a >> 24) | ((a & 0xff0000) >> 16) << 8 | ((a & 0xff00) >> 8) << 16 | (a & 0xff) << 8;
}
#endif



#if G_BYTE_ORDER == G_BIG_ENDIAN
void
byteswap_vti_skin(uint32_t *calc, skinInfos *skin)
{
  int i;

  skin->colortype = bswap_32(skin->colortype);
  skin->lcd_white = bswap_32(skin->lcd_white);
  skin->lcd_black = bswap_32(skin->lcd_black);
  
  *calc = bswap_32(*calc);
  
  skin->lcd_pos.top = bswap_32(skin->lcd_pos.top);
  skin->lcd_pos.left = bswap_32(skin->lcd_pos.left);
  skin->lcd_pos.bottom = bswap_32(skin->lcd_pos.bottom);
  skin->lcd_pos.right = bswap_32(skin->lcd_pos.right);
  
  for (i = 0; i < 80; i++)
    {
      skin->keys_pos[i].top = bswap_32(skin->keys_pos[i].top);
      skin->keys_pos[i].bottom = bswap_32(skin->keys_pos[i].bottom);
      skin->keys_pos[i].left = bswap_32(skin->keys_pos[i].left);
      skin->keys_pos[i].right = bswap_32(skin->keys_pos[i].right);
    }
}
#endif


int
load_skin_old_vti(FILE *fp, skinInfos *skin)
{
  uint32_t calc;
  int i;

  fseek(fp, 8, SEEK_SET);

  skin->name = (char *)malloc(65);

  if (skin->name != NULL)
    {
      memset(skin->name, 0, 65);
      fread(skin->name, 64, 1, fp);

      if (strlen(skin->name) == 0)
	{
	  free(skin->name);
	  skin->name = NULL;
	}
      else
	{
	  skin->name = realloc(skin->name, strlen(skin->name) + 1);
	}
    }

  fread(&calc, 4, 1, fp);

  fread(&skin->colortype, 4, 1, fp);
  fread(&skin->lcd_white, 4, 1, fp);
  fread(&skin->lcd_black, 4, 1, fp);

  fread(&skin->lcd_pos, 16, 1, fp);
  
  fread(skin->keys_pos, 80*16, 1, fp);

#if G_BYTE_ORDER == G_BIG_ENDIAN
  /*
   * VTI skins are usually designed on a little endian architecture
   */
  
  byteswap_vti_skin(&calc, skin);
#endif

  // added by roms
  for (i = 0; i < 80; i++)
    {
      skin->keys_pos[i].top >>= 1;
      skin->keys_pos[i].bottom >>= 1;
      skin->keys_pos[i].left >>= 1;
      skin->keys_pos[i].right >>= 1;
    }
  skin->lcd_pos.top >>= 1;
  skin->lcd_pos.bottom >>= 1;
  skin->lcd_pos.left >>= 1;
  skin->lcd_pos.right >>= 1;
  
  vti_calc_type_to_string(calc, skin);

  skin->type = SKIN_TYPE_OLD_VTI;

  fseek(fp, 1384, SEEK_SET);

  return load_jpeg(fp, skin);
}


int
load_skin_vti(FILE *fp, skinInfos *skin)
{
  uint32_t calc;
  int i;

  fseek(fp, 8, SEEK_SET);

  skin->name = (char *)malloc(65);

  if (skin->name != NULL)
    {
      memset(skin->name, 0, 65);
      fread(skin->name, 64, 1, fp);

      if (strlen(skin->name) == 0)
	{
	  free(skin->name);
	  skin->name = NULL;
	}
      else
	{
	  skin->name = realloc(skin->name, strlen(skin->name) + 1);
	}
    }

  skin->author = (char *)malloc(65);

  if (skin->author != NULL)
    {
      memset(skin->author, 0, 65);
      fread(skin->author, 64, 1, fp);

      if (strlen(skin->author) == 0)
	{
	  free(skin->author);
	  skin->author = NULL;
	}
      else
	{
	  skin->author = realloc(skin->author, strlen(skin->author) + 1);
	}
    }

  fread(&calc, 4, 1, fp);

  fread(&skin->colortype, 4, 1, fp);
  fread(&skin->lcd_white, 4, 1, fp);
  fread(&skin->lcd_black, 4, 1, fp);

  fread(&skin->lcd_pos, 16, 1, fp);
  
  fread(skin->keys_pos, 80*16, 1, fp);

#if G_BYTE_ORDER == G_BIG_ENDIAN
  /*
   * VTI skins are usually designed on a little endian architecture
   */

  byteswap_vti_skin(&calc, skin);
#endif

  // added by roms
  for (i = 0; i < 80; i++)
    {
      skin->keys_pos[i].top >>= 1;
      skin->keys_pos[i].bottom >>= 1;
      skin->keys_pos[i].left >>= 1;
      skin->keys_pos[i].right >>= 1;
    }
  skin->lcd_pos.top >>= 1;
  skin->lcd_pos.bottom >>= 1;
  skin->lcd_pos.left >>= 1;
  skin->lcd_pos.right >>= 1;
  
  vti_calc_type_to_string(calc, skin);

  skin->type = SKIN_TYPE_VTI;  

  fseek(fp, 1448, SEEK_SET);

  return load_jpeg(fp, skin);
}

int
load_skin_tiemu(FILE *fp, skinInfos *skin)
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


  fread(skin->calc, 8, 1, fp);

  fread(&skin->colortype, 4, 1, fp);
  fread(&skin->lcd_white, 4, 1, fp);
  fread(&skin->lcd_black, 4, 1, fp);


  /*
   * Skin name
   */
  fread(&length, 4, 1, fp);

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > 0)
    {
      skin->name = (char *)malloc(length + 1);

      if (skin->name == NULL)
	return -1;

      memset(skin->name, 0, length + 1);

      fread(skin->name, length, 1, fp);
    }


  /*
   * Skin author
   */

  fread(&length, 4, 1, fp);

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > 0)
    {
      skin->author = (char *)malloc(length + 1);

      if (skin->author == NULL)
	return -1;

      memset(skin->author, 0, length + 1);

      fread(skin->author, length, 1, fp);
    }


  fread(&skin->lcd_pos, 16, 1, fp);

  /* number of RECT struct to read */
  fread(&length, 4, 1, fp);

  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > SKIN_KEYS)
    return -1;

  fread(skin->keys_pos, length*16, 1, fp);

  if (endian != ENDIANNESS_FLAG)
    {
      skin->colortype = bswap_32(skin->colortype);
      skin->lcd_white = bswap_32(skin->lcd_white);
      skin->lcd_black = bswap_32(skin->lcd_black);
      
      skin->lcd_pos.top = bswap_32(skin->lcd_pos.top);
      skin->lcd_pos.left = bswap_32(skin->lcd_pos.left);
      skin->lcd_pos.bottom = bswap_32(skin->lcd_pos.bottom);
      skin->lcd_pos.right = bswap_32(skin->lcd_pos.right);

      for (i = 0; i < 80; i++)
	{
	  skin->keys_pos[i].top = bswap_32(skin->keys_pos[i].top);
	  skin->keys_pos[i].bottom = bswap_32(skin->keys_pos[i].bottom);
	  skin->keys_pos[i].left = bswap_32(skin->keys_pos[i].left);
	  skin->keys_pos[i].right = bswap_32(skin->keys_pos[i].right);
	}
    }

  skin->type = SKIN_TYPE_TIEMU;

  fseek(fp, jpeg_offset, SEEK_SET);
  
  return load_jpeg(fp, skin);
}


/***********/
/* Exports */
/***********/


/* Unload skin by freeing allocated memory */
int skin_unload(void)
{
  if (skin_loaded == FALSE)
    return -1;
  else
    skin_loaded = FALSE;
  
  free(skin.img);
  free(skin.name);
  free(skin.author);

  memset(&skin, 0, sizeof(skinInfos));
  
  return 0;
}

/* Load a skin (automatically detects the format and version) */
int skin_load(const char *filename)
{
  FILE *fp = NULL;
  char buf[17];
  int ret = 0;

  fp = fopen(filename, "rb");
  if (fp == NULL)
    {
      DISPLAY("Unable to open this file: <%s>\n", filename);
      return -1;
    }

  /*
   * Determine the type of skin being loaded
   */

  fread(buf, 16, 1, fp);
  
  if (buf[0] == 'V') /* VTi skin, string is only 8 bytes long */
    buf[7] = 0;

  if (strncmp(buf, "VTIv2.1", 7) == 0)
    ret = load_skin_old_vti(fp, &skin);
  else if (strncmp(buf, "VTIv2.5", 7) == 0)
    ret = load_skin_vti(fp, &skin);
  else
    ret = load_skin_tiemu(fp, &skin);

  ret = load_jpeg(fp, &skin);

  fclose(fp);

  if (ret == 0)
    {
      DISPLAY("Skin loaded (%s): %d x %d, %s\n",
	      filename, skin.width, skin.height, buf);
    }
  
  return ret;
}


/* Read the header embedded in skin */
int skin_read_header(const char *filename, skinInfos* infos)
{
  FILE *fp = NULL;
  char buf[17];
  int ret = 0;

  fp = fopen(filename, "rb");
  if (fp == NULL)
    {
      DISPLAY("Unable to open this file: <%s>\n", filename);
      return -1;
    }

  /*
   * Determine the type of skin being loaded
   */

  fread(buf, 16, 1, fp);
  
  if (buf[0] == 'V') /* VTi skin, string is only 8 bytes long */
    buf[7] = 0;

  if (strncmp(buf, "VTIv2.1", 7) == 0)
    ret = load_skin_old_vti(fp, infos);
  else if (strncmp(buf, "VTIv2.5", 7) == 0)
    ret = load_skin_vti(fp, infos);
  else
    ret = load_skin_tiemu(fp, infos);

  fclose(fp);

  return 0;
}
