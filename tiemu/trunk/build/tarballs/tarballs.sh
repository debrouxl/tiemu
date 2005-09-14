#! /bin/sh
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libticables2-`date +%Y%m%d`.tar.bz2 libticables2
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libtifiles2-`date +%Y%m%d`.tar.bz2 libtifiles2
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libticalcs2-`date +%Y%m%d`.tar.bz2 libticalcs2
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f tiemu3-`date +%Y%m%d`.tar.bz2 tiemu
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f tilp2-`date +%Y%m%d`.tar.bz2 tilp2

