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

#import "PropController.h"

#include "../src/struct.h"
#include "../src/defs.h"

extern struct skinInfos skin_infos;

@implementation PropController

- (void)setProperties
{
  float rgb[3];

  if (skin_infos.name != NULL)
    [skinTitle setStringValue:[NSString stringWithCString:skin_infos.name]];
  else
    [skinTitle setStringValue:@""];
  
  if (skin_infos.author != NULL)
    [skinAuthor setStringValue:[NSString stringWithCString:skin_infos.author]];
  else
    [skinAuthor setStringValue:@""];


  switch(skin_infos.colortype)
  {
    case LCD_COLORTYPE_HIGH:
      [LCDColorMatrix setState:NSOnState atRow:0 column:0];
      [LCDColorCustomWhite setColor:[NSColor whiteColor]];
      [LCDColorCustomBlack setColor:[NSColor blackColor]];
      break;
    case LCD_COLORTYPE_LOW:
      [LCDColorMatrix setState:NSOnState atRow:1 column:0];
      [LCDColorCustomWhite setColor:[NSColor whiteColor]];
      [LCDColorCustomBlack setColor:[NSColor blackColor]];
      break;
    case LCD_COLORTYPE_CUSTOM:
      [LCDColorMatrix setState:NSOnState atRow:2 column:0];
      
      rgb[0] = (float)((skin_infos.lcd_white >> 16) & 0xff) / 0xff;
      rgb[1] = (float)((skin_infos.lcd_white >> 8) & 0xff) / 0xff;
      rgb[2] = (float)(skin_infos.lcd_white & 0xff) / 0xff;
      [LCDColorCustomWhite setColor:[NSColor colorWithCalibratedRed:rgb[0] green:rgb[1] blue:rgb[2] alpha:1.0 ]];

      rgb[0] = (float)((skin_infos.lcd_black >> 16) & 0xff) / 0xff;
      rgb[1] = (float)((skin_infos.lcd_black >> 8) & 0xff) / 0xff;
      rgb[2] = (float)(skin_infos.lcd_black & 0xff) / 0xff;
      [LCDColorCustomBlack setColor:[NSColor colorWithCalibratedRed:rgb[0] green:rgb[1] blue:rgb[2] alpha:1.0 ]];      
      break;
    default:
      break;
  }

  
  if (strcmp(skin_infos.calc, CALC_TI73) == 0)
  {
    [calcMatrix setState:NSOnState atRow:0 column:0];
  }
  else if (strcmp(skin_infos.calc, CALC_TI82) == 0)
  {
    [calcMatrix setState:NSOnState atRow:1 column:0];
  }
  else if (strcmp(skin_infos.calc, CALC_TI83) == 0)
  {
    [calcMatrix setState:NSOnState atRow:2 column:0];
  }
  else if (strcmp(skin_infos.calc, CALC_TI83P) == 0)
  {
    [calcMatrix setState:NSOnState atRow:3 column:0];
  }
  else if (strcmp(skin_infos.calc, CALC_TI85) == 0)
  {
    [calcMatrix setState:NSOnState atRow:4 column:0];
  }
  else if (strcmp(skin_infos.calc, CALC_TI86) == 0)
  {
    [calcMatrix setState:NSOnState atRow:0 column:1];
  }
  else if (strcmp(skin_infos.calc, CALC_TI89) == 0)
  {
    [calcMatrix setState:NSOnState atRow:1 column:1];
  }
  else if (strcmp(skin_infos.calc, CALC_TI92) == 0)
  {
    [calcMatrix setState:NSOnState atRow:2 column:1];
  }
  else if (strcmp(skin_infos.calc, CALC_TI92P) == 0)
  {
    [calcMatrix setState:NSOnState atRow:3 column:1];
  }
  else if (strcmp(skin_infos.calc, CALC_V200) == 0)
  {
    [calcMatrix setState:NSOnState atRow:4 column:1];
  }
  
}

- (IBAction)propCancel:(id)sender
{
    [properties orderOut:self];
}

- (IBAction)propOK:(id)sender
{
    float rgb[3];
    uint8_t r, g, b;

    if (skin_infos.name != NULL)
        free(skin_infos.name);
    
    skin_infos.name = strdup([[skinTitle stringValue] cString]);
    
    if (skin_infos.author != NULL)
        free(skin_infos.author);
    
    skin_infos.author = strdup([[skinAuthor stringValue] cString]);


    memset(skin_infos.calc, 0, 8);

    if ([calc73 state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI73);
      skin_infos.keymap = 0;
    }
    else if ([calc82 state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI82);
      skin_infos.keymap = 1;
    }
    else if ([calc83 state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI83);
      skin_infos.keymap = 1;
    }
    else if ([calc83P state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI83P);
      skin_infos.keymap = 1;
    }
    else if ([calc85 state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI85);
      skin_infos.keymap = 2;
    }
    else if ([calc86 state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI86);
      skin_infos.keymap = 2;
    }
    else if ([calc89 state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI89);
      skin_infos.keymap = 3;
    }
    else if ([calc92 state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI92);
      skin_infos.keymap = 4;
    }
    else if ([calc92P state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_TI92P);
      skin_infos.keymap = 4;
    }
    else if ([calcV200 state] == NSOnState)
    {
      strcpy(skin_infos.calc, CALC_V200);
      skin_infos.keymap = 4;
    }

    
    if ([LCDColorHigh state] == NSOnState)
    {
      skin_infos.colortype = LCD_COLORTYPE_HIGH;
      skin_infos.lcd_white = LCD_HI_WHITE;
      skin_infos.lcd_black = LCD_HI_BLACK;
    }
    else if ([LCDColorHigh state] == NSOnState)
    {
      skin_infos.colortype = LCD_COLORTYPE_LOW;
      skin_infos.lcd_white = LCD_LOW_WHITE;
      skin_infos.lcd_black = LCD_LOW_BLACK;
    }
    else if ([LCDColorCustom state] == NSOnState)
    {
      skin_infos.colortype = LCD_COLORTYPE_CUSTOM;

      [[LCDColorCustomWhite color] getRed:&rgb[0] green:&rgb[1] blue:&rgb[2] alpha:nil];
      r = rgb[0] * 0xff;
      g = rgb[1] * 0xff;
      b = rgb[2] * 0xff;
      skin_infos.lcd_white = (r << 16) | (g << 8) | b;

      [[LCDColorCustomBlack color] getRed:&rgb[0] green:&rgb[1] blue:&rgb[2] alpha:nil];
      r = rgb[0] * 0xff;
      g = rgb[1] * 0xff;
      b = rgb[2] * 0xff;
      skin_infos.lcd_black = (r << 16) | (g << 8) | b;
    }

    skin_infos.changed = 1;
    
    [properties orderOut:self];
}

@end
