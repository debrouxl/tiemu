How to build TiEmu for iPAQ H3970 ?

First, the main site for Linux PDA support is <http://www.handhelds.org>.
You will find all you need about the Familiar distribution and GPE (GTK).

Cross-compilation is covered here: 
<http://handhelds.org/moin/moin.cgi/GpeCrossCompilation>

We will use the easier method (and probably the best one): OpenEmbedded based 
GPE SDK. The latest one is available here: <http://handhelds.org/~florian/sdk/gpe-sdk-20050210.tar.bz2>.
Almost everything you need should be included in the archive, just unpack it to /.

To crosscompile a GPE application you need to do this:

1. export PKG_CONFIG_PATH=/usr/local/arm/oe/arm-linux/lib/pkgconfig

2. export PATH=/usr/local/arm/oe/bin:$PATH

3. Change to the location of your source

4. Run make CC=arm-linux-gcc to build your application

If you use autotools you will usually need to run ./configure --host=arm-linux 

That's all !

---

Thanks to Andrew Seddon of Cambridge Signal Processing <http://camsig.co.uk)
who donated a PDA (iPAQ PocketPC H3970 with GPE installed).

---

15/04/2005, Romain Liévin