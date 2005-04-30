#! /bin/sh
tar cfj libticables-`date +%Y%m%d`.tar.bz2 libticables --exclude='\.svn' --exclude='.*/\.svn'
tar cfj libtifiles-`date +%Y%m%d`.tar.bz2 libtifiles --exclude='\.svn' --exclude='.*/\.svn'
tar cfj libticalcs-`date +%Y%m%d`.tar.bz2 libticalcs --exclude='\.svn' --exclude='.*/\.svn'
tar cfj tiemu-tigcc-debugging-`date +%Y%m%d`.tar.bz2 tiemu-tigcc-debugging --exclude='\.svn' --exclude='.*/\.svn'
tar cfj tilp-`date +%Y%m%d`.tar.bz2 tilp --exclude='\.svn' --exclude='.*/\.svn'

