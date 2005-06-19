#! /bin/sh
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libticables-`date +%Y%m%d`.tar.bz2 libticables
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libtifiles-`date +%Y%m%d`.tar.bz2 libtifiles
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f libticalcs-`date +%Y%m%d`.tar.bz2 libticalcs
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f tiemu-tigcc-debugging-`date +%Y%m%d`.tar.bz2 tiemu-tigcc-debugging
tar -cj --exclude='\.svn' --exclude='.*/\.svn' -f tilp-`date +%Y%m%d`.tar.bz2 tilp

