#!/bin/sh
export PKG_CONFIG_PATH=/usr/local/arm/oe/arm-linux/lib/pkgconfig
export PATH=/usr/local/arm/oe/bin:$PATH
./configure --host=arm-linux --target=arm-linux --build=i386-linux --disable-nls --prefix=/usr/local/arm/oe/arm-linux
