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

#import "SkinController.h"
#import "SkinView.h"
#import "SkineditController.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../src/defs.h"
#include "../src/struct.h"
#include "../src/keymaps.h"

#include "utils.h"

struct skinInfos skin_infos;

@implementation SkinController


// skin loading methods

- (int)loadSkin:(NSString *)path
{
    int ret;
    NSData *skin = nil;
    
    skin = [NSData dataWithContentsOfFile:path];
    
    if (skin == nil)
        return -1;
        
    if (strncmp([skin bytes], "VTIv2.1", 7) == 0)
        ret = [self loadSkinOldVTi:skin];
    else if (strncmp([skin bytes], "VTIv2.5", 7) == 0)
        ret = [self loadSkinVTi:skin];
    else
        ret = [self loadSkinTiEmu:skin];

    if (ret != 0)
      return -1;
        
    skin_infos.skin_path = strdup([path fileSystemRepresentation]);

    set_calc_keymap();
    
    return 0;
}


- (int)loadSkinVTi:(NSData *)skin
{
  uint32_t calc;
  int i;
  unsigned char *bytes = NULL;

  bytes = (unsigned char *)[skin bytes];
  
  if (bytes == NULL)
    return -1;

  bytes += 8;

  skin_infos.name = (char *)malloc(65);

  if (skin_infos.name != NULL)
  {
    memset(skin_infos.name, 0, 65);
    memcpy(skin_infos.name, bytes, 64);

    if (strlen(skin_infos.name) == 0)
    {
      free(skin_infos.name);
      skin_infos.name = NULL;
    }
    else
    {
      skin_infos.name = realloc(skin_infos.name, strlen(skin_infos.name) + 1);
      skin_infos.name[strlen(skin_infos.name) + 1] = 0;
    }
  }
  bytes += 64;


  skin_infos.author = (char *)malloc(65);

  if (skin_infos.author != NULL)
  {
    memset(skin_infos.author, 0, 65);
    memcpy(skin_infos.author, bytes, 64);

    if (strlen(skin_infos.author) == 0)
    {
      free(skin_infos.author);
      skin_infos.author = NULL;
    }
    else
    {
      skin_infos.author = realloc(skin_infos.author, strlen(skin_infos.author) + 1);
      skin_infos.author[strlen(skin_infos.author) + 1] = 0;
    }
  }
  bytes += 64;

  memcpy(&calc, bytes, 4);
  bytes += 4;

  memcpy(&skin_infos.colortype, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_white, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_black, bytes, 4);
  bytes += 4;

  memcpy(&skin_infos.lcd_pos.left, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.top, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.right, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.bottom, bytes, 4);
  bytes += 4;

  for (i = 0; i < 80; i++)
    {
      memcpy(&skin_infos.keys_pos[i].left, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].top, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].right, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].bottom, bytes, 4);
      bytes += 4;
    }

#if (G_BYTE_ORDER == G_BIG_ENDIAN) || defined(__MACOSX__)
  /*
   * VTI skins are usually designed on a little endian architecture
   */

  byteswap_vti_skin(&calc);
#endif

  vti_calc_type_to_string(calc);

  skin_infos.type = SKIN_TYPE_VTI;

  return [self loadJPEG:skin atOffset:1448];
}


- (int)loadSkinOldVTi:(NSData *)skin
{
  uint32_t calc;
  int i;
  unsigned char *bytes = NULL;

  bytes = (unsigned char *)[skin bytes];

  if (bytes == NULL)
    return -1;

  bytes += 8;

  skin_infos.name = (char *)malloc(65);

  if (skin_infos.name != NULL)
  {
    memset(skin_infos.name, 0, 65);
    memcpy(skin_infos.name, bytes, 64);

    if (strlen(skin_infos.name) == 0)
    {
      free(skin_infos.name);
      skin_infos.name = NULL;
    }
    else
    {
      skin_infos.name = realloc(skin_infos.name, strlen(skin_infos.name) + 1);
      skin_infos.name[strlen(skin_infos.name) + 1] = 0;
    }
  }
  bytes += 64;

  memcpy(&calc, bytes, 4);
  bytes += 4;
  
  memcpy(&skin_infos.colortype, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_white, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_black, bytes, 4);
  bytes += 4;

  memcpy(&skin_infos.lcd_pos.left, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.top, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.right, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.bottom, bytes, 4);
  bytes += 4;

  for (i = 0; i < 80; i++)
    {
      memcpy(&skin_infos.keys_pos[i].left, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].top, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].right, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].bottom, bytes, 4);
      bytes += 4;
    }

#if (G_BYTE_ORDER == G_BIG_ENDIAN) || defined(__MACOSX__)
  /*
   * VTI skins are usually designed on a little endian architecture
   */

  byteswap_vti_skin(&calc);
#endif

  vti_calc_type_to_string(calc);

  skin_infos.type = SKIN_TYPE_OLD_VTI;

  return [self loadJPEG:skin atOffset:1384];
}

- (int)loadSkinTiEmu:(NSData *)skin
{
  int i;
  unsigned char *bytes = NULL;
  uint32_t endian;
  uint32_t jpeg_offset;
  uint32_t length;

  bytes = (unsigned char *)[skin bytes];

  bytes += 16;
  
  memcpy(&endian, bytes, 4);
  bytes += 4;
  
  memcpy(&jpeg_offset, bytes, 4);
  bytes += 4;
  
  if (endian != ENDIANNESS_FLAG)
    jpeg_offset = bswap_32(jpeg_offset);


  memcpy(skin_infos.calc, bytes, 8);
  bytes += 8;
  
  memcpy(&skin_infos.colortype, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_white, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_black, bytes, 4);
  bytes += 4;

  /*
   * Skin name
   */
  memcpy(&length, bytes, 4);
  bytes += 4;
  
  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > 0)
  {
    skin_infos.name = (char *)malloc(length + 1);

    if (skin_infos.name == NULL)
      return -1;

    memset(skin_infos.name, 0, length + 1);

    memcpy(skin_infos.name, bytes, length);
  }
  bytes += length;
  

  /*
   * Skin author
   */

  memcpy(&length, bytes, 4);
  bytes += 4;
  
  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > 0)
  {
    skin_infos.author = (char *)malloc(length + 1);

    if (skin_infos.author == NULL)
      return -1;

    memset(skin_infos.author, 0, length + 1);

    memcpy(skin_infos.author, bytes, length);
  }
  bytes += length;


  memcpy(&skin_infos.lcd_pos.left, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.top, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.right, bytes, 4);
  bytes += 4;
  memcpy(&skin_infos.lcd_pos.bottom, bytes, 4);
  bytes += 4;
  
  /* number of RECT struct to read */
  memcpy(&length, bytes, 4);
  bytes += 4;
  
  if (endian != ENDIANNESS_FLAG)
    length = bswap_32(length);

  if (length > SKIN_KEYS)
    return -1;

  for (i = 0; i < length; i++)
    {
      memcpy(&skin_infos.keys_pos[i].left, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].top, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].right, bytes, 4);
      bytes += 4;
      memcpy(&skin_infos.keys_pos[i].bottom, bytes, 4);
      bytes += 4;
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

  return [self loadJPEG:skin atOffset:jpeg_offset];  
}

- (int)loadJPEG:(NSData *)skin atOffset:(uint32_t)jpeg_offset
{
  NSRange jpegRange;
  NSSize jpegSize;
  NSImage *skinImage;
  NSData *jpegData;

  jpegRange = NSMakeRange(jpeg_offset, [skin length] - jpeg_offset);

  jpegData = [skin subdataWithRange:jpegRange];

  skinImage = [[NSImage alloc] initWithData:jpegData];

  if (skinImage == nil)
    return -1;

  jpegSize = [skinImage size];

  skin_infos.width = jpegSize.width;
  skin_infos.height = jpegSize.height;

  [skinView setFrameSize:jpegSize];
  [skinView setNeedsDisplay:YES];
  [skinView setImage:skinImage];
  
  return 0;
}


// skin writing methods

- (int)writeSkin
{
  NSMutableData *skin = nil;
  NSData *jpeg = nil;

  /*
   * write the skin to skin_infos.skin_path
   */

  switch(skin_infos.type)
  {
    case SKIN_TYPE_NEW:
      skin = [self writeHeader];

      if (skin == nil)
        return -1;

      // readJPEG will do the Right Thing
      jpeg = [self readJPEG:[NSString stringWithCString:skin_infos.jpeg_path]];

      if (jpeg == nil)
        return -1;

      [skin appendData:jpeg];

      if ([skin writeToFile:[NSString stringWithCString:skin_infos.skin_path] atomically:YES] != YES)
        return -1;
      
      skin_infos.type = SKIN_TYPE_TIEMU;
      break;
    case SKIN_TYPE_TIEMU:
      skin = [self writeHeader];

      if (skin == nil)
        return -1;

      // readJPEG will do the Right Thing
      jpeg = [self readJPEG:[NSString stringWithCString:skin_infos.skin_path]];

      if (jpeg == nil)
        return -1;

      [skin appendData:jpeg];

      if ([skin writeToFile:[NSString stringWithCString:skin_infos.skin_path] atomically:YES] != YES)
        return -1;
        
        break;
    case SKIN_TYPE_VTI:
    case SKIN_TYPE_OLD_VTI:
      skin = [self writeHeader];

      if (skin == nil)
        return -1;

      // readJPEG will do the Right Thing
      jpeg = [self readJPEG:[NSString stringWithCString:skin_infos.skin_path]];

      if (jpeg == nil)
        return -1;

      [skin appendData:jpeg];

      if ([skin writeToFile:[NSString stringWithCString:skin_infos.skin_path] atomically:YES] != YES)
        return -1;

      skin_infos.type = SKIN_TYPE_TIEMU;

      break;
    default: /* should not get there */
      return -1;
  }

  return 0;
}

- (int)writeSkinAs:(NSString *)dest
{
  NSMutableData *skin = nil;
  NSData *jpeg = nil;

  if ((skin_infos.skin_path != NULL) && ([[NSString stringWithCString:skin_infos.skin_path] isEqualToString:dest]))
    return [self writeSkin];

  switch(skin_infos.type)
  {
    case SKIN_TYPE_NEW:
      skin_infos.skin_path = strdup([dest fileSystemRepresentation]);
      return [self writeSkin];
    case SKIN_TYPE_TIEMU:
    case SKIN_TYPE_VTI:
    case SKIN_TYPE_OLD_VTI:
      skin = [self writeHeader];

      if (skin == nil)
        return -1;

      // readJPEG will do the Right Thing
      jpeg = [self readJPEG:[NSString stringWithCString:skin_infos.skin_path]];

      if (jpeg == nil)
        return -1;

      [skin appendData:jpeg];

      if ([skin writeToFile:dest atomically:YES] != YES)
        return -1;

      if (skin_infos.skin_path != NULL)
        free(skin_infos.skin_path);
        
      skin_infos.skin_path = strdup([dest fileSystemRepresentation]);
      skin_infos.type = SKIN_TYPE_TIEMU;

      break;
    default: /* should not get there */
      return -1;
  }

  return 0;
}

// returns a NSMutableData to be completed by writeSkin/writeSkinAs
- (NSMutableData *)writeHeader
{
  NSMutableData *skin = nil;
  
  uint32_t endian = ENDIANNESS_FLAG;
  uint32_t jpeg_offset = 0;
  uint32_t length = 0;
  int i;
  unsigned char idString[16] = TIEMU_SKIN_ID;
  unsigned char *bytes = NULL;

  skin = [NSMutableData dataWithLength:16];

  if (skin == nil)
    return nil;

  bytes = (unsigned char *)[skin mutableBytes];

  memcpy(bytes, idString, 16);

  bytes = NULL;

  [skin appendBytes:&endian length:4];

  // write the jpeg_offset, we'll come back later...

  [skin appendBytes:&jpeg_offset length:4];

  [skin appendBytes:skin_infos.calc length:8];

  [skin appendBytes:&skin_infos.colortype length:4];
  [skin appendBytes:&skin_infos.lcd_white length:4];
  [skin appendBytes:&skin_infos.lcd_black length:4];
  
  if (skin_infos.name != NULL)
    length = strlen(skin_infos.name);
  else
    length = 0;

  [skin appendBytes:&length length:4];

  if (length > 0)
    [skin appendBytes:skin_infos.name length:length];


  if (skin_infos.author != NULL)
    length = strlen(skin_infos.author);
  else
    length = 0;

  [skin appendBytes:&length length:4];

  if (length > 0)
    [skin appendBytes:skin_infos.author length:length];


  [skin appendBytes:&skin_infos.lcd_pos.left length:4];
  [skin appendBytes:&skin_infos.lcd_pos.top length:4];
  [skin appendBytes:&skin_infos.lcd_pos.right length:4];
  [skin appendBytes:&skin_infos.lcd_pos.bottom length:4];

  // write the number of RECT structs
  length = SKIN_KEYS;
  [skin appendBytes:&length length:4];

  for (i = 0; i < SKIN_KEYS; i++)
    {
      [skin appendBytes:&skin_infos.keys_pos[i].left length:4];
      [skin appendBytes:&skin_infos.keys_pos[i].top length:4];
      [skin appendBytes:&skin_infos.keys_pos[i].right length:4];
      [skin appendBytes:&skin_infos.keys_pos[i].bottom length:4];
    }

  // get the current position
  jpeg_offset = [skin length];

  // get the bytes
  bytes = (unsigned char *)[skin mutableBytes];

  // go back to the jpeg_offset location
  bytes += 20;

  // write down the jpeg_offset
  memcpy(bytes, &jpeg_offset, 4);

  bytes = NULL;

  return skin;
}

// load in a NSData, return a NSData subDataWithRange:
- (NSData *)readJPEG:(NSString *)path
{
  NSData *jpeg = nil;
  NSData *ret = nil;
  NSRange jpegRange;
  unsigned char *bytes = NULL;

  uint32_t endian = 0;
  uint32_t jpeg_offset = 0;
  
  jpeg = [NSData dataWithContentsOfFile:path];

  if (jpeg == nil)
    return nil;

  // differentiate vti/oldvti/tiemu/raw jpeg

  bytes = (unsigned char *)[jpeg bytes];

  if (strncmp(bytes, "VTIv2.1", 7) == 0)
    jpegRange = NSMakeRange(1384, [jpeg length] - 1384);
  else if (strncmp(bytes, "VTIv2.5", 7) == 0)
    jpegRange = NSMakeRange(1448, [jpeg length] - 1448);
  else if (strncmp(bytes, "TiEmu v2", 8) == 0)
  {
    bytes += 16;
    memcpy(&endian, bytes, 4);
    bytes += 4;
    memcpy(&jpeg_offset, bytes, 4);

    if (endian != ENDIANNESS_FLAG)
      jpeg_offset = bswap_32(jpeg_offset);

    jpegRange = NSMakeRange(jpeg_offset, [jpeg length] - jpeg_offset);
  }
  else
    return jpeg;

  ret = [jpeg subdataWithRange:jpegRange];
      
  return ret;
}


// VTi export functions ...

- (int)exportSkinVTi:(NSString *)path version:(int)vtiVersion
{
  NSMutableData *skin = nil;
  NSData *jpeg;

  unsigned char buf[64];
  unsigned char *bytes = NULL;
  uint32_t tmpint;
  int i;  

  if (skin_infos.type == SKIN_TYPE_NEW)
  {
    jpeg = [self readJPEG:[NSString stringWithCString:skin_infos.jpeg_path]];
  }
  else
  {
    jpeg = [self readJPEG:[NSString stringWithCString:skin_infos.skin_path]];
  }

  if (jpeg == nil)
    return -1;

  skin = [NSMutableData dataWithLength:8];

  if (skin == nil)
    return -1;

  bytes = (unsigned char *)[skin mutableBytes];
  
  if (vtiVersion == EXPORT_VTI_2_1)
  {
    memcpy(bytes, "VTIv2.1 ", 8);
    bytes = NULL;
    
    memset(buf, 0, 64);

    if (skin_infos.name != NULL)
      strncpy(buf, skin_infos.name, 64);

    [skin appendBytes:buf length:64];
  }
  else if (vtiVersion == EXPORT_VTI_2_5)
  {
    memcpy(bytes, "VTIv2.5 ", 8);
    bytes = NULL;
    
    memset(buf, 0, 64);

    if (skin_infos.name != NULL)
      strncpy(buf, skin_infos.name, 64);

    [skin appendBytes:buf length:64];

    memset(buf, 0, 64);

    if (skin_infos.author != NULL)
      strncpy(buf, skin_infos.author, 64);

    [skin appendBytes:buf length:64];
  }
  else
    return -1;

  
  // VTI CORE
  if (strcmp(skin_infos.calc, CALC_TI73) == 0)
  {
    tmpint = 73;
  }
  else if (strcmp(skin_infos.calc, CALC_TI82) == 0)
  {
    tmpint = 82;
  }
  else if (strcmp(skin_infos.calc, CALC_TI83) == 0)
  {
    tmpint = 83;
  }
  else if (strcmp(skin_infos.calc, CALC_TI83P) == 0)
  {
    tmpint = 84;
  }
  else if (strcmp(skin_infos.calc, CALC_TI85) == 0)
  {
    tmpint = 85;
  }
  else if (strcmp(skin_infos.calc, CALC_TI86) == 0)
  {
    tmpint = 86;
  }
  else if (strcmp(skin_infos.calc, CALC_TI89) == 0)
  {
    tmpint = 89;
  }
  else if (strcmp(skin_infos.calc, CALC_TI92) == 0)
  {
    tmpint = 92;
  }
  else if (strcmp(skin_infos.calc, CALC_TI92P) == 0)
  {
    tmpint = 94;
  }
  else if (strcmp(skin_infos.calc, CALC_V200) == 0)
  {
    tmpint = 94; // same as 92P (for VTi)
  }

#if (G_BYTE_ORDER == G_BIG_ENDIAN) || defined(__MACOSX__)
  tmpint = bswap_32(tmpint);
  [skin appendBytes:&tmpint length:4]; // calc type

  tmpint = bswap_32(skin_infos.colortype);
  [skin appendBytes:&tmpint length:4];

  tmpint = bswap_32(skin_infos.lcd_white);
  [skin appendBytes:&tmpint length:4];

  tmpint = bswap_32(skin_infos.lcd_black);
  [skin appendBytes:&tmpint length:4];

  [skin appendBytes:&bswap_32(skin_infos.lcd_pos.top) length:4];
  [skin appendBytes:&bswap_32(skin_infos.lcd_pos.bottom) length:4];
  [skin appendBytes:&bswap_32(skin_infos.lcd_pos.left) length:4];
  [skin appendBytes:&bswap_32(skin_infos.lcd_pos.right) length:4];

  for (i = 0; i < 80; i++)
  {
    [skin appendBytes:&bswap_32(skin_infos.keys_pos[i].top) length:4];
    [skin appendBytes:&bswap_32(skin_infos.keys_pos[i].bottom) length:4];
    [skin appendBytes:&bswap_32(skin_infos.keys_pos[i].left) length:4];
    [skin appendBytes:&bswap_32(skin_infos.keys_pos[i].right) length:4];
  }
#else
  [skin appendBytes:&tmpint length:4];
  [skin appendBytes:&skin_infos.colortype length:4];
  [skin appendBytes:&skin_infos.lcd_white length:4];
  [skin appendBytes:&skin_infos.lcd_black length:4];

  [skin appendBytes:&skin_infos.lcd_pos.top length:4];
  [skin appendBytes:&skin_infos.lcd_pos.bottom length:4];
  [skin appendBytes:&skin_infos.lcd_pos.left length:4];
  [skin appendBytes:&skin_infos.lcd_pos.right length:4];

  for (i = 0; i < 80; i++)
  {
    [skin appendBytes:&skin_infos.keys_pos[i].top length:4];
    [skin appendBytes:&skin_infos.keys_pos[i].bottom length:4];
    [skin appendBytes:&skin_infos.keys_pos[i].left length:4];
    [skin appendBytes:&skin_infos.keys_pos[i].right length:4];
  }
#endif
  // END VTI CORE

  [skin appendData:jpeg];

  if ([skin writeToFile:path atomically:YES] != YES)
    return -1;

  return 0;  
}


// drawer callback

- (IBAction)listKeysFinish:(id)sender
{
  // erase the last rubberband
  [skinView setRubMode:ACTION_NONE];

  [listKeysDrawer close:self];
}


// NSTableView (listKeys) dataSource & delegate methods

- (int)numberOfRowsInTableView:(NSTableView *)tableView
{
    int i = 0;
    
    if ((skin_infos.skin_path == NULL) && (skin_infos.jpeg_path == NULL))
        return 0;
    
    while (keynames[skin_infos.keymap][i] != NULL)
        {
            i++;
        }

    return i;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row
{
    return [NSString stringWithCString:keynames[skin_infos.keymap][row]];
}

- (IBAction)listKeysSelect:(id)sender
{
    // draw the rubberbox for the key
    [skinView setKeyIndex:[listKeys clickedRow]];
}

@end
