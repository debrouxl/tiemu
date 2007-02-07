#!/bin/sh
TODAYS_DATE=`date +%Y%m%d`
sed -e s/'%(date +%%Y%%m%%d)'/$TODAYS_DATE/g tiemu/build/fedora/libticonv.spec >libticonv-$TODAYS_DATE.spec
sed -e s/'%(date +%%Y%%m%%d)'/$TODAYS_DATE/g tiemu/build/fedora/libticables2.spec >libticables2-$TODAYS_DATE.spec
sed -e s/'%(date +%%Y%%m%%d)'/$TODAYS_DATE/g tiemu/build/fedora/libtifiles2.spec >libtifiles2-$TODAYS_DATE.spec
sed -e s/'%(date +%%Y%%m%%d)'/$TODAYS_DATE/g tiemu/build/fedora/libticalcs2.spec >libticalcs2-$TODAYS_DATE.spec
sed -e s/'%(date +%%Y%%m%%d)'/$TODAYS_DATE/g tiemu/build/fedora/tiemu3.spec >tiemu3-$TODAYS_DATE.spec
sed -e s/'%(date +%%Y%%m%%d)'/$TODAYS_DATE/g tiemu/build/fedora/tilp2.spec >tilp2-$TODAYS_DATE.spec
