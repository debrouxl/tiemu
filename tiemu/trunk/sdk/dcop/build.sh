#!/bin/sh
g++ -Os -s -fno-exceptions -I$QTDIR/include -I/usr/include/kde dcoptest.cpp tiemu_stub.cpp -L$QTDIR/lib -lqt-mt -L`kde-config --prefix`/lib`kde-config --libsuffix`/kde3 -lkdecore -lkio -o dcoptest
