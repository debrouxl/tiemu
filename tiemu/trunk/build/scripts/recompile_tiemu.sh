#! /bin/sh

# Maintainer script for automating the compilation and installation of tiemu
# from a checkout of the complete tiemu repository over at svn.tilp.info.
#
# Copyright (C) 2010, 2011 Lionel Debroux

# The prefix where the binaries will be installed, e.g. $HOME, /usr, /usr/local.
PREFIX="$HOME"

# Common flags and definitions.
CCFLAGS="-Os -g3 -Wall -W -Wno-unused-parameter -Wshadow -Wwrite-strings -Wredundant-decls -fstack-protector-all -Wstack-protector --param=ssp-buffer-size=1 -Wp,-D_FORTIFY_SOURCE=2"

# Configure and build the given module
handle_one_module() {
  module_name="$1"
  shift # Kick the first argument, so as to be able to pass the rest to configure.

  cd "$module_name/trunk"
  echo "Configuring $module_name"
  rm -f `find . -iname config.cache`
  ./configure CFLAGS="$CCFLAGS" CXXFLAGS="$CCFLAGS" "--prefix=$PREFIX" $@ || return 1
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
