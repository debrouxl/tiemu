/*  tilp - a linking program for TI graphing calculators
 *  Copyright (C) 1999-2002  Romain Lievin
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
  Windows .ico files management routines.
  Read support only. 16/256 colors only. Does not manage alpha channel (yet).
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#else
#include "win32/unistd.h"
#endif

#include "fmt.h"
#include "bmpfile.h"


/***********/
/* Writing */
/***********/

int write_ico_format(FILE *file, Image *img)
{
  return 0;
}


/***********/
/* Reading */
/***********/

int read_ico_format(FILE *file, Image *img) //tested: OK (12/09/2002)
{
  DWORD			i;
  WORD			wUnused;
  WORD			wEntries;
  ICONENTRY		icoEntry, iUnused;
  BITMAPINFOHEADER	icoInfoHeader;
  BYTE			*andBitmap;
  BYTE			*PtrSource, *PtrTarget, *PtrDummy;
  int                   BytesPerLine;
  
  fread(&wUnused, sizeof(WORD), 1, file);  // reserved
  fread(&wUnused, sizeof(WORD), 1, file);  // type
  fread(&wEntries, sizeof(WORD), 1, file); // # of icons
  
  fread(&icoEntry, sizeof(ICONENTRY), 1, file); // first icon entry
  for(i=1; i<wEntries; i++)			// ignore others
    fread(&iUnused, sizeof(ICONENTRY), 1, file);
  fread(&icoInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);	// icon header
  
  if( (icoEntry.bColorCount != 16) & (icoEntry.bColorCount != 0) )
    {
      printf("#colors = %i\n", icoEntry.bColorCount);
      return -1;
    }	
  
  BytesPerLine = 4*((icoEntry.bWidth+3)/4);	/* Z! Modulo 4 */
  
  clear_image(img);
  img->depth    = icoEntry.bColorCount == 0 ? 256 : icoEntry.bColorCount;
  img->width    = icoEntry.bWidth;
  img->height   = icoEntry.bHeight;
  img->encoding = !0;
  img->bytemap  = (uint8_t *)malloc(icoEntry.bWidth * icoEntry.bHeight);
  img->colormap = (uint8_t *)malloc(3*256 * sizeof(uint8_t));
  
  // load colormap
  for(i=0; i<(DWORD)img->depth; i++)		// color map
    {
      BYTE r, g, b, u;
      
      fread(&r, sizeof(BYTE), 1, file);
      fread(&g, sizeof(BYTE), 1, file);
      fread(&b, sizeof(BYTE), 1, file);
      fread(&u, sizeof(BYTE), 1, file);
      
      (img->colormap)[3*i+0] = r;
      (img->colormap)[3*i+1] = g;
      (img->colormap)[3*i+2] = b;
    }
  
  // load XORbitmap (color bitmap)
  if(icoEntry.bColorCount == 16)
    {
      for(i=0; i<(DWORD)icoEntry.bWidth * icoEntry.bHeight; i+=2)
	{
	  BYTE data = fgetc(file);
	  img->bytemap[i+0] = (data & 0xf0) >> 4;
	  img->bytemap[i+1] = (data & 0x0f);
	}
    }
  else
    {
      for(i=0; i<(DWORD)icoEntry.bWidth * icoEntry.bHeight; i++)
	{
	  img->bytemap[i] = fgetc(file);
	}
    }
  
  // and invert order (Z order)
  PtrSource = img->bytemap + ((icoEntry.bHeight-1)*BytesPerLine);	// beginning of last scanline
  PtrTarget = img->bytemap;						// first scan line
  PtrDummy = (BYTE *)malloc(BytesPerLine);   
  
  for (i=0; i<(DWORD)icoEntry.bHeight/2; i++)
    {
      memcpy(PtrDummy,  PtrTarget, BytesPerLine);
      memcpy(PtrTarget, PtrSource, BytesPerLine);
      memcpy(PtrSource, PtrDummy, BytesPerLine);  
      
      PtrTarget += BytesPerLine;
      PtrSource -= BytesPerLine;
    }
  free(PtrDummy);
  
  // load ANDbitmap (alpha channel)
  andBitmap = (uint8_t *)malloc(icoInfoHeader.biWidth*icoInfoHeader.biHeight/8 * sizeof(uint8_t));
  for(i=0; i<(DWORD)icoInfoHeader.biWidth*icoInfoHeader.biHeight/8; i++)
    {
      andBitmap[i] = fgetc(file);
    }
  
  // infos
  /*
    printf("# entries     : %i\n", wEntries);
    printf("cursor width  : %i\n", icoEntry.bWidth);
    printf("cursor height : %i\n", icoEntry.bHeight);
    printf("color count   : %i\n", icoEntry.bColorCount);
    printf("bit count     : %i\n", icoEntry.bBitCount);
    printf("size in bytes : %i\n", icoEntry.dwSizeInBytes);
    printf("file offset   : %i\n", icoEntry.dwFileOffset);
    printf("\n");
    printf("width		: %i\n", icoInfoHeader.biWidth);
    printf("height		: %i\n", icoInfoHeader.biHeight);
    printf("planes		: %i\n", icoInfoHeader.biPlanes);
    printf("bit count	: %i\n", icoInfoHeader.biBitCount);
    printf("compression	: %i\n", icoInfoHeader.biCompression);
    printf("image size	: %i\n", icoInfoHeader.biSizeImage);
    printf("Xpixels		: %i\n", icoInfoHeader.biXPelsPerMeter);
    printf("Ypixels		: %i\n", icoInfoHeader.biYPelsPerMeter);
    printf("color used	: %i\n", icoInfoHeader.biClrUsed);
    printf("color impor	: %i\n", icoInfoHeader.biClrImportant);
  */
  
  return 0;
}
