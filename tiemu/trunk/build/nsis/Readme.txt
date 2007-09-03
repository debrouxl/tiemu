TiEmu 3 (version 3.01a)
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005-2007, Romain Liévin, Kevin Kofler
 *  Copyright (c) 2007, Peter Fernandes
Licensed under the GNU General Public License version 2 or later.

1. Installed components

The following components have been successfully installed:
* TiEmu (with TIGCC debugging support)
* libticables
* libticonv
* libtifiles
* libticalcs
* libusb (DLL only, see section 2.2)
* GTK+ (if not already present)
* Tcl
* Tk
* [Incr Tcl]
* [Incr Tk]
* [Incr widgets]
* readline (linked statically into TiEmu)
* libgui (linked statically into TiEmu)
* GDB (linked statically into TiEmu)
* Insight (linked statically into TiEmu)
* SDL (linked statically into TiEmu)

Uninstalling TiEmu removes all these components except GTK+, which can be
uninstalled separately.

2. Real link cable support

If you want to link TiEmu to a real calculator through a link cable, you may
need to install additional drivers which are not installed with TiEmu:

2.1. DhaHelper/RWPorts

In order to use TI's BlackLink serial cables or home-made serial or parallel
cables on any NT-based version of Windows (Windows NT 4, Windows 2000, Windows
XP, Windows 2003 or any newer version of Windows), you'll need to install the
DhaHelper (32-bit) or RWPorts (64-bit) driver. These drivers are currently not
installed by this package of TiEmu. DhaHelper can be installed by installing
TiLP 2 for Windows, version 1.07a or newer.

2.2. Libusb (SilverLink)

In order to use TI's SilverLink USB cables (formerly known as TI-GraphLink
USB), you'll need to install the libusb driver for Windows at:
http://sourceforge.net/projects/libusb-win32
You also need the silverlk.inf file included with libticables2. The easiest
way to install this driver with the correct .inf files is to install TiLP 2
for Windows, version 1.07 or newer. This package currently only installs the
DLL, NOT the driver.

IMPORTANT: This driver is NOT compatible with the driver used by TI Connect,
installing it will replace TI's driver or the opposite. In order to use TI
Connect, you'll have to uninstall the libusb driver and reinstall TI's driver.
Likewise, to use the SilverLink in TiEmu again, you'll have to uninstall TI's
driver and reinstall the libusb one. As an alternative, we suggest using TiLP
(TiLP 2 >= 1.07a) which uses the libusb driver.

3. GTK+ Troubleshooting

If you're getting one of these:
- ** WARNING **: Couldn't load font "MS Sans Serif 8" falling back to "Sans 8"
- ** ERROR **: file shape.c line 75: assertion failed: (glyphs->num_glyphs > 0)
or any other warning or error message regarding fonts/glyphs, the fix is to
open your C:\Program Files\Shared Files\Gtk\2.0\etc\gtk-2.0\gtkrc and to add
this:
	style "user-font"
	{
	font_name="Arial"
	}
	widget_class "*" style "user-font"
This forces GTK+ to use a compatible font (Arial) and reportedly fixes both
the aforementioned problems, and probably others too.

Another source of errors like the aforementioned ones is attempting to use GTK+
2.8 or higher with a Windows version not supported by that version of GTK+.
GTK+ 2.8 or higher requires Windows 2000, XP or newer. If you're running Windows
95, 98, Me or NT 4, use GTK+ 2.6 instead.

If you are getting any errors with DLLs, such as "The procedure entry point
atk_relation_set_add_relation_by_type could not be located in the dynamic link
library libatk-1.0-0.dll", make sure you have one and only ONE version of the
GTK+ DLLs. If unsure, remove all GTK+ packages and install it again.

4. Usage

The installer automatically creates program entries in the Windows Startup menu.
In order to get detailed instructions on how to use TiEmu, right-click on the
emulated calculator screen and select "Misc" / "User's Manual".

5. Contact Information

You can reach us through our project web page:
http://lpg.ticalc.org/prj_tiemu/
Links to bug/feature/patch/support trackers, forums and mailing lists can be
found on the web page.

        -- Kevin Kofler for the TiEmu Team