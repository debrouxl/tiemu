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

#import <Cocoa/Cocoa.h>

@interface SkineditController : NSObject
{
    IBOutlet id listKeys;
    IBOutlet id listKeysDrawer;
    IBOutlet id mainWindow;
    IBOutlet id properties;
    IBOutlet id skinChanged;

    IBOutlet id skinView;
    
    IBOutlet id myPropController;
    IBOutlet id mySkinController;
    
    NSMutableDictionary *toolbarItems;
    int action;
}

// toolbar
- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)itemIdentifier willBeInsertedIntoToolbar:(BOOL)flag;    
- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar;
- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar;

// Application menu
- (IBAction)menuQuit:(id)sender;

// File
- (IBAction)menuNew:(id)sender;
- (IBAction)menuOpen:(id)sender;
- (IBAction)menuSave:(id)sender;
- (IBAction)menuSaveAs:(id)sender;
- (IBAction)menuExportVTi21:(id)sender;
- (IBAction)menuExportVTi25:(id)sender;

// Edit
- (IBAction)menuProperties:(id)sender;
- (IBAction)menuLCDPos:(id)sender;
- (IBAction)menuKeyPos:(id)sender;

// skin changed callbacks
- (void)showSkinChanged;
- (IBAction)skinChangedCancel:(id)sender;
- (IBAction)skinChangedIgnore:(id)sender;
- (IBAction)skinChangedSave:(id)sender;

// openPanels callbacks
- (void)newSkinDidEnd:(NSOpenPanel *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo;
- (void)openSkinDidEnd:(NSOpenPanel *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo;

// savePanels callbacks
- (void)saveSkinAsDidEnd:(NSWindow *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo;
- (void)exportSkinVTiDidEnd:(NSWindow *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo;

@end
