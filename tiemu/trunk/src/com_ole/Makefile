ifeq ($(OSTYPE),msys)
CC ?= gcc
DLLTOOL ?= dlltool
else
CC ?= i386-mingw32-gcc
DLLTOOL ?= i386-mingw32-dlltool
endif

CFLAGS ?= -Os -s -fno-exceptions

tiemups.dll: dlldata.o oleaut_p.o oleaut_i.o librpcrt4nt4.a
	$(CC) -shared $(CFLAGS) -o tiemups.dll -Wl,--enable-stdcall-fixup \
		dlldata.o oleaut_p.o oleaut_i.o \
		-lkernel32 -lrpcns4 -lrpcrt4 librpcrt4nt4.a -loleaut32 -luuid \
		tiemups.def

lib%.a: %.def
	$(DLLTOOL) $(DLLTOOL_FLAGS) --output-lib $@ --def $<

oleaut_p.o: oleaut_p.c
	$(CC) -c $(CFLAGS) -O0 -fno-omit-frame-pointer \
		-DWIN32 -D_WIN32_WINNT=0x0400 -DREGISTER_PROXY_DLL \
		-Doleaut_ProxyFileInfo=tiemu_ProxyFileInfo \
		oleaut_p.c

.c.o:
	$(CC) -c $(CFLAGS) -DWIN32 -D_WIN32_WINNT=0x0400 -DREGISTER_PROXY_DLL \
		$<

clean:
	rm -f tiemups.dll dlldata.o oleaut_p.o oleaut_i.o librpcrt4nt4.a
