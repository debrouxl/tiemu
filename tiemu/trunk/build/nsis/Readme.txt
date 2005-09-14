TiEmu 3 (development version 2.80)
 *  Copyright (c) 2000-2001, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2003, Romain Lievin
 *  Copyright (c) 2003, Julien Blache
 *  Copyright (c) 2004, Romain Liévin
 *  Copyright (c) 2005, Romain Liévin, Kevin Kofler
Licensed under the GNU General Public License.

1. Installed components

The following components have been successfully installed:
* TiEmu (tigcc-debugging)
* libticables
* libtifiles
* libticalcs
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

Uninstalling TiEmu removes all these components except GTK+, which can be
uninstalled separately.

2. Real link cable support

If you want to link TiEmu to a real calculator through a link cable, you may
need to install additional drivers which are not installed with TiEmu:

2.1. PortTalk

In order to use TI's BlackLink serial cables or home-made serial or parallel
cables on any NT-based version of Windows (Windows NT 4, Windows 2000, Windows
XP, Windows 2003 or any newer version of Windows), you'll need to install the
PortTalk driver. This driver is not included with this package of TiEmu for
licensing reasons. It can be obtained at:
http://www.beyondlogic.org/porttalk/porttalk.htm

2.2. SilverLink

In order to use TI's SilverLink USB cables (formerly known as TI-GraphLink
USB), you'll need to install the LPG SilverLink driver for Windows at:
http://lpg.ticalc.org/prj_usb
This driver is NOT compatible with the driver used by TI Connect, installing it
will replace TI's driver. In order to use TI Connect, you'll have to uninstall
the LPG driver and reinstall TI's driver. As an alternative, we suggest using
TiLP (TiLP I >= 6.72 or TiLP II) which uses our (LPG) driver.

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