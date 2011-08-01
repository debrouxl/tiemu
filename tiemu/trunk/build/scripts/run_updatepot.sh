#! /bin/sh
cd skinedit/trunk/po; make tiemu-skinedit.pot-update; make update-po; cd ../../..
cd tiemu/trunk/po; make tiemu.pot-update; make update-po; cd ../../..
