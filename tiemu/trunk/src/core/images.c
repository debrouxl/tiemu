/* Hey EMACS -*- linux-c -*- */
/* $Id$ */

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
  TI's ROMs: loading of images (ROM image or FLASH upgrade)
  0x12000 is the beginning of the system privileged part
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
# include <glib.h>

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
//#include "platform.h"
#include "m68k.h"
#include "bkpts.h"

#define is_num(c)   isdigit(c)
#define is_alnum(c) isalnum(c)

static int get_rom_version(UBYTE *ptr, int size, char *version);

/*********************/
/* Stating functions */
/*********************/

/*
  Try to get some informations on the ROM image:
  - size
  - internal/external
  - FLASH/EPROM
  - soft version
  - calc type
*/
int ti68k_getImageInfo(const char *filename, ROM_INFO *ri)
{
  FILE *file;
  UBYTE *rom;

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
  
  // Load ROM into memory (a bit boring since a ROM is 2MB wide)
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

  free(rom);

  return 0;
}

#ifdef __WIN32__
# define strcasecmp _stricmp
#endif

/*
  Try to get some informations on the FLASH upgrade:
  - size
  - internal/external
  - soft version
  - calc type
*/
int ti68k_getUpgradeInfo(const char *filename, ROM_INFO *ri)
{
  FILE *file;
  char str[128];
  uint32_t flash_size;
  int i, j;
  uint8_t str_size;
  char date[5];
  char *signature = "Advanced Mathematics Software";
  int tib = 0;
  char *ext = NULL;
  UBYTE *rom;

  ext = strrchr(filename, '.');
  if(ext == NULL)
    return ERR_68K_TI_FILE;
  
  if(!strcasecmp(ext, ".89u"))
    ri->calc_type = TI89;
  else if(!strcasecmp(ext, ".9Xu"))
    ri->calc_type = TI92;
  else
    return ERR_68K_INVALID_FLASH;

  file = fopen(filename, "rb");
  if(file == NULL)
    {
      fprintf(stderr, _("Unable to open this file: <%s>\n"), filename);
      return ERR_68K_CANT_OPEN;
    }

  /* Check whether we have a .89u/.9xu or a .tib file */
  fgets(str, 128, file);
  if(strstr(str, "**TIFL**") == NULL) // is a .89u file
    {
      for(i=0, j=0; i<127; i++) // is a .tib file
        {
          if(str[i] == signature[j])
            {
              j++;
              if(j==strlen(signature))
                {
                  //DISPLAY(_("TIB file.\n"));
                  tib = 1;
                  break;
                }
            }
        }
      if(j < strlen(signature))
        return ERR_68K_INVALID_FLASH; // not a FLASH file
    }

  /* Now, we read it */
  rewind(file);
  if(!tib)
    {
      /* If a .89u/.9xu file, we skip the licence header */
      fgets(str, 9, file);
      if(strcmp(str, "**TIFL**"))
        return ERR_68K_INVALID_FLASH;
      for(i=0; i<4; i++)
        fgetc(file);

      for(i=0; i<4; i++)
        date[i] = fgetc(file);
      //DISPLAY("Date of the FLASHapp or License: %02X/%02X/%02X%02X\n",
      //        date[0], date[1], date[2], 0xff & date[3]);
      str_size=fgetc(file);
      for(i=0; i<str_size; i++)
        str[i]=fgetc(file);
      str[i]='\0';
      for(i=16+str_size+1; i<0x4A; i++)
        fgetc(file);
      flash_size = fgetc(file);
      flash_size += (fgetc(file) << 8);
      flash_size += (fgetc(file) << 16);
      flash_size += (fgetc(file) << 24);
      
      if(!strcmp(str, "License"))
        {
          //DISPLAY("There is a license header: skipped.\n");
          for(i=0; i<flash_size; i++)
            fgetc(file);

          fgets(str, 9, file);
          if(strcmp(str, "**TIFL**"))
            return ERR_68K_INVALID_FLASH;
          for(i=0; i<4; i++) fgetc(file);
          for(i=0; i<4; i++)
            date[i] = 0xff & fgetc(file);
          //DISPLAY("Date of the FLASHapp or License: %02X/%02X/%02X%02X\n",
          //        date[0], date[1], date[2], 0xff & date[3]);
          str_size=fgetc(file);
	  for(i=0; i<str_size; i++)
            str[i]=fgetc(file);
          str[i]='\0';
          for(i=16+str_size+1; i<0x4A; i++)
            fgetc(file);
	  flash_size = fgetc(file);
	  flash_size += (fgetc(file) << 8);
	  flash_size += (fgetc(file) << 16);
	  flash_size += (fgetc(file) << 24);
        }
    }
  else
    {
      /* else, we can read it directly */
      fseek(file, 0, SEEK_END);
      flash_size = ftell(file);
      fseek(file, 0, SEEK_SET);
      strcpy(str, "basecode");
    }

  // Load TIB into memory (a bit boring since a ROM is 2MB wide)
  rom = malloc(0x12000 + flash_size + 4);
  memset(rom, 0xff, 0x12000 + flash_size);

  /* Now, read data from file */
  fread(rom+0x12000, 1, flash_size, file);
  fclose(file);

  /* Update current ROM infos */
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
  ri->size = flash_size+0x12000;

  get_rom_version(rom, ri->size, ri->version);

  free(rom);

  return 0;
}


int ti68k_getFileInfo(const char *filename, ROM_INFO *ri)
{
  char *ext = NULL;

  ext = strrchr(filename, '.');

  if(!str_cmp(ext, ".rom"))
    return ti68k_getImageInfo(filename, ri); // a ROM image
  else
    return ti68k_getUpgradeInfo(filename, ri); // a FLASH upgrade

  return 0;
}


/*********************/
/* Loading functions */
/*********************/

/*
  Load a ROM image (image got with a ROM dumper)
*/
int ti68k_loadRom(const char *filename)
{
  FILE *file; 
  ROM_INFO *cri = &current_rom_info;
  int err;

  if((err=ti68k_getImageInfo(filename, cri)))
    {
      DISPLAY(_("Unable to get informations on ROM image.\n"));
      return err;
    }

  DISPLAY(_("ROM image informations:\n"));
  DISPLAY(_("  Calculator : %s"), 
	  (cri->calc_type & TI92) ? "TI92" : "TI89");
  DISPLAY(_("%c\n"), 
	  (cri->calc_type & MODULEPLUS) ? '+' : '\0');
  DISPLAY(_("  Version : %s\n"), cri->version);
  DISPLAY(_("  Type : %s "), (cri->internal) ? "internal" : "external");
  DISPLAY(_("%s\n"), (cri->flash) ? "FLASH" : "PROM");
  //DISPLAY(_("  Size : %i bytes (%1.2f MB)\n"), cri->size,  (float)((cri->size >> 10) / 1024.0));
  DISPLAY(_("  Size : %i bytes\n"), cri->size);
  
  params.rom_size = cri->size;
  params.ram_size = (cri->size == 1024*1024) ? 128 : 256;
  
  file = fopen(filename, "rb");
  cri->content = malloc(cri->size);
  if(cri->content == NULL)
    return -1;
  memset(cri->content, 0xff, cri->size);
  fread(cri->content, 1, cri->size, file);
  fclose(file);
  
  cri->loaded = 1;
  return 0;
}

/*
  Convert a FLASH upgrade into a ROM image. The image does have boot block
  not certificate.
*/
int ti68k_convertTibToRom(const char *f_in, char *f_out)
{
  const char *filename = f_in;
  char *filename2 = f_out;
  FILE *file, *fo;
  uint8_t data;
  char str[128];
  uint32_t flash_size;
  int i, j;
  int num_blocks;
  uint16_t last_block;
  uint8_t str_size;
  char date[5];
  char *ext;
  char *signature = "Advanced Mathematics Software";
  int tib = 0;
  int calc_type;
  long offset;
  uint8_t buffer[65536];

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
    strcpy(filename2, filename);
  strcat(filename2, "_");
  strcat(filename2, ".rom");

  fo = fopen(filename2, "wb");
  if(fo == NULL)
    {
      fprintf(stderr, "Unable to open this file: <%s>\n", filename2);
      return ERR_68K_CANT_OPEN;
    }

  /* Check whether we have a .89u/.9xu or a .tib file */
  fgets(str, 128, file);
  if(strstr(str, "**TIFL**") == NULL) // is a .89u file
    {
      for(i=0, j=0; i<127; i++) // is a .tib file
	{
	  if(str[i] == signature[j])
	    {
	      j++;
	      if(j==strlen(signature))
		{
		  tib = 1;
		  break;
		}
	    }
	}
      if(j < strlen(signature))
	return ERR_68K_TI_FILE; // not a FLASH file
    }

  /* Now, we convert it */
  rewind(file);
  if(!tib)
    {
      /* If a .89u/.9xu file, we skip the licence header */
      fgets(str, 9, file);
      if(strcmp(str, "**TIFL**")) 
	return ERR_68K_INVALID_FLASH;
      for(i=0; i<4; i++) 
	fgetc(file);
      
      for(i=0; i<4; i++)
	date[i] = fgetc(file);
      DISPLAY("Date of the FLASHapp or License: %02X/%02X/%02X%02X\n", 
	      date[0], date[1], date[2], 0xff & date[3]);
      str_size=fgetc(file);
      for(i=0; i<str_size; i++)
	str[i]=fgetc(file);
      str[i]='\0';
      for(i=16+str_size+1; i<0x4A; i++)
	fgetc(file);
      flash_size = fgetc(file);
      flash_size += (fgetc(file) << 8);
      flash_size += (fgetc(file) << 16);
      flash_size += (fgetc(file) << 24);
      
      if(!strcmp(str, "License"))
	{
	  DISPLAY("There is a license header: skipped.\n");
	  for(i=0; i<flash_size; i++)
	    fgetc(file);
	  
	  fgets(str, 9, file);
	  if(strcmp(str, "**TIFL**"))
	    return ERR_68K_INVALID_FILE;
	  for(i=0; i<4; i++) fgetc(file);
	  for(i=0; i<4; i++)
	    date[i] = 0xff & fgetc(file);
	  DISPLAY("Date of the FLASHapp or License: %02X/%02X/%02X%02X\n", 
		  date[0], date[1], date[2], 0xff & date[3]);
	  str_size=fgetc(file);
	  for(i=0; i<str_size; i++)
	    str[i]=fgetc(file);
	  str[i]='\0';
	  for(i=16+str_size+1; i<0x4A; i++)
	    fgetc(file);
      flash_size = fgetc(file);
      flash_size += (fgetc(file) << 8);
      flash_size += (fgetc(file) << 16);
      flash_size += (fgetc(file) << 24);
	}
    }
  else
    {
      /* else, we can read it directly */
      fseek(file, 0, SEEK_END);
      flash_size = ftell(file);
      fseek(file, 0, SEEK_SET);
      strcpy(str, "basecode");
    }       
  
  DISPLAY("FLASH application name: \"%s\"\n", str);
  DISPLAY("FLASH application/Operating System size: %i bytes.\n", flash_size);

  /* Now, read data from file and convert it */
  num_blocks = flash_size/65536;
  DISPLAY("Number of blocks: %i\n", num_blocks + 1);
  DISPLAY("Converting: ");

  /* Boot block */
  for(i=0; i<0x05; i++)
    fputc(0xff, fo);

  offset = ftell(file);
  fseek(file, 0x8d, SEEK_CUR); // MSB of the PC reset vector
  data = fgetc(file);
  if((data&0x60)==0x20) // internal (0x200000)  or external (0x400000)
    calc_type=TI89; 
  else
    calc_type=TI92;
  fseek(file, offset, SEEK_SET);

  if(calc_type == TI89)
    fputc(0x20, fo); // internal
  else
    fputc(0x40, fo); // external

  for(i=0x06; i<0x65; i++)
    fputc(0xff, fo);

  fputc(0xf0, fo); // FLASH ROM

  for(i=0x66; i<0x12000; i++)
    fputc(0xff, fo);
  
  /* FLASH upgrade */
  for(i=0; i<num_blocks; i++ )
    {
      DISPLAY(".", i);
      fflush(stdout);

      /*
      for(j=0; j<65536; j++)
 	{
	  data=fgetc(file);
	  fputc(data, fo);
	}
      */
      fread(buffer, 1, 65536, file);
      fwrite(buffer, 1, 65536, fo);
    }

  DISPLAY(".");
  fflush(stdout);
  last_block=flash_size % 65536;
  for(j=0; j<last_block; j++)
    {
      data=fgetc(file);
      fputc(data, fo);
    }
  DISPLAY("\n");
  DISPLAY("Completing to 2MB size\n");
  for(j=0x12000+flash_size; j<2*1024*1024; j++)
  fputc(0xff, fo);
  fclose(file);
  fclose(fo);

  return 0;
}

/*
  This function load a ROM image or a FLASH upgrade (converted into image).
*/
int ti68k_loadImage(const char *filename)
{
  char *ext = NULL;
  char filename2[MAXCHARS];

  ext = strrchr(filename, '.');

  if(!str_cmp(ext, ".rom"))
    return ti68k_loadRom(filename); // a ROM image
  else {
    ti68k_convertTibToRom(filename, filename2);
    return ti68k_loadRom(filename2); // a FLASH upgrade
  }  

  return 0;
}

/*
  Load a FLASH upgrade (.tib/.9xu/.89u).
  Note: a ROM image must have been loaded before calling this function.
*/
int ti68k_loadUpgrade(const char *filename)
{
  FILE *file;
  char str[128];
  uint32_t flash_size;
  int i, j;
  uint8_t str_size;
  char date[5];
  char *signature = "Advanced Mathematics Software";
  int tib = 0;
  ROM_INFO *cri = &current_rom_info;
  int err;

  if((err=ti68k_getUpgradeInfo(filename, cri)))
    {
      DISPLAY(_("Unable to get informations on FLASH upgrade.\n"));
      return err;
    }

  DISPLAY(_("FLASH upgrade informations:\n"));
  DISPLAY(_("  Calculator : %s"), 
	  (cri->calc_type & TI92) ? "TI92" : "TI89");
  DISPLAY(_("%c\n"), 
	  (cri->calc_type & MODULEPLUS) ? '+' : '\0');
  DISPLAY(_("  Version : %s\n"), cri->version);
  DISPLAY(_("  Type : %s "), (cri->internal) ? "internal" : "external");
  DISPLAY(_("%s\n"), (cri->flash) ? "FLASH" : "PROM");
  DISPLAY(_("  Size : %i bytes (%1.2f MB)\n"), cri->size, 
	  (float)(cri->size >> 10)/1024.0);

  params.rom_size = cri->size;
  params.ram_size = (cri->size == 1024*1024) ? 128 : 256;

  file = fopen(filename, "rb");
  if(file == NULL)
    {
      fprintf(stderr, _("Unable to open this file: <%s>\n"), filename);
      return ERR_68K_CANT_OPEN;
    }

  /* Check whether we have a .89u/.9xu or a .tib file */
  fgets(str, 128, file);
  if(strstr(str, "**TIFL**") == NULL) // is a .89u file
    {
      for(i=0, j=0; i<127; i++) // is a .tib file
        {
          if(str[i] == signature[j])
            {
              j++;
              if(j==strlen(signature))
                {
                  //DISPLAY(_("TIB file.\n"));
                  tib = 1;
                  break;
                }
            }
        }
      if(j < strlen(signature))
        return ERR_68K_INVALID_FLASH; // not a FLASH file
    }

  /* Now, we read it */
  rewind(file);
  if(!tib)
    {
      /* If a .89u/.9xu file, we skip the licence header */
      fgets(str, 9, file);
      if(strcmp(str, "**TIFL**"))
        return ERR_68K_INVALID_FLASH;
      for(i=0; i<4; i++)
        fgetc(file);

      for(i=0; i<4; i++)
        date[i] = fgetc(file);
      //DISPLAY("Date of the FLASHapp or License: %02X/%02X/%02X%02X\n",
      //        date[0], date[1], date[2], 0xff & date[3]);
      str_size=fgetc(file);
      for(i=0; i<str_size; i++)
        str[i]=fgetc(file);
      str[i]='\0';
      for(i=16+str_size+1; i<0x4A; i++)
        fgetc(file);
      flash_size  =  LSB(fgetc(file));
      flash_size += (LSB(fgetc(file)) << 8);
      flash_size += (LSB(fgetc(file)) << 16);
      flash_size += (LSB(fgetc(file)) << 24);

      if(!strcmp(str, "License"))
        {
          //DISPLAY("There is a license header: skipped.\n");
          for(i=0; i<flash_size; i++)
            fgetc(file);

          fgets(str, 9, file);
          if(strcmp(str, "**TIFL**"))
            return ERR_68K_INVALID_FLASH;
          for(i=0; i<4; i++) fgetc(file);
          for(i=0; i<4; i++)
            date[i] = 0xff & fgetc(file);
          //DISPLAY("Date of the FLASHapp or License: %02X/%02X/%02X%02X\n",
          //        date[0], date[1], date[2], 0xff & date[3]);
          str_size=fgetc(file);
	  for(i=0; i<str_size; i++)
            str[i]=fgetc(file);
          str[i]='\0';
          for(i=16+str_size+1; i<0x4A; i++)
            fgetc(file);
	  flash_size = fgetc(file);
	  flash_size += (fgetc(file) << 8);
	  flash_size += (fgetc(file) << 16);
	  flash_size += (fgetc(file) << 24);
        }
    }
  else
    {
      /* else, we can read it directly */
      fseek(file, 0, SEEK_END);
      flash_size = ftell(file);
      fseek(file, 0, SEEK_SET);
      strcpy(str, "basecode");
    }

  DISPLAY(_("FLASH application/Operating System size: %i bytes.\n"), 
	  flash_size);

  /* Now, read data from file */
  cri->content = malloc(cri->size);
  if(cri->content == NULL)
    return -1;

  fread(cri->content+0x12000, 1, flash_size, file);
  fclose(file);

  memcpy(ti_rom+0x12000, cri->content+0x12000, cri->size-0x12000);
  free(cri->content);

  cri->loaded = 2;
  return 0;
}

/*
  Return ROM type: internal/external, EPROM/FLASH
*/
int ti68k_getRomType(void)
{
  int internal, flashrom;

  internal = ((ti_rom[5] & 0x60) == 0x20) ? INTERNAL : 0;
  flashrom = (ti_rom[0x65] & 0xf) ? 0 : FLASH_ROM;
  
  return internal | flashrom;
}

/********/
/* Misc */
/********/

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
static int get_rom_version(UBYTE *ptr, int size, char *version)
{
  int i;

  strcpy(version, "?.??");

  for (i=0x12000; i<size-16; i+=2)
    {
      if (is_num(ptr[i])&&(ptr[i+1]=='.') && is_num(ptr[i+2]) &&
	  (ptr[i+3]==0)&&is_alnum(ptr[i+4]) && is_alnum(ptr[i+5]) &&
	  (ptr[i+6]=='/')&&is_alnum(ptr[i+7]) && is_alnum(ptr[i+8]) &&
	  (ptr[i+9]=='/')&&is_alnum(ptr[i+10]) && is_alnum(ptr[i+11]))
	{
	  break;
	}
      if (is_num(ptr[i]) && (ptr[i+1]=='.') && is_num(ptr[i+2]) && 
	  is_num(ptr[i+3]) && (ptr[i+4]==0) && is_alnum(ptr[i+5]) && 
	  is_alnum(ptr[i+6]) && (ptr[i+7]=='/') && is_alnum(ptr[i+8]) && 
	  is_alnum(ptr[i+9]) && (ptr[i+10]=='/') && is_alnum(ptr[i+11]) && 
	  is_alnum(ptr[i+12]))
	{
	  break;
	}
      if (is_num(ptr[i]) && (ptr[i+1]=='.') && is_num(ptr[i+2]) && 
	  (ptr[i+3]==0) && is_alnum(ptr[i+4]) && is_alnum(ptr[i+5]) && 
	  is_alnum(ptr[i+6]) && is_alnum(ptr[i+7]) && is_alnum(ptr[i+8]) && 
	  is_alnum(ptr[i+9]) && is_alnum(ptr[i+10]) && is_alnum(ptr[i+11]))
	{
	  break;
	}
      if (is_num(ptr[i]) && (ptr[i+1]=='.') && is_num(ptr[i+2]) && 
	  is_alnum(ptr[i+3]) && (ptr[i+4]==0) && is_alnum(ptr[i+5]) && 
	  is_alnum(ptr[i+6]) && is_alnum(ptr[i+7]) && is_alnum(ptr[i+8]) && 
	  is_alnum(ptr[i+9]) && is_alnum(ptr[i+10]) && is_alnum(ptr[i+11]))
	{
	  break;
	}
    }
  if (i<size-16) 
    {
      int n;
      for (n=i; n<i+16; n++) 
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
