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

#include <stdint.h>

@interface SkinController : NSObject
{
    IBOutlet id listKeysDrawer;
    IBOutlet id listKeys;

    IBOutlet id skinScrollView;
    IBOutlet id skinView;
    IBOutlet id mainWindow;

    IBOutlet id myPropController;
    IBOutlet id mySkineditController;
}

// skin loading methods
- (int)loadSkin:(NSString *)path;
- (int)loadSkinVTi:(NSData *)skin;
- (int)loadSkinOldVTi:(NSData *)skin;
- (int)loadSkinTiEmu:(NSData *)skin;
- (int)loadJPEG:(NSData *)skin atOffset:(uint32_t)jpeg_offset;

// skin writing methods
- (int)writeSkin;
- (int)writeSkinAs:(NSString *)dest;
- (NSMutableData *)writeHeader;
- (NSData *)readJPEG:(NSString *)path;

// VTi export
- (int)exportSkinVTi:(NSString *)path version:(int)vtiVersion;

// drawer callback
- (IBAction)listKeysFinish:(id)sender;

// NSTableView (listKeys) dataSource & action
- (int)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(int)row;
- (IBAction)listKeysSelect:(id)sender;

@end
