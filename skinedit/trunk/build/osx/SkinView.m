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

#import "SkinView.h"

#include "../src/defs.h"
#include "../src/struct.h"

extern struct skinInfos skin_infos;

@implementation SkinView

// set _rubMode to ACTION_NONE
- (id)init
{
  self = [super init];

  if (self == nil)
    return nil;

  _rubMode = ACTION_NONE;

  keyIndex = 0;
  
  rubOrig = NSZeroPoint;
  rubberbox = NSZeroRect;

  return self;
}

- (void)setKeyIndex:(int)key
{
  if (key < [listKeys numberOfRows])
  {
    keyIndex = key;
  }
  else
    keyIndex = 0;

  rubberbox = NSMakeRect((float)skin_infos.keys_pos[keyIndex].left,
                         (float)(skin_infos.height - skin_infos.keys_pos[keyIndex].bottom),
                         (float)(skin_infos.keys_pos[keyIndex].right - skin_infos.keys_pos[keyIndex].left),
                         (float)(skin_infos.keys_pos[keyIndex].bottom - skin_infos.keys_pos[keyIndex].top));

  [listKeys selectRow:keyIndex byExtendingSelection:NO];
  [listKeys scrollRowToVisible:keyIndex];

  [self scrollRectToVisible:rubberbox];
  
  [self setNeedsDisplay:YES];
}

- (void)setRubMode:(int)mode
{
  _rubMode = mode;

  rubOrig = NSZeroPoint;
  rubberbox = NSZeroRect;
  
  switch(_rubMode)
  {
    case DEFINE_KEYS:
      // set rubberbox for the first key
      rubberbox = NSMakeRect((float)skin_infos.keys_pos[0].left,
                             (float)(skin_infos.height - skin_infos.keys_pos[0].bottom),
                             (float)(skin_infos.keys_pos[0].right - skin_infos.keys_pos[0].left),
                             (float)(skin_infos.keys_pos[0].bottom - skin_infos.keys_pos[0].top));
      [self scrollRectToVisible:rubberbox];
      break;
    case DEFINE_LCD:
      // set rubberbox for the LCD
      rubberbox = NSMakeRect((float)skin_infos.lcd_pos.left,
                             (float)(skin_infos.height - skin_infos.lcd_pos.bottom),
                             (float)(skin_infos.lcd_pos.right - skin_infos.lcd_pos.left),
                             (float)(skin_infos.lcd_pos.bottom - skin_infos.lcd_pos.top));
      [self scrollRectToVisible:rubberbox];
      break;
    default:
      break;
  }

  [self setNeedsDisplay:YES];
}


// get mouse events
- (void)mouseDown:(NSEvent *)event
{
  NSPoint mouseClick;
  
  if ((_rubMode != DEFINE_LCD) && (_rubMode != DEFINE_KEYS))
    return;
    
  if ([event type] == NSLeftMouseDown)
  {    
    if ([event modifierFlags] & NSControlKeyMask)
    {
      skin_infos.changed = 1;

      switch(_rubMode)
      {
        case DEFINE_LCD:
          skin_infos.lcd_pos.top = (skin_infos.height - (rubberbox.origin.y + rubberbox.size.height));
          skin_infos.lcd_pos.bottom = (skin_infos.height - rubberbox.origin.y);
          skin_infos.lcd_pos.left = rubberbox.origin.x;
          skin_infos.lcd_pos.right = (rubberbox.origin.x + rubberbox.size.width);
          [self setRubMode:ACTION_NONE];
          break;
        case DEFINE_KEYS:
          skin_infos.keys_pos[keyIndex].top = (skin_infos.height - (rubberbox.origin.y + rubberbox.size.height));
          skin_infos.keys_pos[keyIndex].bottom = (skin_infos.height - rubberbox.origin.y);
          skin_infos.keys_pos[keyIndex].left = rubberbox.origin.x;
          skin_infos.keys_pos[keyIndex].right = (rubberbox.origin.x + rubberbox.size.width);
          [self setKeyIndex:keyIndex+1];
          break;
        default:
          break;
      }
      
      return;
    }

    rubOrig = NSZeroPoint;
    rubberbox = NSZeroRect;
    [self setNeedsDisplay:YES];
    
    mouseClick = [self convertPoint:[event locationInWindow] fromView:nil];

    if ([self mouse:mouseClick inRect:[self bounds]])
    {
      rubOrig = mouseClick;

      [self trackMouse];
    }
  }
}

// track the mouse to define the LCD/keys
- (void)trackMouse
{
  NSEvent *event;
  NSPoint cur;

  int x, y, w, h;
  
  while (1)
  {
    event = [[self window] nextEventMatchingMask:(NSLeftMouseDraggedMask | NSLeftMouseUpMask)];

    if ([event type] == NSLeftMouseDragged)
    {
      cur = [self convertPoint:[event locationInWindow] fromView:nil];

      if ([self mouse:cur inRect:[self bounds]])
      {
        if (cur.x >= rubOrig.x)
        {
          x = rubOrig.x;
          w = (cur.x - rubOrig.x);

          if (cur.y >= rubOrig.y)
          {
            y = rubOrig.y;
            h = (cur.y - rubOrig.y);
          }
          else
          {
            /* remap Y orig */
            y = cur.y;
            h = (rubOrig.y - cur.y);
          }
        }
        else
        {
          /* remap X orig */
          x = cur.x;
          w = (rubOrig.x - cur.x);

          if (cur.y >= rubOrig.y)
          {
            y = rubOrig.y;
            h = (cur.y - rubOrig.y);
          }
          else
          {
            /* remap Y orig */
            y = cur.y;
            h = (rubOrig.y - cur.y);
          }
        }

        rubberbox = NSMakeRect(x, y, w, h);
        [self autoscroll:event];
        [self setNeedsDisplay:YES];
      }
    }
    else
      break;
  }
}

// override drawRect to draw the rubberbox
- (void)drawRect:(NSRect)rect
{  
  [super drawRect:rect];

  if (NSEqualRects(rubberbox, NSZeroRect) != YES)
  {
    // knobColor
    [[NSColor orangeColor] set];

    [NSBezierPath strokeRect:rubberbox];
  }
}

@end
