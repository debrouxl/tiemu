#!/bin/sh
i386-mingw32-gcc -Os -s -fno-exceptions oletest.c tiemuole.c -lole32 -loleaut32 -luuid -o oletest.exe