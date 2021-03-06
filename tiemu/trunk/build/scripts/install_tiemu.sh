#! /bin/sh

# This script, aimed at users, automates the compilation and installation of tiemu
# from the SVN repository.
# It's mirrored at http://lpg.ticalc.org/prj_tiemu/downloads/install_tiemu.sh
#
# **********
# IMPORTANT:
# **********
#     * see below for prerequisites (build dependencies) or peculiarities (e.g. 64-bit Fedora).
#     * you should remove equivalent packages, if any, before running this script.
#
# Copyright (C) Lionel Debroux 2009, 2010, 2011

# libti* and tiemu are compiled with a proposed set of configuration options,
# but you may wish to use others. The complete list is available through
# `./configure --help` run in $SRCDIR/tilp/libticonv, $SRCDIR/tilp/libtifiles,
# $SRCDIR/tilp/libticables, $SRCDIR/tilp/libticalcs, $SRCDIR/tiemu/tiemu.


# The prefix where the binaries will be installed, e.g. $HOME, /usr, /usr/local.
#
# ****************
# IMPORTANT NOTES:
# ****************
# * for compilation to succeed, you may have to execute
# $ export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:[{$PREFIX}]/lib/pkgconfig
# (where [{$PREFIX}] is the contents of the PREFIX line below, without the quotes).
# The main cause for having to execute this line is installing to e.g. PREFIX=$HOME or /usr/local,
# but it may be necessary when installing to PREFIX=/usr, if your distro doesn't store
# libraries into the standard /usr/lib path.
#
# * after successful installation, you may have to add $PREFIX/bin to $PATH,
# and $PREFIX/lib to $LD_LIBRARY_PATH, for the SVN versions of libti* and tiemu
# to get picked up.
#PREFIX="$HOME"
PREFIX="/usr"


# The place where the sources will be stored.
SRCDIR="$HOME/lpg"


# MANDATORY dependencies for compiling and running libti* and tiemu:
# (Debian and Fedora package names are given if they're different from each other)
# * (subversion - for downloading the sources)
# * pkg-config
# * GNU autoconf, automake, libtool
# * glib 2.x development files (libglib2.0-dev, glib2-devel)
# * zlib development files (zlib1g-dev, zlib-devel)
# * libusb development files (libusb-dev, libusb-devel)
#   (libusb 0.1.x preferred, but libticables now has a libusb 1.0 backend activated with "--enable-libusb10", see below)
# * GTK+ 2.x and Glade development files (libgtk2.0-dev + libglade2-dev, gtk2-devel + libglade2-devel)
# * SDL 1.2 development files (libsdl1.2-dev, SDL-devel)
# * libncurses 5 development files (libncurses5-dev, ncurses-devel)
# * GNU gettext
# * GNU bison, flex
# * GNU groff, texinfo
# * XDG utils
# * Qt 3 development headers.
#
# Optional dependencies:
# * KDE 3.x development files (kdelibs4-dev, kdelibs3-devel), if you want to compile TIEmu with support for the KDE file dialog.
#   (this implies a slew of development files, among which the Qt development files, and is therefore disabled by default)
# * DBus-Glib development files (libdbus-glib-1-dev, dbus-devel + dbus-glib-devel), if you want to compile TIEmu with D-Bus support.


# Checkout/update, `configure`, `make` and `make install` the given module
handle_one_module() {
  module_name="$1"
  shift # Kick the first argument, so as to be able to pass the rest to configure.

  if [ -d "$module_name" -a -d "$module_name/.svn" ]; then
    echo "Updating $module_name"
    cd "$module_name"
    svn up || return 1
    cd ../..
  else
    echo "Checking out $module_name"
    svn co "http://svn.tilp.info/repos/$module_name/trunk" "$module_name" || return 1
  fi

  cd "$module_name"
  echo "Configuring $module_name"
  # Add --libdir=/usr/lib64 on e.g. 64-bit Fedora 14, which insists on searching for 64-bit libs in /usr/lib64.
  # Or modify PKG_CONFIG_PATH as described above.
  ./configure "--prefix=$PREFIX" $@ || return 1
  echo "Building $module_name"
  make || return 1
  echo "Installing $module_name"
  make install || return 1
  cd ../..
}

echo -e "\033[4mBefore proceeding further, make sure that you're ready to go (look inside the install script):\033[m"
echo -e "1) configured \033[1mPREFIX\033[m and \033[1mSRCDIR\033[m the way you wish;"
echo -e "2a) if you're using \033[1m64-bit Fedora\033[m (or any distro which installs libraries to non-standard paths), added --libdir=/usr/lib64 to the marked line, or..."
echo -e "2b) configured \033[1mPKG_CONFIG_PATH\033[m if necessary"
echo -e "3) installed the build dependencies listed in the script."
echo -e "\033[4mOtherwise, the build will fail!\033[m."
echo -e "\033[1mENTER to proceed, CTRL + C to abort\033[m."
read


echo "Creating output folder if necessary"
mkdir -p "$SRCDIR/tilp" || exit 1
mkdir -p "$SRCDIR/tiemu" || exit 1

cd "$SRCDIR"
echo "=== libticonv ==="
handle_one_module tilp/libticonv || exit 1
# Useful configure options include --disable-nls.
echo "=== libtifiles ==="
handle_one_module tilp/libtifiles || exit 1
# Useful configure options include --disable-nls, --enable-logging, --enable-libusb10.
echo "=== libticables ==="
handle_one_module tilp/libticables --enable-logging || exit 1
# Useful configure options include --disable-nls.
echo "=== libticalcs ==="
handle_one_module tilp/libticalcs || exit 1

# Useful configure options include --disable-gdb, --with-dbus and --with-kde.
# Use --with-kde if you want to use the native KDE file dialogs (it defaults to disabled because it requires a slew of development package dependencies).
echo "=== tiemu ==="
handle_one_module tiemu/tiemu --with-dbus || exit 1

echo "============================================="
echo "=== libti* + tiemu installed successfully ==="
echo "============================================="
echo ""
echo ""
echo ""
echo "============================================="
echo "IMPORTANT NOTES               IMPORTANT NOTES"
echo "============================================="
echo "If you want to use linking with a real calculator, follow the instructions in $SRCDIR/tilp/libticables/CONFIG"
