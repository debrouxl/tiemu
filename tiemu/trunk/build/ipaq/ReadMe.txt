How to build TiEmu for iPAQ H3970 ?

First, the main site for Linux PDA support is <http://www.handhelds.org>.
You will find all you need about the Familiar distribution and GPE (GTK).

Cross-compilation is covered here: 
<http://handhelds.org/moin/moin.cgi/GpeCrossCompilation>

Some tips:
<http://www.handhelds.org/minihowto/porting-software.html>

We will use the easier method (and probably the best one): OpenEmbedded based 
GPE SDK. The latest one is available here: <http://handhelds.org/~florian/sdk/gpe-sdk-20050210.tar.bz2>.
Almost everything you need should be included in the archive, just unpack it to /.

To cross-compile TiEmu and the TiLP framework, copy the cross-config.sh script at the top of the source and run it.
If this does'nt work, you can follow the guidelines above...

To build a GPE application, you need to do this:

1. export PKG_CONFIG_PATH=/usr/local/arm/oe/arm-linux/lib/pkgconfig
2. export PATH=/usr/local/arm/oe/bin:$PATH
3. Change to the location of your source
4. Run ./configure --host=arm-linux --disable-nls 
   Option: --prefix=/usr/local/arm/oe/arm-linux --datadir=/usr/local
5. Files are installed in /usr/local/arm/oe/arm-linux if you choose the option.

To debug:

1. Get gdb source (the same version as arm-linux-gdb if possible)
2. Go to gdb, run ./configure --host=arm-linux and do make
3. You will get a gdbserver executable targetted for ARM. You can strip it to reduce size.
3. Copy gdbserver executable onto the iPAQ (with scp). 
4. Run 'gdbserver tiemu host:1234'
5. Run locally 'gdb ./tiemu'
6. Type in 'remote target ipaq:1234'

That's all !

---

Thanks to Andrew Seddon of Cambridge Signal Processing <http://camsig.co.uk)
who donated a PDA (iPAQ PocketPC H3970 with GPE installed).

Thanks to Kevin for some tips about gdb.

---

15/04/2005, Romain Liévin