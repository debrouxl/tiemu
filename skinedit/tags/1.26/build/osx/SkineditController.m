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

#import "SkineditController.h"
#import "PropController.h"
#import "SkinController.h"
#import "SkinView.h"

#include "../src/struct.h"
#include "../src/defs.h"

#include "utils.h"

extern struct skinInfos skin_infos;

static void addToolbarItem(NSMutableDictionary *theDict, NSString *identifier, NSString *label, NSString *paletteLabel, NSString *toolTip, id target, SEL settingSelector, id itemContent, SEL action)
{
    NSToolbarItem *item;
    
    item = [[[NSToolbarItem alloc] initWithItemIdentifier:identifier] autorelease];
    
    [item setLabel:label];
    [item setPaletteLabel:paletteLabel];
    [item setToolTip:toolTip];
    [item setTarget:target];

    [item performSelector:settingSelector withObject:itemContent];
    
    [item setAction:action];
  
    [theDict setObject:item forKey:identifier];
}


@implementation SkineditController

- (id)init
{
  self = [super init];

  if (self == nil)
    return nil;
  
  action = ACTION_NONE;

  return self;
}

// toolbar

- (void)awakeFromNib
{
    NSToolbar *toolbar;
    
    toolbar = [[[NSToolbar alloc] initWithIdentifier:@"myToolbar"] autorelease];
    
    toolbarItems = [[NSMutableDictionary dictionary] retain];

    addToolbarItem(toolbarItems, @"Properties", @"Properties", @"Properties", @"Pop up skin properties dialog", self, @selector(setImage:), [NSImage imageNamed:@"prop.tiff"], @selector(menuProperties:));

    addToolbarItem(toolbarItems, @"LCDPos", @"LCD Position", @"LCD Position", @"View and set the LCD position", self, @selector(setImage:), [NSImage imageNamed:@"screen.tiff"], @selector(menuLCDPos:));

    addToolbarItem(toolbarItems, @"KeyPos", @"Key Positions", @"Key Positions", @"View and set key positions", self, @selector(setImage:), [NSImage imageNamed:@"keyboard.tiff"], @selector(menuKeyPos:));

    [toolbar setDelegate:self];

    [toolbar setAllowsUserCustomization:YES];

    [toolbar setAutosavesConfiguration: YES]; 
 
    [toolbar setDisplayMode:NSToolbarDisplayModeIconAndLabel];
    
    [mainWindow setToolbar:toolbar];
}

- (void) dealloc
{
    [toolbarItems release];

    [super dealloc];
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)itemIdentifier willBeInsertedIntoToolbar:(BOOL)flag
{
    NSToolbarItem *newItem;
    NSToolbarItem *item;
    
    newItem = [[[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier] autorelease];
    item = [toolbarItems objectForKey:itemIdentifier];
    
    [newItem setLabel:[item label]];
    [newItem setPaletteLabel:[item paletteLabel]];

    [newItem setImage:[item image]];

    [newItem setToolTip:[item toolTip]];
    [newItem setTarget:[item target]];
    [newItem setAction:[item action]];
    [newItem setMenuFormRepresentation:[item menuFormRepresentation]];

    return newItem;
}

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar
{
    return [NSArray arrayWithObjects:@"Properties", @"LCDPos", @"KeyPos", @"text", nil];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar
{
    return [NSArray arrayWithObjects:@"Properties", @"LCDPos", @"KeyPos", @"text", nil];
}


// Application menu

- (IBAction)menuQuit:(id)sender
{
  if (skin_infos.changed)
  {
    action = ACTION_EXIT;
    [self showSkinChanged];

    return;
  }

  clear_skin_infos();

  [NSApp terminate:self];
}


// File

- (IBAction)menuNew:(id)sender
{
  NSOpenPanel *op = nil;
  
  if (skin_infos.changed)
  {
    action = ACTION_NEW;
    [self showSkinChanged];

    return;
  }

  if ([listKeysDrawer state] != NSDrawerClosedState)
    [listKeysDrawer close];

  [skinView setImage:nil];

  // clear the handlers for the rubberband, if any !
  
  clear_skin_infos();

  action = ACTION_NONE;

  [mainWindow makeKeyAndOrderFront:self];
  
  op = [NSOpenPanel openPanel];

  [op setTitle:@"Choose the JPEG image to use"];
  [op setAllowsMultipleSelection:NO];

  [op beginSheetForDirectory:NSHomeDirectory()
      file:nil
      types:[NSArray arrayWithObjects:@"jpg", @"jpeg", @"JPG", @"JPEG", nil]
      modalForWindow:mainWindow
      modalDelegate:self
      didEndSelector:@selector(newSkinDidEnd:returnCode:contextInfo:)
      contextInfo:nil];
}

- (IBAction)menuOpen:(id)sender
{
  NSOpenPanel *op = nil;

  if (skin_infos.changed)
  {
    action = ACTION_OPEN;
    [self showSkinChanged];

    return;
  }

  if ([listKeysDrawer state] != NSDrawerClosedState)
    [listKeysDrawer close];

  [skinView setImage:nil];

  // clear the handlers for the rubberband, if any !

  clear_skin_infos();

  action = ACTION_NONE;

  [mainWindow makeKeyAndOrderFront:self];
  
  op = [NSOpenPanel openPanel];

  [op setTitle:@"Choose the skin to open"];
  [op setAllowsMultipleSelection:NO];

  [op beginSheetForDirectory:NSHomeDirectory()
      file:nil
      types:[NSArray arrayWithObjects:@"skn", @"SKN", nil]
      modalForWindow:mainWindow
      modalDelegate:self
      didEndSelector:@selector(openSkinDidEnd:returnCode:contextInfo:)
      contextInfo:nil];  
}

- (IBAction)menuSave:(id)sender
{
  if ([mainWindow isVisible] != YES)
    return;

  if (skin_infos.skin_path != NULL)
  {
    if ([mySkinController writeSkin] == 0)
      skin_infos.changed = 0;
  }
  else
  {
    [self menuSaveAs:self];
  }  
}

- (IBAction)menuSaveAs:(id)sender
{
  NSSavePanel *sp = nil;
  
  if ([mainWindow isVisible] != YES)
    return;

  sp = [NSSavePanel savePanel];

  [sp beginSheetForDirectory:NSHomeDirectory()
      file:@"tiemu-skin.skn"
      modalForWindow:mainWindow
      modalDelegate:self
      didEndSelector:@selector(saveSkinAsDidEnd:returnCode:contextInfo:)
      contextInfo:sp];
}

- (IBAction)menuExportVTi21:(id)sender
{
  NSSavePanel *sp = nil;
  
  if ([mainWindow isVisible] != YES)
    return;

  action = EXPORT_VTI_2_1;
  
  sp = [NSSavePanel savePanel];

  [sp beginSheetForDirectory:NSHomeDirectory()
      file:@"VTi21-skin.skn"
      modalForWindow:mainWindow
      modalDelegate:self
      didEndSelector:@selector(exportSkinVTiDidEnd:returnCode:contextInfo:)
      contextInfo:sp];  
}

- (IBAction)menuExportVTi25:(id)sender
{
  NSSavePanel *sp = nil;
  
  if ([mainWindow isVisible] != YES)
    return;

  action = EXPORT_VTI_2_5;
  
  sp = [NSSavePanel savePanel];

  [sp beginSheetForDirectory:NSHomeDirectory()
      file:@"VTi25-skin.skn"
      modalForWindow:mainWindow
      modalDelegate:self
      didEndSelector:@selector(exportSkinVTiDidEnd:returnCode:contextInfo:)
      contextInfo:sp]; 
}


// Edit

- (IBAction)menuProperties:(id)sender
{
    if ([mainWindow isVisible] != YES)
        return;

    [properties setExcludedFromWindowsMenu:YES];

    [myPropController setProperties];
    
    [NSApp beginSheet:properties
           modalForWindow:mainWindow
           modalDelegate:nil
           didEndSelector:nil
           contextInfo:nil];

    [NSApp endSheet:properties];
}

- (IBAction)menuLCDPos:(id)sender
{
  NSBeginInformationalAlertSheet(@"How to define the LCD area",
                                 @"Ok !", nil, nil,
                                 mainWindow,
                                 nil, nil, nil, nil,
                                 @"Left click + drag to define\nCtrl + click to save");
  
  [skinView setRubMode:DEFINE_LCD];
}

- (IBAction)menuKeyPos:(id)sender
{
    [listKeys reloadData];

    [listKeysDrawer open:self];
    
    [listKeys selectRow:0 byExtendingSelection:NO];

    [skinView setRubMode:DEFINE_KEYS];
    // Draw rubberbox for the first key
}


// skin changed callbacks

- (void)showSkinChanged
{
  if ([skinChanged isVisible] != NO)
    return;

  [skinChanged setExcludedFromWindowsMenu:YES];
  
  [NSApp beginSheet:skinChanged
         modalForWindow:mainWindow
         modalDelegate:nil
         didEndSelector:nil
         contextInfo:nil];

  [NSApp endSheet:skinChanged];
}

- (IBAction)skinChangedCancel:(id)sender
{
  [skinChanged orderOut:self];
}

- (IBAction)skinChangedIgnore:(id)sender
{
  skin_infos.changed = 0;

  [skinChanged orderOut:self];

  switch(action)
  {
    case ACTION_NEW:
      [self menuNew:self];
      break;
    case ACTION_OPEN:
      [self menuOpen:self];
      break;
    case ACTION_EXIT:
      [self menuQuit:self];
      break;
    default:
      return;
  }  
}

- (IBAction)skinChangedSave:(id)sender
{  
  if (skin_infos.skin_path != NULL)
  {
    if ([mySkinController writeSkin] == 0)
      skin_infos.changed = 0;

  [skinChanged orderOut:self];
    
    switch(action)
    {
      case ACTION_NEW:
        [self menuNew:self];
        break;
      case ACTION_OPEN:
        [self menuOpen:self];
        break;
      case ACTION_EXIT:
        [self menuQuit:self];
        break;
      default:
        return;
    }
  }
  else
  {
    [self menuSaveAs:self];
  }  
}


// openPanels callbacks
- (void)newSkinDidEnd:(NSOpenPanel *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo
{
  NSArray *files = nil;
  NSData *jpeg = nil;
  
  [sheet orderOut:self];

  if (returnCode == NSOKButton)
  {
    files = [sheet filenames];

    if ((files == nil) || ([files count] == 0))
      return;

    jpeg = [NSData dataWithContentsOfFile:[files objectAtIndex:0]];

    if (jpeg == nil)
      return;

    if ([mySkinController loadJPEG:jpeg atOffset:0] == 0)
    {
      skin_infos.changed = 1;
      
      [mainWindow makeKeyAndOrderFront:self];

      [self menuProperties:self];
    }
  }
}

- (void)openSkinDidEnd:(NSOpenPanel *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo
{
  NSArray *files = nil;

  [sheet orderOut:self];

  if (returnCode == NSOKButton)
  {
    files = [sheet filenames];

    if ((files == nil) || ([files count] == 0))
      return;

    if ([mySkinController loadSkin:[files objectAtIndex:0]] == 0)
    {
      [mainWindow makeKeyAndOrderFront:self];
      
      [self menuProperties:self];
    }
  }  
}


// savePanels callbacks
- (void)saveSkinAsDidEnd:(NSWindow *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo
{
  NSSavePanel *sp;

  if (returnCode == NSOKButton)
  {
    sp = contextInfo;

    if ([mySkinController writeSkinAs:[sp filename]] == 0)
    {
      if (skin_infos.skin_path != NULL)
        free(skin_infos.skin_path);

      skin_infos.skin_path = strdup([[sp filename] fileSystemRepresentation]);

      skin_infos.changed = 0;

      switch(action)
      {
        case ACTION_NEW:
          [self menuNew:self];
          break;
        case ACTION_OPEN:
          [self menuOpen:self];
          break;
        case ACTION_EXIT:
          [self menuQuit:self];
          break;
        default:
          return;
      }
    }
  }
}

- (void)exportSkinVTiDidEnd:(NSWindow *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo
{
  NSSavePanel *sp;

  if (returnCode == NSOKButton)
  {
    sp = contextInfo;

    [mySkinController exportSkinVTi:[sp filename] version:action];

    action = ACTION_NONE;
  }
}

@end
