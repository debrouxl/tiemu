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

@interface PropController : NSObject
{
    IBOutlet id calc73;
    IBOutlet id calc82;
    IBOutlet id calc83;
    IBOutlet id calc83P;
    IBOutlet id calc85;
    IBOutlet id calc86;
    IBOutlet id calc89;
    IBOutlet id calc92;
    IBOutlet id calc92P;
    IBOutlet id calcV200;
    IBOutlet id calcMatrix;
    IBOutlet id LCDColorCustom;
    IBOutlet id LCDColorCustomBlack;
    IBOutlet id LCDColorCustomWhite;
    IBOutlet id LCDColorHigh;
    IBOutlet id LCDColorLow;
    IBOutlet id LCDColorMatrix;
    IBOutlet id properties;
    IBOutlet id skinAuthor;
    IBOutlet id skinTitle;
    
    IBOutlet id mySkinController;
    IBOutlet id mySkineditController;
}

// set properties
- (void)setProperties;

// callbacks
- (IBAction)propCancel:(id)sender;
- (IBAction)propOK:(id)sender;

@end
