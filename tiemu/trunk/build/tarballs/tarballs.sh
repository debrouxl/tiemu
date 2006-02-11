#! /bin/sh
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libticables2-`date +%Y%m%d`.tar.bz2 libticables
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libtifiles2-`date +%Y%m%d`.tar.bz2 libtifiles
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libticalcs2-`date +%Y%m%d`.tar.bz2 libticalcs
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f tiemu3-`date +%Y%m%d`.tar.bz2 tiemu
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f tilp2-`date +%Y%m%d`.tar.bz2 tilp

