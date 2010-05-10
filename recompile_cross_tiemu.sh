#!/bin/sh

# Maintainer script for automating the cross-compilation and installation of tiemu
# from a checkout of the complete tiemu repository over at svn.tilp.info.
#
# Copyright (C) 2010 Lionel Debroux


# REMINDERS: don't forget to:
# 1) create AND chmod +x a "i586-mingw32msvc-pkg-config" script in the path:
# #!/bin/sh
# export PKG_CONFIG_LIBDIR=/usr/i586-mingw32msvc/lib
# exec pkg-config $@
# (this is for my Debian Lenny, at least)
#
# 2) create a /target folder pointing to the GTK Wine install path, e.g.
#    $HOME/.wine/drive_c/GTK
#
# 3) if necessary (dependency error about pixman-1), remove "pixman-1 >= 0.10.0"
#    from the "Requires.private:" line in <GTK_INSTALL_DIR>/lib/pkgconfig/cairo.pc.
#
# 4) in order to avoid needless dependencies, edit the following files:
#   /usr/lib/gcc/i586-mingw32msvc/4.2.1-sjlj/libstdc++.la
#   /usr/lib/gcc/i586-mingw32msvc/4.2.1-sjlj/libsupc++.la
#   /usr/lib/gcc/i586-mingw32msvc/4.2.1-sjlj/debug/libstdc++.la
#   and remove "-lmingwthrd" and "-lgcc_s".


# The prefix where the binaries will be installed, e.g. $HOME, /usr, /usr/local, /usr/share.
PREFIX="$HOME/lpg/packages"

# Common flags and definitions.
CCFLAGS="-Os"
CCPPFLAGS="-I$HOME/lpg/deps/gtk-win32/include"
CLDFLAGS="-L$HOME/lpg/deps/gtk-win32/lib"
LIBUSB_DIR="$HOME/lpg/deps/libusb-win32"
CHOST="i586-mingw32msvc"

export PKG_CONFIG_PATH=$HOME/lpg/packages/lib/pkgconfig:$HOME/lpg/deps/gtk-win32/lib/pkgconfig:$PKG_CONFIG_PATH
export PKG_CONFIG_LIBDIR=$HOME/lpg/pkgconfig

# Configure and build the given module
handle_one_module() {
  module_name="$1"
  shift # Kick the first argument, so as to be able to pass the rest to configure.

  cd "$module_name/trunk"
  echo "Configuring $module_name"
  rm -f config.cache
  ./configure -C --host="$CHOST" CFLAGS="$CCFLAGS" --prefix="$PREFIX" CPPFLAGS="$CCPPFLAGS" LDFLAGS="$CLDFLAGS" LIBUSB_CFLAGS="-I$LIBUSB_DIR/include" LIBUSB_LIBS="-L$LIBUSB_DIR/lib/gcc -lusb" $@ || return 1
  echo "Building $module_name"
  make clean || return 1
  make || return 1
  echo "Installing $module_name"
  make install || return 1
  cd ../..
}


echo "=== AUTORECONF ==="
sh run_autoreconf.sh

echo "=== UPDATEPOT ==="
sh run_updatepot.sh


sed "s/+ _nl_msg.*$//" -i skinedit/trunk/configure
sed "s/+ _nl_msg.*$//" -i tiemu/trunk/configure


echo "=== skinedit ==="
handle_one_module skinedit || exit 1


echo "=== tiemu ==="
# XXX need to remove --disable-sound for the final version !
handle_one_module tiemu --disable-sound --disable-gdb || exit 1
