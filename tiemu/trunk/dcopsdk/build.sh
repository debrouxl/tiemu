#!/bin/sh
g++ -Os -s -fno-exceptions -I$QTDIR/include -I/usr/include/kde dcoptest.cpp tiemu_stub.cpp -L$QTDIR/lib -lqt-mt -L/usr/lib/kde3 -lkdecore -lkio -o dcoptest