/*  GtkTiEmu - a TI emulator
 *  skn_loader.c: loader of Rusty's skin
 *  Copyright (C) 2000, Thomas Corvazier, Romain Lievin
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
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <unistd.h>
#include <string.h>

#include "paths.h"
#include "tilibs.h"
#include "lib68k.h"
#include "skn_loader.h"
#include "struct.h"

#define SKINFILE_92  "ti92.skn"
#define SKINFILE_89  "ti89.skn"

extern const char sknKey92[];
extern const char sknKey89[];

/*
  Active skin
*/
SKIN skin = { 0 };
static int skin_loaded = 0;

/*
  Load a JPEG file from stream 'fp'
*/
int load_jpeg(FILE * fp, SKIN * skin)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    unsigned char *p;
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

    // Load JPEG image
    jpeg_start_decompress(&cinfo);

    skin->ncolors = cinfo.actual_number_of_colors;

    // Copy the color palette
    for (j = 0; j < cinfo.actual_number_of_colors; j++) {
	skin->cmap[0][j] = cinfo.colormap[0][j];
	skin->cmap[1][j] = cinfo.colormap[1][j];
	skin->cmap[2][j] = cinfo.colormap[2][j];
    }

    if (cinfo.output_components != 1)	// 1: palettized 
	return -1;

    // Get skin size
    skin->width = cinfo.output_width + (cinfo.output_width & 3);
    skin->height = cinfo.output_height + (cinfo.output_height & 3);

    // Allocate image
    skin->img = p = (unsigned char *) malloc(skin->width * skin->height);
    if (skin->img == NULL)
	return -1;

    // Load jpeg image line by line //c += xx*yy;
    while (cinfo.output_scanline < cinfo.output_height) {
	p += skin->width;
	jpeg_read_scanlines(&cinfo, &p, 1);
    }

    // Close JPEG
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 0;
}

/*
  Unload skin by freeing allocated memory
*/
int unload_skin(void)
{
  if (skin_loaded == FALSE)
    return -1;
  
  skin_loaded = FALSE;
  free(skin.img);
  memset(&skin, 0, sizeof(SKIN));
  
    return 0;
}

/*
  Load a skin from file and place it into the 'skin' structure
*/
int load_skin(char *fn)
{
    FILE *fp;
    int i;
    char str[9];
    char filename[2 * MAXCHARS];
    char *skin_fn = SKINFILE_92;

    if(skin_loaded)
      unload_skin();

    skin.key_mapping = NULL;
    strcpy(filename, inst_paths.skin_dir);

    if (ti68k_getCalcType() & TI92) {
	skin_fn = SKINFILE_92;
	skin.key_mapping = sknKey92;
    } else if (ti68k_getCalcType() & TI89) {
	skin_fn = SKINFILE_89;
	skin.key_mapping = sknKey89;
    }

    /* Open the skin file */
    strcat(filename, skin_fn);
    fprintf(stdout, "Loading skin: %s\n", filename);

    if (access(filename, F_OK) != 0) {
	fprintf(stdout,
		"Unable to load the requested skin: it does not exist.\n");
	exit(-1);
    }

    fp = fopen(filename, "rb");
    if (fp == NULL)
	return 0;

    /* Check the skin version */
    fread(str, 8, 1, fp);
    str[8] = 0;
    fprintf(stdout, "Skin version: %s\n", str);
    if (!strcmp(str, "VTIv2.1 "))
	fseek(fp, 80, SEEK_SET);
    else
	fseek(fp, 144, SEEK_SET);

    /* Read the skin data */
    fread(&skin.white, 4, 1, fp);
    fread(&skin.black, 4, 1, fp);
    fread(&skin.rcLcd, sizeof (RECT), 1, fp);	// read the LCD coords
    fread(skin.rcKeys, sizeof (RECT), 80, fp);	// read skin keys

    /* Read the skin image */
    if (load_jpeg(fp, &skin)) {
	return -1;
	fclose(fp);
    }

    for (i = 0; i < 80; i++) {
#ifdef WORDS_BIGENDIAN
	rcKeys[i].top = swap_bytes(rcKeys[i].top);
	rcKeys[i].bottom = swap_bytes(rcKeys[i].bottom);
	rcKeys[i].left = swap_bytes(rcKeys[i].left);
	rcKeys[i].right = swap_bytes(rcKeys[i].right);
#endif

	skin.rcKeys[i].top >>= 1;
	skin.rcKeys[i].bottom >>= 1;
	skin.rcKeys[i].left >>= 1;
	skin.rcKeys[i].right >>= 1;
    }

#ifdef WORDS_BIGENDIAN
    rcLcd.top = swap_bytes(rcLcd.top);
    rcLcd.bottom = swap_bytes(rcLcd.bottom);
    rcLcd.left = swap_bytes(rcLcd.left);
    rcLcd.right = swap_bytes(rcLcd.right);
#endif

    skin.rcLcd.top >>= 1;
    skin.rcLcd.bottom >>= 1;
    skin.rcLcd.left >>= 1;
    skin.rcLcd.right >>= 1;

    skin_loaded = TRUE;

    return 1;
}
