/* Hey EMACS -*- linux-c -*- */
/* $Id: cabl_int.h 651 2004-04-25 15:22:07Z roms $ */

/*  TiEmu - a TI emulator
 *  loader.c: loader for TiEmu skins
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
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
#include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#if !defined(__WIN32__) && !defined(__MACOSX__)
#include <byteswap.h>
#endif
#include <jpeglib.h>

#include "skin.h"

#if defined(__MACOSX__) || defined(__WIN32__)
#define bswap_32(a) (a >> 24) | ((a & 0xff0000) >> 16) << 8 | ((a & 0xff00) >> 8) << 16 | (a & 0xff) << 8
#endif /* __MACOSX__ || __WIN32__ */

SKIN_INFOS skin_infos = { 0 };
static int skin_loaded = 0;

// taken from skinedit/src/skinops.c/load_skin_tiemu()
int skin_read_header(const char *filename,SKIN_INFOS* infos)
{
	FILE *fp = NULL;
  	int i;
  	uint32_t endian;
  	uint32_t jpeg_offset;
  	uint32_t length;
  
	fp = fopen(filename, "rb");
  	if (fp == NULL)
    	{
      		fprintf(stderr, "Unable to open this file: <%s>\n", filename);
      		return -1;
    	}
 
	/* offsets */
  	fseek(fp, 16, SEEK_SET);  
  	fread(&endian, 4, 1, fp);
  	fread(&jpeg_offset, 4, 1, fp);
	if (endian != ENDIANNESS_FLAG)
		jpeg_offset = bswap_32(jpeg_offset);

	/* Skin name */
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
	
	/* Skin author */
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

	/* LCD colors */
  	fread(&skin_infos.colortype, 4, 1, fp);
  	fread(&skin_infos.lcd_white, 4, 1, fp);
  	fread(&skin_infos.lcd_black, 4, 1, fp);

   	/* Calc type */
  	fread(skin_infos.calc, 8, 1, fp);

  	/* LCD position */
  	fread(&skin_infos.lcd_pos.left, 4, 1, fp);
  	fread(&skin_infos.lcd_pos.top, 4, 1, fp);
  	fread(&skin_infos.lcd_pos.right, 4, 1, fp);
  	fread(&skin_infos.lcd_pos.bottom, 4, 1, fp);

	/* Number of RECT struct to read */
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
    	
    	fclose(fp);
  	
    	return 0;
}

int skin_read_image(const char *filename, SKIN_INFOS* infos)
{
	FILE *fp = NULL;
  	uint32_t endian;
  	uint32_t jpeg_offset;
  	struct jpeg_decompress_struct cinfo;
   	struct jpeg_error_mgr jerr;
	int i, j;
	char *p;
	int scaled = 0;
  
    	fp = fopen(filename, "rb");
  	if (fp == NULL)
    	{
      		fprintf(stderr, "Unable to open this file: <%s>\n", filename);
      		return -1;
    	}
    	
    	/* offsets */
  	fseek(fp, 16, SEEK_SET);  
  	fread(&endian, 4, 1, fp);
  	fread(&jpeg_offset, 4, 1, fp);
	if (endian != ENDIANNESS_FLAG)
		jpeg_offset = bswap_32(jpeg_offset);
	fseek(fp, jpeg_offset, SEEK_SET);
  	
  	// Init JPEG
    	cinfo.err = jpeg_std_error(&jerr);
    	jpeg_create_decompress(&cinfo);

#ifdef __WIN32__
	jpeg_file_src(&cinfo, fp);
#else
	jpeg_stdio_src(&cinfo, fp);
#endif
    	jpeg_read_header(&cinfo, TRUE);

    	// Rescale image to half if necessary
    	if ((cinfo.image_width > 600) || (cinfo.image_height > 600)) {
		cinfo.scale_num = 1;
		cinfo.scale_denom = 2;
		scaled = 1;
    	}
    
    	// Require a colormapped output with a limited number of colors
    	cinfo.quantize_colors = TRUE;
    	cinfo.desired_number_of_colors = MAX_COLORS;

    	// Load JPEG image
    	jpeg_start_decompress(&cinfo);
    	skin_infos.ncolors = cinfo.actual_number_of_colors;

    	// Copy the color palette
    	for (j = 0; j < cinfo.actual_number_of_colors; j++) {
			skin_infos.cmap[0][j] = cinfo.colormap[0][j];
			skin_infos.cmap[1][j] = cinfo.colormap[1][j];
			skin_infos.cmap[2][j] = cinfo.colormap[2][j];
    	}

    	if (cinfo.output_components != 1)	// 1: palettized 
		return -1;

    	// Get skin size
    	skin_infos.width = cinfo.output_width + (cinfo.output_width & 3);
    	skin_infos.height = cinfo.output_height + (cinfo.output_height & 3);

    	// Allocate image
    	skin_infos.img = p = (unsigned char *) malloc(skin_infos.width * skin_infos.height);
    	if (skin_infos.img == NULL)
		return -1;

    	// Load jpeg image line by line //c += xx*yy;
    	while (cinfo.output_scanline < cinfo.output_height) {
		p += skin_infos.width;
		jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&p, 1);
    	}

    	// Close JPEG
    	jpeg_finish_decompress(&cinfo);
    	jpeg_destroy_decompress(&cinfo);

	// Rescale coords
	if(scaled) {
		skin_infos.lcd_pos.left >>= 1;
		skin_infos.lcd_pos.right >>= 1;
		skin_infos.lcd_pos.top >>= 1;
		skin_infos.lcd_pos.bottom >>= 1;
	
		for (i = 0; i < SKIN_KEYS; i++) {
      			skin_infos.keys_pos[i].left >>= 1;
      			skin_infos.keys_pos[i].top >>= 1;
      			skin_infos.keys_pos[i].right >>= 1;
      			skin_infos.keys_pos[i].bottom >>= 1;
    		}
	}
    	
    	/* Close file */
    	fclose(fp);
    	
    	return 0;
}

/* Unload skin by freeing allocated memory */
int skin_unload(void)
{
  	if (skin_loaded == FALSE)
    		return -1;
  	else
    		skin_loaded = FALSE;
  
  	free(skin_infos.img);
  	free(skin_infos.name);
  	free(skin_infos.author);

  	memset(&skin_infos, 0, sizeof(SKIN_INFOS));
  
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
      		fprintf(stderr, "Unable to open this file: <%s>\n", filename);
      		return -1;
    	}

  	fread(buf, 16, 1, fp);
  	if (strncmp(buf, "TiEmu v2.00", 16))
  	{
  		fprintf(stderr, "Bad skin format\n");
      		return -1;
  	}

	fclose(fp);
  	
  	ret = skin_read_header(filename, &skin_infos);
  	ret = skin_read_image(filename, &skin_infos);

  	if (ret == 0)
    	{
      		fprintf(stderr, "Skin loaded (%s): %d x %d, %s\n", filename, skin_infos.width, skin_infos.height, buf);
    	}
  
  	return ret;
}



