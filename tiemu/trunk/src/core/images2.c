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
	This module handles loading of:
	- ROM dump
	- FLASH upgrade
	
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

static int get_rom_version(char *ptr, int size, char *version);

/*
	Display informations
*/
void ti68k_display_image_infos(ROM_INFO *ri)
{
	DISPLAY(_("Image informations:\n"));
  	DISPLAY(_("  Calculator : %s\n"), ti68k_calctype_to_string(ri->calc_type));
  	DISPLAY(_("  Firmware : v%s\n"), ri->version);
  	DISPLAY(_("  Memory type : %s\n"), ti68k_romtype_to_string(ri->internal | ri->flash));
  	DISPLAY(_("  Memory size : %i bytes (%iMB)\n"), ri->size,  ri->size >> 20);
}

/*
  Try to get some informations on the ROM dump:
  - size
  - internal/external
  - FLASH/EPROM
  - soft version
  - calc type
*/
int ti68k_getRomdumpInfo(const char *filename, ROM_INFO *ri, int keep)
{
  	FILE *file;
	char *rom;

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
  
  	rom = malloc(ri->size + 4);
  	memset(rom, 0xff, ri->size);
  	fread(rom, 1, ri->size, file);
  	fclose(file);

  	ri->internal = ((rom[0x05] & 0x60) == 0x20) ? (!0) : 0;
  	ri->flash = (rom[0x65] & 0xf) ? 0 : (!0);
  	ri->tib = 0;

  	if(ri->internal && ri->flash)
    	ri->calc_type = TI89;
  	else if (ri->flash)
    	ri->calc_type = TI92 | MODULEPLUS;
  	else 
    	ri->calc_type = TI92;
  
  	get_rom_version(rom, ri->size, ri->version);

	if(keep)
		ri->content = rom;
	else
		free(rom);

	return 0;
}


/*
  Try to get some informations on the FLASH upgrade:
  - size
  - internal/external
  - soft version
  - calc type
*/
int ti68k_getUpgradeInfo(const char *filename, ROM_INFO *ri, int keep)
{
	Ti9xFlash content;
	Ti9xFlash *ptr;
	int nheaders;
	int i;
  	char *rom;

	// Check valid file
	if(!tifiles_is_a_ti_file(filename))
		return ERR_68K_TI_FILE;
		
	if(!tifiles_is_a_flash_file(filename) && !tifiles_is_a_tib_file(filename))
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
  	rom = malloc(0x12000 + ptr->data_length + 4);
  	memset(rom, 0xff, 0x12000 + ptr->data_length);
  	memcpy(rom+0x12000, ptr->data_part, ptr->data_length);
  	
  	// Update current ROM infos
  	if (((rom+0x12000)[0x88+0x05] & 0x60) == 0x20)
    {
      	ri->internal = !0;
      	ri->calc_type = TI89;
    }
	else
    {
      	ri->internal = 0;
      	ri->calc_type = TI92 | MODULEPLUS;
    }
    
  	ri->flash = !0;
  	ri->tib = !0;
  	ri->size = ptr->data_length + 0x12000;

  	get_rom_version(rom, ri->size, ri->version);
  	
  	ti9x_free_flash_content(&content);
	if(keep)
		ri->content = rom;
	else
		free(rom);

  	return 0;
}


int ti68k_getFileInfo(const char *filename, ROM_INFO *ri)
{
  	char *ext = strrchr(filename, '.');

  	if(!strcasecmp(ext, ".rom"))
    	return ti68k_getRomdumpInfo(filename, ri, 0);		// a ROM dump
  	else if(tifiles_is_a_ti_file(filename))
    	return ti68k_getUpgradeInfo(filename, ri, 0);	// a FLASH upgrade
	else
		return ERR_68K_CANT_OPEN;

  	return 0;
}


/*
  	Load a ROM image (image got with a ROM dumper)
*/
int ti68k_loadRom(const char *filename)
{
  	FILE *file; 
  	ROM_INFO *cri = &current_rom_info;
  	int err;

  	if((err=ti68k_getRomdumpInfo(filename, cri, !0)))
    {
      	DISPLAY(_("Unable to get informations on ROM image.\n"));
      	return err;
    }

	ti68k_display_image_infos(cri);
  	
  	params.rom_size = cri->size;
  	params.ram_size = (cri->size == 1024*1024) ? 128 : 256;
  
  	cri->loaded = 1;

  	return 0;
}

/*
  	Convert an FLASH upgrade into a ROM image. 
  	The image have neither boot block nor certificate.
*/
int ti68k_convertTibToRom(const char *f_in, char *f_out)
{
	FILE *file, *fo;
	ROM_INFO ri;
	Ti9xFlash content;
	Ti9xFlash *ptr;
	const char *filename = f_in;
  	char *filename2 = f_out;
	int ret;
	char *data;
	int num_blocks, last_block;
	char *ext;
	int nheaders;
	int i, j;
  
  	// Check valid file
  	ret = ti68k_getUpgradeInfo(filename, &ri, 0);
  	if(ret)
  		return ret;

	// Create destination file
  	file = fopen(filename, "rb");
  	if(file == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", filename);
      	return ERR_68K_CANT_OPEN;
    }
  
  	ext = strrchr(filename, '.');
  	if(ext != NULL)
    {
      	strncpy(filename2, filename, strlen(filename)-strlen(ext));
      	filename2[strlen(filename)-strlen(ext)] = '\0';
    }  
  	else
  	{
    	strcpy(filename2, filename);
    }
  	strcat(filename2, "_");
  	strcat(filename2, ".rom");

	// Load source file
	ti9x_read_flash_file(filename, &content);
	
	// count headers
  	for (ptr = &content; ptr != NULL; ptr = ptr->next)
    	nheaders++;
  	
  	// keep the last one (data)
  	for (i = 0, ptr = &content; i < nheaders - 1; i++)
    	ptr = ptr->next;
    data = ptr->data_part;
    	
    // Open destination file
  	fo = fopen(filename2, "wb");
  	if(fo == NULL)
    {
      	fprintf(stderr, "Unable to open this file: <%s>\n", filename2);
      	return ERR_68K_CANT_OPEN;
    }
  
  	// Boot block
  	for(i=0; i<0x05; i++)
    	fputc(0xff, fo);
    
    if(ri.calc_type == TI89)
    	fputc(0x20, fo); 	// internal
  	else
    	fputc(0x40, fo); 	// external

  	for(i=0x06; i<0x65; i++)
    	fputc(0xff, fo);

  	fputc(0xf0, fo); 		// FLASH ROM

  	for(i = 0x66; i < 0x12000; i++)
    	fputc(0xff, fo);
  
  	// FLASH upgrade
  	num_blocks = ptr->data_length / 65536;
  	for(i = 0; i < num_blocks; i++ )
    {
      	DISPLAY(".");
      	fflush(stdout);

      	fwrite(&data[65536 * i], sizeof(char), 65536, fo);
    }

  	last_block = ptr->data_length % 65536;
   	fwrite(&data[65536 * i], sizeof(char), last_block, fo);
  
  	DISPLAY("\n");
  	DISPLAY("Completing to 2MB size\n");
  	for(j = 0x12000 + ptr->data_length; j < 2*1024*1024; j++)
  		fputc(0xff, fo);
  
  	fclose(file);
  	fclose(fo);

  	return 0;
}

/*
  	This function load a ROM image or an 
  	FLASH upgrade (converted into image).
*/
int ti68k_loadImage(const char *filename)
{
  	char *ext = strrchr(filename, '.');
  	char filename2[MAXCHARS];

  	if(!strcasecmp(ext, ".rom"))
    	return ti68k_loadRom(filename); 	// a ROM image
  	else 
  	{
    	ti68k_convertTibToRom(filename, filename2);
    	return ti68k_loadRom(filename2); 	// a FLASH upgrade
  	}  

  	return 0;
}

/*
  	Load an FLASH upgrade (.tib/.9xu/.89u).
  	Note: an ROM image must have been loaded 
  	before calling this function.
*/
int ti68k_loadUpgrade(const char *filename)
{
	Ti9xFlash content;
	Ti9xFlash *ptr;
	int nheaders;
	int i;
	ROM_INFO *cri = &current_rom_info;
  	int err;

	err = ti68k_getUpgradeInfo(filename, cri, !0);
	if(err)
    {
      DISPLAY(_("Unable to get informations on FLASH upgrade.\n"));
      return err;
    }

	ti68k_display_image_infos(cri);

  	params.rom_size = cri->size;
  	params.ram_size = (cri->size == 1024*1024) ? 128 : 256;

  	cri->loaded = 2;
	 
	return 0;
}

/*
  	Return ROM type:
  	- internal/external
  	- EPROM/FLASH
*/
int ti68k_getRomType(void)
{
  	int internal, flashrom;

  	internal = ((ti_rom[5] & 0x60) == 0x20) ? INTERNAL : 0;
  	flashrom = (ti_rom[0x65] & 0xf) ? 0 : FLASH_ROM;
  
  	return internal | flashrom;
}


/*
  	Scan ROM images in a given directory 
  	and build the cache file.
*/
int ti68k_scanFiles(const char *dirname, const char *filename)
{
	FILE *file;
	ROM_INFO ri;
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
  	char *line[6];
	char *p1, *p2, *p3, *p5;
	int err;  	

  	DISPLAY(_("Scanning ROM images... "));

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
    }  

  	// List all ROMs available in the directory
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
		
		// we have a new ROM, we add it in the cache
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
				ret = ti68k_getFileInfo(path, &ri);
	      		if(ret)
	      		{
	      			fprintf(stderr, _("Can not get ROM/update info: <%s>\n"), path);
	      		}
	      		else
				{
		  			line[0] = dirent;

		  			switch(ri.calc_type)
		    		{
		    			case TI92: line[1] = "TI92";
		      			break;
		    			case TI89: line[1] = "TI89";
		      			break;
		    			case TI92 | MODULEPLUS: line[1] = "TI92+";
		      			break;
		    			default: line[1] = "TI??";
		      			break;
		    		}
		  
		  			line[2] = ri.version;
		  			if(ri.internal)
		    			p2 = _("internal");
		  			else
		    			p2 = _("external");
		  			
		  			if(ri.flash)
		    			p3 = _("FLASH");
		  			else
		    			p3 = _("PROM");
		  			sprintf(buffer, "%s-%s", p3, p2);
		  			line[3] = buffer;
		  
		  			sprintf(str, "%iKB", ri.size >> 10);
		  			line[4] = str;
		  
		  			if(ri.tib)
		    			p5 = _("tib");
		  			else
		    			p5 = _("img");
		  			line[5] = p5;
		  
		  			fprintf(file, "%s\t%s\t%s\t%s\t%s\t%s\n", 
		  				line[0], line[1], line[2], 
		  				line[3], line[4], line[5]);
				}
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
