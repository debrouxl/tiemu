/* Hey EMACS -*- linux-c -*- */
/* $Id: interface.c 234 2004-05-21 15:49:39Z roms $ */

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

/*
	This module handles loading of images or upgrades.
	Images can be:
	- ROM dump
	- FLASH upgrade as a ROM dump
	
  	Note:0x12000 is the beginning of the system privileged part.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <glib.h>

#include "../core/uae/uae.h"

#include "intl.h"
#include "ti68k_int.h"
#include "ti68k_err.h"
#include "ti68k_def.h"
#include "../ti_hw/memory.h"
#include "images.h"

#define is_num(c)   isdigit(c)
#define is_alnum(c) isalnum(c)

#define SPP	0x12000		// system privileged part

IMG_INFO	img_infos = { 0 };
int			img_loaded = 0;

/*
	Utility functions
*/
int ti68k_is_a_rom_file(const char *filename)
{
	char *ext;

	ext = strrchr(filename, '.');
	if(ext == NULL)
		return 0;
	else if(!strcasecmp(ext, ".rom"))
		return !0;

	return 0;
}

int ti68k_is_a_tib_file(const char *filename)
{
	return (tifiles_is_a_flash_file(filename) || tifiles_is_a_tib_file(filename));
}

int ti68k_is_a_img_file(const char *filename)
{
	char *ext;

	ext = strrchr(filename, '.');
	if(ext == NULL)
		return 0;
	else if(!strcasecmp(ext, ".img"))
		return !0;

	return 0;
}

static uint16_t rd_word(uint8_t *p)
{
	return (p[0] << 8) | p[1];
}

static uint32_t rd_long(uint8_t *p)
{
	return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}

/*
	Display informations
*/
void ti68k_display_rom_infos(IMG_INFO *s)
{
	DISPLAY(_("Rom informations:\n"));
  	DISPLAY(_("  Calculator  : %s\n"), ti68k_calctype_to_string(s->calc_type));
  	DISPLAY(_("  Firmware    : v%s\n"), s->version);
  	DISPLAY(_("  Memory type : %s\n"), ti68k_romtype_to_string(s->internal | s->flash));
  	DISPLAY(_("  Memory size : %iMB (%i bytes)\n"), s->size >> 20, s->size);
	DISPLAY(_("  Hardware    : %i\n"), s->hw_type);
}

void ti68k_display_tib_infos(IMG_INFO *s)
{
	DISPLAY(_("Tib informations:\n"));
  	DISPLAY(_("  Calculator  : %s\n"), ti68k_calctype_to_string(s->calc_type));
  	DISPLAY(_("  Firmware    : v%s\n"), s->version);
  	DISPLAY(_("  Memory type : %s\n"), ti68k_romtype_to_string(s->internal | s->flash));
  	DISPLAY(_("  Memory size : %iMB (%i bytes)\n"), s->size >> 20, s->size);
}

void ti68k_display_img_infos(IMG_INFO *s)
{
	DISPLAY(_("Image informations:\n"));
  	DISPLAY(_("  Calculator  : %s\n"), ti68k_calctype_to_string(s->calc_type));
  	DISPLAY(_("  Firmware    : v%s\n"), s->version);
  	DISPLAY(_("  Memory type : %s\n"), ti68k_romtype_to_string(s->internal | s->flash));
  	DISPLAY(_("  Memory size : %iMB (%i bytes)\n"), s->size >> 20, s->size);
	DISPLAY(_("  Has boot    : %s\n"), s->has_boot ? "yes" : "no");
	DISPLAY(_("  Hardware    : %i\n"), s->hw_type);
}

void ti68k_display_hw_param_block(HW_PARM_BLOCK *s)
{
    gint i = 0;

    DISPLAY(_("Hardware Parameters Block:\n"));
    DISPLAY(_("Length           : %i\n"), s->len);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  hardwareID       : %i\n"), s->hardwareID);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  hardwareRevision : %i\n"), s->hardwareRevision);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  bootMajor        : %i\n"), s->bootMajor);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  bootRevision     : %i\n"), s->bootRevision);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  bootBuild        : %i\n"), s->bootBuild);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  gateArray        : %i\n"), s->gateArray);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  physDisplayBitsWide : %i\n"), s->physDisplayBitsWide & 0xff);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  physDisplayBitsTall : %i\n"), s->physDisplayBitsTall & 0xff);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  LCDBitsWide         : %i\n"), s->LCDBitsWide & 0xff);
    if(s->len > 2+(4*i++))
        DISPLAY(_("  LCDBitsTall         : %i\n"), s->LCDBitsTall & 0xff);
}

/*
    Read hardware parameter block from image.
*/
int ti68k_get_hw_param_block(IMG_INFO *rom, HW_PARM_BLOCK *block)
{
    uint32_t addr;

    addr = rd_long(&rom->data[0x104]);
	if(rom->internal)
		addr -= 0x200000;
	else
		addr -= 0x400000;

    memset(block, 0, sizeof(HW_PARM_BLOCK));
    block->len = rd_word(&(rom->data[addr+0]));
    block->hardwareID = rd_long(&(rom->data[addr+2]));
    block->hardwareRevision = rd_long(&(rom->data[addr+6]));
    block->bootMajor = rd_long(&(rom->data[addr+10]));
    block ->bootRevision = rd_long(&(rom->data[addr+14]));
    block->bootBuild = rd_long(&(rom->data[addr+18]));
    block->gateArray = rd_long(&(rom->data[addr+22]));
    block->physDisplayBitsWide = rd_long(&(rom->data[addr+26]));
    block->physDisplayBitsTall = rd_long(&(rom->data[addr+30]));
    block->LCDBitsWide = rd_long(&(rom->data[addr+34]));
    block->LCDBitsTall = rd_long(&(rom->data[addr+38]));

    return 0;
}


/*
	Get some informations on the ROM dump:
	- size
	- internal/external
	- FLASH/EPROM
	- os version
	- calc type
	Note: if the data field is NULL, memory is allocated. 
	Otherwise, data is overwritten.
	Thanks to Kevin for HW2 detection code.
*/
int ti68k_get_rom_infos(const char *filename, IMG_INFO *rom, int preload)
{
  	FILE *file;
    HW_PARM_BLOCK hwblock;

	// No filename, exits
	if(!strcmp(filename, ""))
		return 0;

	// Open file
  	file = fopen(filename, "rb");
  	if(file == NULL)
    {
      DISPLAY(_("Unable to open this file: <%s>\n"), filename);
      return ERR_CANT_OPEN;
    }

  	// Retrieve ROM size
  	fseek(file, 0, SEEK_END);
  	rom->size = ftell(file);
  	fseek(file, 0, SEEK_SET);

  	if(rom->size < 256) 
    	return ERR_INVALID_ROM_SIZE;
  	if (rom->size > 4*MB)
    	return ERR_INVALID_ROM_SIZE;
  
	if(rom->data == NULL)
  		rom->data = malloc(rom->size + 4);
	if(rom->data == NULL)
		return ERR_MALLOC;
  	memset(rom->data, 0xff, rom->size);
  	fread(rom->data, 1, rom->size, file);
  	fclose(file);

    rom->has_boot = 1;
  	rom->internal = ((rom->data[0x05] & 0x60) == 0x20) ? INTERNAL : 0;
  	rom->flash = (rom->data[0x65] & 0xf) ? 0 : FLASH_ROM;

    get_rom_version(rom->data, rom->size, rom->version);

    if(!rom->flash)
    {
        rom->calc_type = TI92;
        rom->hw_type = HW1;
    }
    else
    {
        // Get hw param block to determine calc type & hw type
        ti68k_get_hw_param_block(rom, &hwblock);
        ti68k_display_hw_param_block(&hwblock);

        switch(hwblock.hardwareID)
        {
        case 1: rom->calc_type = TI92p; break;
        case 3: rom->calc_type = TI89;  break;
        case 8: rom->calc_type = V200;  break;
        default: break;
        }

        if(rom->flash)
        {
            if(hwblock.len < 24)
                rom->hw_type = HW1;
            else
                rom->hw_type = (char)hwblock.gateArray;
        }
    }

	if(!preload)
		free(rom->data);

	return 0;
}


/*
  Get some informations on the FLASH upgrade:
  - size
  - internal/external
  - os version
  - calc type
*/
int ti68k_get_tib_infos(const char *filename, IMG_INFO *tib, int preload)
{
	Ti9xFlash content;
	Ti9xFlash *ptr;
	int nheaders;
	int i;

	// No filename, exits
	if(!strcmp(filename, ""))
		return 0;

	// Check valid file
	if(!tifiles_is_a_ti_file(filename))
		return ERR_NOT_TI_FILE;
		
	if(!ti68k_is_a_tib_file(filename))
		return ERR_INVALID_UPGRADE;

	// Load file
	ti9x_read_flash_file(filename, &content);
	
	// count headers
  	for (ptr = &content; ptr != NULL; ptr = ptr->next)
    	nheaders++;
  	
  	// keep the last one (data)
  	for (i = 0, ptr = &content; i < nheaders - 1; i++)
    	ptr = ptr->next;
    	
  	// Load TIB into memory and relocate at SPP
	if(tib->data == NULL)
  		tib->data = malloc(SPP + ptr->data_length + 4);
	if(tib->data == NULL)
		return ERR_MALLOC;
  	//memset(tib->data, 0xff, SPP + ptr->data_length);
    memset(tib->data + SPP, 0xff, ptr->data_length);
  	memcpy(tib->data + SPP, ptr->data_part, ptr->data_length);
  	
  	// Update current rom infos
	switch(ptr->device_type & 0xff)
	{
		case DEVICE_TYPE_89:
	   		tib->internal = INTERNAL;
      		tib->calc_type = TI89;
		break;
		case DEVICE_TYPE_92P:
            if(tifiles_which_calc_type(filename) == CALC_TI92P)
            {
      		    tib->calc_type = TI92p;
                tib->internal = 0;
            }
            else
            {
                tib->calc_type = V200;
                tib->internal = INTERNAL;
            }
		break;
		default:
			DISPLAY("TIB problem: <%i>!\n", 0xff & ptr->device_type);
			return ERR_INVALID_UPGRADE;
		break;
	}
    
  	tib->flash = FLASH_ROM;
  	tib->has_boot = 0;
  	tib->size = ptr->data_length + SPP;

  	get_rom_version(tib->data, tib->size, tib->version);

	//tib->hw_type = 2;	// hw2 is default
  	
  	ti9x_free_flash_content(&content);
	if(!preload)
		free(tib->data);

  	return 0;
}

/*
	Try to get some informations on the ROM dump:
	- size
	- internal/external
	- FLASH/EPROM
	- soft version
	- calc type
*/
int ti68k_get_img_infos(const char *filename, IMG_INFO *ri)
{
	FILE *f;

	// No filename, exits
	if(!strcmp(filename, ""))
		return 0;

	// Check file
	if(!ti68k_is_a_img_file(filename))
	{
		fprintf(stderr, "Images must have '.img' extension.\n", filename);
      	return ERR_CANT_OPEN;
	}
	
	// Open dest file
  	f = fopen(filename, "rb");
  	if(f == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", filename);
      	return ERR_CANT_OPEN;
    }
    
    // Read header
    fread(ri, sizeof(IMG_INFO), 1, f);
    if(strcmp(ri->signature, "TiEmu img v2.00"))
   	{
   		fprintf(stderr, "Bad image: <%s>\n", filename);
      	return ERR_INVALID_UPGRADE;
   	}
    
    // Close file
    fclose(f);
    
    return 0;
}

/*
  	Convert a romdump into an image.
	This kind of image is complete (boot & certificate).
*/
int ti68k_convert_rom_to_image(const char *srcname, const char *dirname, char **dstname)
{
  	FILE *f; 
  	int err;
	IMG_INFO img = { 0 };
	char *ext;
	gchar *basename;
	int i;

	// No filename, exits
	if(!strcmp(srcname, ""))
		return 0;

	// Preload romdump
	err = ti68k_get_rom_infos(srcname, &img, !0);
	if(err)
    {
      	DISPLAY(_("Unable to get informations on ROM dump.\n"));
      	return err;
    }
	ti68k_display_rom_infos(&img);

	// Create destination file
	basename = g_path_get_basename(srcname);
	ext = strrchr(basename, '.');
  	*ext='\0';
	strcat(basename, ".img");

	*dstname = g_strconcat(dirname, basename, NULL);
	g_free(basename);

	// Open dest file
  	f = fopen(*dstname, "wb");
  	if(f == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", *dstname);
      	return ERR_CANT_OPEN;
    }

	// Fill header
	strcpy(img.signature, "TiEmu img v2.00");
	img.header_size = sizeof(IMG_INFO);
	img.data_offset = 0x40;

	// Write file
	fwrite(&img, 1, sizeof(IMG_INFO), f);
	for(i = img.header_size; i < img.data_offset; i++)
		fputc(0, f);
	fwrite(img.data, sizeof(char), img.size, f);

	// Close file
	fclose(f);

  	return 0;
}

/*
	Convert an upgrade into an image.
  	The image has neither boot block nor certificate.
*/
int ti68k_convert_tib_to_image(const char *srcname, const char *dirname, char **dstname)
{
	FILE *f; 
  	int err;
	IMG_INFO img = { 0 };
	char *ext;
	gchar *basename;
	int i, j;
	int num_blocks, last_block;
    int real_size;

	// No filename, exits
	if(!strcmp(srcname, ""))
		return 0;

	// Preload upgrade
	err = ti68k_get_tib_infos(srcname, &img, !0);
	if(err)
    {
      	DISPLAY(_("Unable to get informations on FLASH upgrade.\n"));
      	return err;
    }
	ti68k_display_tib_infos(&img);

	// Create destination file
	basename = g_path_get_basename(srcname);
	ext = strrchr(basename, '.');
  	*ext='\0';
	strcat(basename, ".img");

	*dstname = g_strconcat(dirname, basename, NULL);
	g_free(basename);

	// Open dest file
  	f = fopen(*dstname, "wb");
  	if(f == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", *dstname);
      	return ERR_CANT_OPEN;
    }

	// Fill header
	strcpy(img.signature, "TiEmu img v2.00");
	img.header_size = sizeof(IMG_INFO);
	img.data_offset = 0x40;
    real_size = img.size - SPP;
    img.size = ti68k_get_rom_size(img.calc_type);
    img.hw_type = HW2;  //default
	
	// Write header
	fwrite(&img, 1, sizeof(IMG_INFO), f);
	for(i = img.header_size; i < img.data_offset; i++)
		fputc(0, f);
  
  	// Write boot block
  	for(i=0; i<0x05; i++)
    	fputc(0xff, f);
    
    if(img.internal)
    	fputc(0x20, f); 	// internal
  	else
    	fputc(0x40, f); 	// external

  	for(i=0x06; i<0x65; i++)
    	fputc(0xff, f);

  	fputc(0xf0, f); 		// FLASH ROM

    for(i = 0x66; i < 0x104; i++)
        fputc(0xff, f);

    // hardware param block
    fputc(0x00, f);
    fputc(img.internal ? 0x20: 0x40, f);
    fputc(0x01, f);
    fputc(0x08, f);

    fputc(0x04, f);
    fputc(0x00, f);

    fputc(0x00, f);
    fputc(0x00, f);
    fputc(0x00, f);
    switch(img.calc_type)
    {
        case TI89:  fputc(0x03, f); break;
        case TI92p: fputc(0x01, f); break;
        case V200:  fputc(0x08, f); break;
    }

  	for(i = 0x10e; i < SPP; i++)
    	fputc(0xff, f);
  
  	// Copy FLASH upgrade at 0x12000 (SPP)
  	num_blocks = real_size / 65536;
  	for(i = 0; i < num_blocks; i++ )
    {
      	DISPLAY(".");
      	fflush(stdout);

      	fwrite(&img.data[65536 * i + SPP], sizeof(char), 65536, f);
    }

  	last_block = real_size % 65536;
   	fwrite(&img.data[65536 * i + SPP], sizeof(char), last_block, f);
  
  	DISPLAY("\n");
  	DISPLAY("Completing to %iMB size\n", img.size >> 20);
  	for(j = SPP + real_size; j < img.size; j++)
  		fputc(0xff, f);
  
  	// Close file
  	fclose(f);

  	return 0;
}

/*
    Convert an romdump into image and replace SPP by upgrade.
    The resulting image has boot block.
*/
int ti68k_merge_rom_and_tib_to_image(const char *srcname1, const char *srcname2, 
                                     const char *dirname, char **dstname)
{
    FILE *f; 
  	int err;
	IMG_INFO img = { 0 };
	char *ext;
	gchar *basename;
	int i;
    int real_size;

	// No filename, exits
	if(!strcmp(srcname1, ""))
		return 0;

    if(!strcmp(srcname2, ""))
		return 0;

	// Preload romdump
	err = ti68k_get_rom_infos(srcname1, &img, !0);
	if(err)
    {
      	DISPLAY(_("Unable to get informations on ROM dump.\n"));
      	return err;
    }
	ti68k_display_rom_infos(&img);

    // Save size
    real_size = img.size;

    // Load upgrade
    err = ti68k_get_tib_infos(srcname2, &img, !0);
	if(err)
    {
      	DISPLAY(_("Unable to get informations on ROM dump.\n"));
      	return err;
    }
	ti68k_display_tib_infos(&img);

	// Create destination file
	basename = g_path_get_basename(srcname1);
	ext = strrchr(basename, '.');
  	*ext='\0';
	strcat(basename, ".img");

	*dstname = g_strconcat(dirname, basename, NULL);
	g_free(basename);

    // Restore size
    img.size = real_size;

	// Open dest file
  	f = fopen(*dstname, "wb");
  	if(f == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", *dstname);
      	return ERR_CANT_OPEN;
    }

	// Fill header
	strcpy(img.signature, "TiEmu img v2.00");
	img.header_size = sizeof(IMG_INFO);
	img.data_offset = 0x40;
    img.has_boot = 1;

	// Write file
	fwrite(&img, 1, sizeof(IMG_INFO), f);
	for(i = img.header_size; i < img.data_offset; i++)
		fputc(0, f);
	fwrite(img.data, sizeof(char), img.size, f);

	// Close file
	fclose(f);

  	return 0;
}


/*
  	This function loads an image.
*/
int ti68k_load_image(const char *filename)
{
	IMG_INFO *img = &img_infos;
  	FILE *f;  	
  	int err;

	// Clear infos
	memset(img, 0, sizeof(IMG_INFO));

	// No filename, exits
	if(!strcmp(filename, ""))
		return 0;

	// Load infos
	err = ti68k_get_img_infos(filename, img);
  	if(err)
    {
      	DISPLAY(_("Unable to get informations on ROM image.\n"));
      	return err;
    }
	ti68k_display_img_infos(img);
	
	// Open file
	f = fopen(filename, "rb");
	if(f == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", filename);
      	return ERR_CANT_OPEN;
    }

	// Read pure data
    fseek(f, img->data_offset, SEEK_SET);
	img->data = malloc(img->size + 4);
	if(img->data == NULL)
		return ERR_MALLOC;
    fread(img->data, 1, img->size, f);	
  
  	img_loaded = 1;
  	return 0;
}

/*
  	Load a FLASH upgrade (.tib/.9xu/.89u).
  	Note: an image must have been loaded before 
  	calling this function.
*/
int ti68k_load_upgrade(const char *filename)
{
	IMG_INFO tib = { 0 };
  	int err;
    IMG_INFO *img = &img_infos;

	if(!img_loaded)
		return -1;

	// No filename, exits
	if(!strcmp(filename, ""))
		return 0;

	err = ti68k_get_tib_infos(filename, &tib, !0);
	if(err)
    {
      	DISPLAY(_("Unable to get informations on FLASH upgrade.\n"));
      	return err;
    }
	ti68k_display_tib_infos(&tib);

    // Allow upgrade ?
    if(tib.calc_type != img->calc_type)
    {
        free(tib.data);
        return ERR_CANT_UPGRADE;
    }

	tib.has_boot = 1;	// still bootable
	memcpy(tihw.rom+SPP, tib.data+SPP, tib.size-SPP);
    free(tib.data);

  	img_loaded = 2;
	return 0;
}


/*
    Search for ROM dumps in a given directory and
    converts them into images.
*/
int ti68k_scan_files(const char *src_dir, const char *dst_dir)
{
    GDir *dir;
	GError *error = NULL;
	G_CONST_RETURN gchar *dirent;
    gchar *path;
    int ret;
    gchar *dstname;

    // Search for *.rom files and convert them
	dir = g_dir_open(src_dir, 0, &error);
	if (dir == NULL) 
	{
		fprintf(stderr, _("Opendir error\n"));
      	return ERR_CANT_OPEN_DIR;
	}

    while ((dirent = g_dir_read_name(dir)) != NULL) 
	{
  		if (dirent[0] == '.') 
  			continue;

        path = g_strconcat(src_dir, dirent, NULL);

        if(ti68k_is_a_rom_file(dirent))
        {
            ret = ti68k_convert_rom_to_image(path, dst_dir, &dstname);
            if(!ret)
                unlink(path);
        }

        g_free(path);
    }

    g_dir_close(dir);

    return 0;
}

/*
  	Scan images in a given directory 
  	and build the cache file.
*/
int ti68k_scan_images(const char *dirname, const char *filename)
{
	FILE *file;
	IMG_INFO img = { 0 };
	GDir *dir;
	GError *error = NULL;
	G_CONST_RETURN gchar *dirent;
	char buffer[1024];
	char str[20];
	char *rom_names[256] = { 0 };
	int nlines;
	int j;
	gchar *path;
	struct stat f_info;
	int ret;  	
  	char *line[7];

  	DISPLAY(_("Scanning images/upgrades... "));

	// First, check if cache file exists
  	if(!access(filename, F_OK))
    {
      	// if yes, ...
      	file = fopen(filename, "rt");
      	if(file == NULL)
		{
	  		fprintf(stderr, _("Unable to open this file: <%s>\n"), filename);
	  		return ERR_CANT_OPEN;
		}

		// read it and store ROM names
      	for(nlines = 0; !feof(file) && (nlines < 255); nlines++)
		{
	  		fgets(buffer, sizeof(buffer), file);
			if(feof(file))
				break;

	  		sscanf(buffer, "%s\t", str);
	  		rom_names[nlines] = g_strdup(str);
		}		
      
		// ready to add new entries
		fclose(file);
      	file = fopen(filename, "at");
      	if(file == NULL)
        {
          	fprintf(stderr, _("Unable to reopen this file: <%s>\n"), filename);
          	return ERR_CANT_OPEN;
        }
    }
  	else
    {
      	DISPLAY(_(" (cache need to be built) "));
      	file = fopen(filename, "wt");
      	if(file == NULL)
        {
          	fprintf(stderr, _("Unable to open this file: <%s>\n"), filename);
          	return ERR_68K_CANT_OPEN;
        }

		nlines = 0;
		/*fprintf(file, "%s\t%s\t%s\t%s\t%s\t%s\n", 
						"filename", "calctype", "version", 
						"memtype", "size", "boot");*/
    }  

  	// List all files available in the directory
	dir = g_dir_open(dirname, 0, &error);
	if (dir == NULL) 
	{
		fprintf(stderr, _("Opendir error\n"));
      	return ERR_CANT_OPEN_DIR;
	}
  
	while ((dirent = g_dir_read_name(dir)) != NULL) 
	{
  		if (dirent[0] == '.') 
  			continue;
      
      	// compare current file with cache
      	for(j = 0; j < nlines; j++)
		{
	  		if(!strcmp(rom_names[j], dirent))
	    		break;
		}
		
		// we have a new entry, we add it in the cache
      	if(j == nlines)
		{
	  		path = g_strconcat(dirname, dirent, NULL);
	  		
			ret = stat(path, &f_info);
			if(ret == -1)
			{
				fprintf(stderr, _("Can not stat: <%s>\n"), dirent);
	      		perror("stat: ");
			}
			else
			{
				if(ti68k_is_a_tib_file(path))
				{
					memset(&img, 0, sizeof(IMG_INFO));
					ret = ti68k_get_tib_infos(path, &img, 0);
					if(ret)
					{
						fprintf(stderr, _("Can not get ROM/update info: <%s>\n"), path);
						break;
					}
				}
				else if(ti68k_is_a_img_file(path))
				{
					memset(&img, 0, sizeof(IMG_INFO));
					ret = ti68k_get_img_infos(path, &img);
					if(ret)
					{
						fprintf(stderr, _("Can not get ROM/update info: <%s>\n"), path);
						break;
					}
				}
                else
					continue;

		  		line[0] = (char *)dirent;
		  		line[1] = (char *)ti68k_calctype_to_string(img.calc_type);
	  			line[2] = img.version;
	  			line[3] = (char *)ti68k_romtype_to_string(img.internal | img.flash);
	  			sprintf(str, "%iKB", img.size >> 10);
	  			line[4] = str;
	  			if(img.has_boot)
	  				line[5] = _("yes");
	  			else
	  				line[5] = _("no");
				line[6] = (char *)ti68k_hwtype_to_string(img.hw_type);
		  
		  			fprintf(file, "%s\t%s\t%s\t%s\t%s\t%s\n", 
		  				line[0], line[1], line[2], 
		  				line[3], line[4], line[5], line[6]);
			}
	  		g_free(path);
		}
    }      

	g_dir_close(dir);
  
  	fclose(file);
  	DISPLAY(_("Done.\n"));
  
  	return 0;
}


/*
  	Search the version string in the ROM
	Arguments:
  	- ptr: a ROM or update image
  	- size: the size of the buffer
  	- version: the returned string version
*/
static int get_rom_version(char *ptr, int size, char *version)
{
  	int i;

  	strcpy(version, "?.??");

  	for (i = SPP; i < size-16; i += 2)
    {
      if (is_num(ptr[i])&&(ptr[i+1]=='.') && is_num(ptr[i+2]) &&
	  (ptr[i+3]==0)&&is_alnum(ptr[i+4]) && is_alnum(ptr[i+5]) &&
	  (ptr[i+6]=='/')&&is_alnum(ptr[i+7]) && is_alnum(ptr[i+8]) &&
	  (ptr[i+9]=='/')&&is_alnum(ptr[i+10]) && is_alnum(ptr[i+11]))
	  	break;

      if (is_num(ptr[i]) && (ptr[i+1]=='.') && is_num(ptr[i+2]) && 
	  is_num(ptr[i+3]) && (ptr[i+4]==0) && is_alnum(ptr[i+5]) && 
	  is_alnum(ptr[i+6]) && (ptr[i+7]=='/') && is_alnum(ptr[i+8]) && 
	  is_alnum(ptr[i+9]) && (ptr[i+10]=='/') && is_alnum(ptr[i+11]) && 
	  is_alnum(ptr[i+12]))
		break;
	
	  if (is_num(ptr[i]) && (ptr[i+1]=='.') && is_num(ptr[i+2]) && 
	  (ptr[i+3]==0) && is_alnum(ptr[i+4]) && is_alnum(ptr[i+5]) && 
	  is_alnum(ptr[i+6]) && is_alnum(ptr[i+7]) && is_alnum(ptr[i+8]) && 
	  is_alnum(ptr[i+9]) && is_alnum(ptr[i+10]) && is_alnum(ptr[i+11]))
	  	break;

      if (is_num(ptr[i]) && (ptr[i+1]=='.') && is_num(ptr[i+2]) && 
	  is_alnum(ptr[i+3]) && (ptr[i+4]==0) && is_alnum(ptr[i+5]) && 
	  is_alnum(ptr[i+6]) && is_alnum(ptr[i+7]) && is_alnum(ptr[i+8]) && 
	  is_alnum(ptr[i+9]) && is_alnum(ptr[i+10]) && is_alnum(ptr[i+11]))
	  	break;
    }
  
  	if (i < size-16) 
    {
      	int n;
      
      	for(n = i; n < i+16; n++) 
		{
	  		if (ptr[n]==0) 
	    	{
	      		strcpy(version, ptr+i);
	      		(version)[n-i]=0;

	      		return 0;
	    	}
		}
    }
    
  	return 0;
}
