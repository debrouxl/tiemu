#! /bin/sh
tar cfj libticables-svn-head.tar.bz2 libticables --exclude='\.svn' --exclude='.*/\.svn'
tar cfj libtifiles-svn-head.tar.bz2 libtifiles --exclude='\.svn' --exclude='.*/\.svn'
tar cfj libticalcs-svn-head.tar.bz2 libticalcs --exclude='\.svn' --exclude='.*/\.svn'
tar cfj tiemu-tigcc-debugging-svn-head.tar.bz2 tiemu-tigcc-debugging --exclude='\.svn' --exclude='.*/\.svn'
tar cfj tilp-svn-head.tar.bz2 tilp --exclude='\.svn' --exclude='.*/\.svn'

