#! /bin/sh

# Maintainer script for automating the compilation and installation of tilp & gfm
# from a checkout of the complete tilp repository over at svn.tilp.info.
#
# Copyright (C) 2010 Lionel Debroux

# The prefix where the binaries will be installed, e.g. $HOME, /usr, /usr/local, /usr/share.
PREFIX="$HOME"

# Configure and build the given module
handle_one_module() {
  module_name="$1"
  shift # Kick the first argument, so as to be able to pass the rest to configure.

  cd "$module_name/trunk"
  echo "Configuring $module_name"
  rm -f config.cache
  ./configure "--prefix=$PREFIX" $@ || return 1
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


echo "=== skinedit ==="
handle_one_module skinedit || exit 1

echo "=== tiemu ==="
handle_one_module tiemu --with-dbus --disable-gdb || exit 1
