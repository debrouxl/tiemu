/* Hey EMACS -*- linux-c -*- */
/* $Id: images.c 167 2004-05-13 07:06:02Z roms $ */

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

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <glib.h>

#include "../core/uae/uae.h"

#include "intl.h"
#include "interface.h"
#include "../ti_hw/memory.h"
#include "packets.h"
#include "hardware.h"
#include "keyboard.h"
#include "params.h"
#include "callbacks.h"
#include "intlist.h"
#include "errcodes.h"
#include "images.h"
#include "m68k.h"
#include "bkpts.h"

#ifdef __WIN32__
# define strcasecmp _stricmp
#endif

#define is_num(c)   isdigit(c)
#define is_alnum(c) isalnum(c)

#define SPP	0x12000		// system privileged part

static int get_rom_version(char *ptr, int size, char *version);

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


/*
	Display informations
*/
int ti68k_display_rom_infos(ROM_INFO *s)
{
	DISPLAY(_("Rom informations:\n"));
  	DISPLAY(_("  Calculator  : %s\n"), ti68k_calctype_to_string(s->calc_type));
  	DISPLAY(_("  Firmware    : v%s\n"), s->version);
  	DISPLAY(_("  Memory type : %s\n"), ti68k_romtype_to_string(s->internal | s->flash));
  	DISPLAY(_("  Memory size : %iMB (%i bytes)\n"), s->size >> 20, s->size);
	DISPLAY(_("  Image type  : dump\n"));
}

int ti68k_display_tib_infos(TIB_INFO *s)
{
	DISPLAY(_("Tib informations:\n"));
  	DISPLAY(_("  Calculator  : %s\n"), ti68k_calctype_to_string(s->calc_type));
  	DISPLAY(_("  Firmware    : v%s\n"), s->version);
  	DISPLAY(_("  Memory type : %s\n"), ti68k_romtype_to_string(s->internal | s->flash));
  	DISPLAY(_("  Memory size : %iMB (%i bytes)\n"), s->size >> 20, s->size);
	DISPLAY(_("  Image type  : upgrade\n"));
}

int ti68k_display_img_infos(IMG_INFO *s)
{
	DISPLAY(_("Image informations:\n"));
  	DISPLAY(_("  Calculator  : %s\n"), tifiles_calctype_to_string(s->calc_type));
  	DISPLAY(_("  Firmware    : v%s\n"), s->revision);
  	DISPLAY(_("  Memory type : %s\n"), ti68k_romtype_to_string(s->internal | s->flash));
  	DISPLAY(_("  Memory size : %iMB (%i bytes)\n"), s->data_size >> 20, s->data_size);
	DISPLAY(_("  Has boot    : %s\n"), s->has_boot ? "yes" : "no");
}

/*
	Get some informations on the ROM dump:
	- size
	- internal/external
	- FLASH/EPROM
	- os version
	- calc type
*/
int ti68k_get_rom_infos(const char *filename, ROM_INFO *ri, int preload)
{
  	FILE *file;

  	file = fopen(filename, "rb");
  	if(file == NULL)
    {
      DISPLAY(_("Unable to open this file: <%s>\n"), filename);
      return ERR_68K_CANT_OPEN;
    }

  	// Retrieve ROM size
  	fseek(file, 0, SEEK_END);
  	ri->size = ftell(file);
  	fseek(file, 0, SEEK_SET);

  	if(ri->size < 256) 
    	return ERR_68K_INVALID_SIZE;
  	if (ri->size > 2*1024*1024)
    	return ERR_68K_INVALID_SIZE;
  
  	ri->content = malloc(ri->size + 4);
  	memset(ri->content, 0xff, ri->size);
  	fread(ri->content, 1, ri->size, file);
  	fclose(file);

  	ri->internal = ((ri->content[0x05] & 0x60) == 0x20) ? INTERNAL : 0;
  	ri->flash = (ri->content[0x65] & 0xf) ? 0 : FLASH_ROM;
  	ri->tib = 0;

  	if(ri->internal && ri->flash)
    	ri->calc_type = TI89;
  	else if (ri->flash)
    	ri->calc_type = TI92 | MODULEPLUS;
  	else 
    	ri->calc_type = TI92;
  
  	get_rom_version(ri->content, ri->size, ri->version);

	if(!preload)
		free(ri->content);

	return 0;
}


/*
  Get some informations on the FLASH upgrade:
  - size
  - internal/external
  - os version
  - calc type
*/
int ti68k_get_tib_infos(const char *filename, TIB_INFO *ti, int preload)
{
	Ti9xFlash content;
	Ti9xFlash *ptr;
	int nheaders;
	int i;

	// Check valid file
	if(!tifiles_is_a_ti_file(filename))
		return ERR_68K_TI_FILE;
		
	if(!ti68k_is_a_tib_file(filename))
		return ERR_68K_INVALID_FLASH;

	// Load file
	ti9x_read_flash_file(filename, &content);
	
	// count headers
  	for (ptr = &content; ptr != NULL; ptr = ptr->next)
    	nheaders++;
  	
  	// keep the last one (data)
  	for (i = 0, ptr = &content; i < nheaders - 1; i++)
    	ptr = ptr->next;
    	
  	// Load TIB into memory
  	ti->content = malloc(SPP + ptr->data_length + 4);
  	memset(ti->content, 0xff, SPP + ptr->data_length);
  	memcpy(ti->content + SPP, ptr->data_part, ptr->data_length);
  	
  	// Update current rom infos
  	if (((ti->content+SPP)[0x88+0x05] & 0x60) == 0x20)
    {
      	ti->internal = INTERNAL;
      	ti->calc_type = TI89;
    }
	else
    {
      	ti->internal = 0;
      	ti->calc_type = TI92 | MODULEPLUS;
    }
    
  	ti->flash = FLASH_ROM;
  	ti->tib = 1;
  	ti->size = ptr->data_length + SPP;

  	get_rom_version(ti->content, ti->size, ti->version);
  	
  	ti9x_free_flash_content(&content);
	if(!preload)
		free(ti->content);

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
	char *ext;

	// Check file
	if(!ti68k_is_a_img_file(filename))
	{
		fprintf(stderr, "Images must have '.img' extension.\n", filename);
      	return ERR_68K_CANT_OPEN;
	}
	
	// Open dest file
  	f = fopen(filename, "rb");
  	if(f == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", filename);
      	return ERR_68K_CANT_OPEN;
    }
    
    // Read header
    fread(ri, sizeof(IMG_INFO), 1, f);
    if(strcmp(ri->signature, "TiEmu img v2.00"))
   	{
   		fprintf(stderr, "Bad image: <%s>\n", filename);
      	return ERR_68K_INVALID_FLASH;
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
	ROM_INFO ri;
	char *ext;
	gchar *basename;
	IMG_INFO header;
	int i;

	// Preload romdump
	err = ti68k_get_rom_infos(srcname, &ri, !0);
	if(err)
    {
      	DISPLAY(_("Unable to get informations on ROM dump.\n"));
      	return err;
    }
	ti68k_display_rom_infos(&ri);

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
      	return ERR_68K_CANT_OPEN;
    }

	// Fill header
	strcpy(header.signature, "TiEmu img v2.00");
	header.header_size = sizeof(IMG_INFO);
	header.data_offset = 0x40;
	header.calc_type = ri.calc_type;
	memcpy(&header.revision, &ri.version, 4);
	header.internal = ri.internal;
	header.flash = ri.flash;
	header.has_boot = !ri.tib;
	header.data_size = ri.size;

	// Write file
	fwrite(&header, 1, sizeof(IMG_INFO), f);
	for(i = header.header_size; i < header.data_offset; i++)
		fputc(0, f);
	fwrite(ri.content, sizeof(char), ri.size, f);

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
	TIB_INFO ri;
	char *ext;
	gchar *basename;
	IMG_INFO header;
	int i, j;
	int num_blocks, last_block;

	// Preload upgrade
	err = ti68k_get_tib_infos(srcname, &ri, !0);
	if(err)
    {
      	DISPLAY(_("Unable to get informations on FLASH upgrade.\n"));
      	return err;
    }
	ti68k_display_tib_infos(&ri);

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
      	return ERR_68K_CANT_OPEN;
    }

	// Fill header
	strcpy(header.signature, "TiEmu img v2.00");
	header.header_size = sizeof(IMG_INFO);
	header.data_offset = 0x40;
	header.calc_type = ri.calc_type;
	memcpy(&header.revision, &ri.version, 4);
	header.internal = ri.internal;
	header.flash = ri.flash;
	header.has_boot = !ri.tib;
	header.data_size = 2 << 20;	//ri.size;
	
	// Write header
	fwrite(&header, 1, sizeof(IMG_INFO), f);
	for(i = header.header_size; i < header.data_offset; i++)
		fputc(0, f);
  
  	// Write boot block
  	for(i=0; i<0x05; i++)
    	fputc(0xff, f);
    
    if(ri.internal)
    	fputc(0x20, f); 	// internal
  	else
    	fputc(0x40, f); 	// external

  	for(i=0x06; i<0x65; i++)
    	fputc(0xff, f);

  	fputc(0xf0, f); 		// FLASH ROM

  	for(i = 0x66; i < 0x12000; i++)
    	fputc(0xff, f);
  
  	// Copy FLASH upgrade
  	num_blocks = ri.size / 65536;
  	for(i = 0; i < num_blocks; i++ )
    {
      	DISPLAY(".");
      	fflush(stdout);

      	fwrite(&ri.content[65536 * i], sizeof(char), 65536, f);
    }

  	last_block = ri.size % 65536;
   	fwrite(&ri.content[65536 * i], sizeof(char), last_block, f);
  
  	DISPLAY("\n");
  	DISPLAY("Completing to 2MB size\n");
  	for(j = SPP + ri.size; j < (2 << 20); j++)
  		fputc(0xff, f);
  
  	// Close file
  	fclose(f);

  	return 0;
}


/*
  	This function loads an image.
*/
int ti68k_load_image(const char *filename)
{
	IMG_INFO *header = &current_img_info;
  	FILE *f;  	
  	int err;

	// Load infos
	err = ti68k_get_img_infos(filename, header);
  	if(err)
    {
      	DISPLAY(_("Unable to get informations on ROM image.\n"));
      	return err;
    }
	ti68k_display_img_infos(header);
	
	// Open file
	f = fopen(filename, "rb");
	if(f == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", filename);
      	return ERR_68K_CANT_OPEN;
    }

	// Read pure data
    fseek(f, header->data_offset, SEEK_SET);
	header->content = malloc(header->data_size + 4);
    fread(header->content, 1, header->data_size, f);	
  	
  	params.rom_size = header->data_size;
  	params.ram_size = (header->data_size == 1024*1024) ? 128 : 256;
  
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
	Ti9xFlash content;
	Ti9xFlash *ptr;
	int nheaders;
	int i;
	IMG_INFO *header = &current_img_info;
  	int err;

	err = ti68k_get_tib_infos(filename, header, !0);
	if(err)
    {
      	DISPLAY(_("Unable to get informations on FLASH upgrade.\n"));
      	return err;
    }
	ti68k_display_img_infos(header);

  	params.rom_size = header->data_size;
  	params.ram_size = (header->data_size == 1024*1024) ? 128 : 256;

  	img_loaded = 2;
	return 0;
}


/*
  	Scan ROM images in a given directory 
  	and build the cache file.
*/
int ti68k_scan_files(const char *dirname, const char *filename)
{
	FILE *file;
	IMG_INFO ii;
	TIB_INFO ti;
	GDir *dir;
	GError *error = NULL;
	G_CONST_RETURN gchar *dirent;
	char buffer[1024];
	char str[20];
	char *rom_names[256] = { 0 };
	int nlines;
	int i,j;
	gchar *path;
	struct stat f_info;
	int ret;  	
  	char *line[7];
	char version[5];
	char *p1, *p2, *p3, *p5;
	int err;

  	DISPLAY(_("Scanning images/upgrades... "));

  	// First, check if cache file exists
  	if(!access(filename, F_OK))
    {
      	// if yes, ...
      	file = fopen(filename, "rt");
      	if(file == NULL)
		{
	  		fprintf(stderr, _("Unable to open this file: <%s>\n"), filename);
	  		return ERR_68K_CANT_OPEN;
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
          	return ERR_68K_CANT_OPEN;
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
						"filename", "calctype", "revision", 
						"memtype", "size", "boot");*/
    }  

  	// List all files available in the directory
	dir = g_dir_open(dirname, 0, &error);
	if (dir == NULL) 
	{
		fprintf(stderr, _("Opendir error\n"));
      	return ERR_68K_CANT_OPEN_DIR;
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
				fprintf(stderr, _("Can not stat: <%s> %i\n"), dirent, err);
	      		perror("stat: ");
			}
			else
			{
				if(ti68k_is_a_tib_file(path))
				{
					ret = ti68k_get_tib_infos(path, &ti, 0);
					if(ret)
					{
						fprintf(stderr, _("Can not get ROM/update info: <%s>\n"), path);
						break;
					}
					ii.calc_type = ti.calc_type;
					memcpy(ii.revision, ti.version, 4);
					ii.internal = ti.internal;
					ii.flash = ti.flash;
					ii.has_boot = !ti.tib;
					ii.data_size = ti.size;
				}
				else if(ti68k_is_a_img_file(path))
				{
					ret = ti68k_get_img_infos(path, &ii, 0);
					if(ret)
					{
						fprintf(stderr, _("Can not get ROM/update info: <%s>\n"), path);
						break;
					}
				}
				else
					continue;

		  		line[0] = dirent;
		  		line[1] = ti68k_calctype_to_string(ii.calc_type);
				memcpy(version, ii.revision, 4); version[4] = '\0';
	  			line[2] = version;
	  			line[3] = ti68k_romtype_to_string(ii.internal | ii.flash);
	  			sprintf(str, "%iKB", ii.data_size >> 10);
	  			line[4] = str;
	  			if(ii.has_boot)
	  				line[5] = _("yes");
	  			else
	  				line[5] = _("no");
		  
		  			fprintf(file, "%s\t%s\t%s\t%s\t%s\t%s\n", 
		  				line[0], line[1], line[2], 
		  				line[3], line[4], line[5]);
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

  	for (i = 0x12000; i < size-16; i += 2)
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
