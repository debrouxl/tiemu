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
  This file contains utility functions about files, attributes,
  sorting routines for selection, conversion routines between dirlist
  and glists.
 */

#ifdef __WIN32__
# include <windows.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "tilibs.h"

#include "struct.h"
#include "support.h"
#include "paths.h"

/*******************************/
/* File manipulation functions */
/*******************************/

/*
  Copy a file from src to dst 
*/
int copy_file(char *src, char *dst)
{
  gchar *cmd;

  cmd = g_strdup_printf("cp %s %s", src, dst);

  return system(cmd); //copy_file(src, dst);
  /*
  FILE *in, *out;
  int c;

   if((in=fopen(src, "rb")) == NULL)
    {
      return 1;
    }
   if((out=fopen(dst, "wb")) == NULL)
     {
      return 2;
    }
   while(!feof(in))
     {
	   c=fgetc(in);
	   if(feof(in)) break;
       fputc(c, out);
     }
   fclose(in);
   fclose(out);
  */
  return 0;
}

/*
  Move the file
 */
int move_file(char *src, char *dst)
{
  gchar *cmd;

  cmd = g_strdup_printf("mv %s %s", src, dst);

  return system(cmd); //copy_file(src, dst);
  /*
  int ret;

  ret=copy_file(src, dst);
  if(ret) return ret;
  unlink(src);
  */
  return 0;
}

/* Remove '\r' characters for GtkText */
void process_buffer(gchar *buf)
{
  gint i;
  
  for(i = 0; i < (int)strlen(buf); i++)
    if(buf[i] == '\r') 
		buf[i]=' ';
}

/*************************************/
/* Extracting informations functions */
/*************************************/

/* Return the filename or its extension if it has one */
char *file_extension(char *filename)
{
  int i;
  char *p;
  
  for(i=strlen(filename); i > 0; i--)
    {
      if(filename[i] == '.') break;
    }
  p=filename+i+1;
  
  return p;
}
