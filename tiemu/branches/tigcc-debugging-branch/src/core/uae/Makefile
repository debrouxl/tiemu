#
# Extra rules for building the emulation engine
#
#CPUFLAGS = -O3 -mpentium -fomit-frame-pointer -Wall -Wno-unused -Wno-format -W -Wmissing-prototypes -Wstrict-prototypes -DX86_ASSEMBLY -D__inline__=inline -DSTATFS_NO_ARGS=2 -DSTATBUF_BAVAIL=f_bavail

# trying to be portable... *TRYING*
#CFLAGS   = -mms-bitfields
CPUFLAGS = $(CFLAGS) -fomit-frame-pointer -Wno-unused -Wno-format -W -Wmissing-prototypes -Wstrict-prototypes -D__inline__=inline -DSTATFS_NO_ARGS=2 -DSTATBUF_BAVAIL=f_bavail #-Wall

gen: cpudefs.c cpustbl.c cputbl.h cpuemu.c

all: cpudefs.c cpustbl.o cputbl.h cpuemu1.o cpuemu2.o cpuemu3.o cpuemu4.o cpuemu5.o cpuemu6.o cpuemu7.o cpuemu8.o

build68k: build68k.o
	@echo "-> Compiling 68k builder..."
	$(CC) $(LDFLAGS) -o $@ $?
gencpu: gencpu.o readcpu.o cpudefs.o missing.o xmalloc.o
	@echo "-> Compiling CPU generator..."
	$(CC) $(LDFLAGS) -o $@ gencpu.o readcpu.o cpudefs.o missing.o xmalloc.o

cpudefs.c: build68k table68k
	@echo "-> Building CPU definitions..."
	./build68k <table68k >cpudefs.c
cpuemu.c: gencpu compiler.h
	@echo "-> Generating CPU tables..."
	./gencpu
# gencpu also creates cpustbl.c and cputbl.h
cpustbl.c: cpuemu.c compiler.h
cputbl.h: cpuemu.c

cpuemu1.o: cpuemu.c
	$(CC) -DPART_1 $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $< -o $@
cpuemu2.o: cpuemu.c
	$(CC) -DPART_2 $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $< -o $@
cpuemu3.o: cpuemu.c
	$(CC) -DPART_3 $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $< -o $@
cpuemu4.o: cpuemu.c
	$(CC) -DPART_4 $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $< -o $@
cpuemu5.o: cpuemu.c
	$(CC) -DPART_5 $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $< -o $@
cpuemu6.o: cpuemu.c
	$(CC) -DPART_6 $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $< -o $@
cpuemu7.o: cpuemu.c
	$(CC) -DPART_7 $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $< -o $@
cpuemu8.o: cpuemu.c
	$(CC) -DPART_8 $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $< -o $@

cpustbl.o: cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cputbl.o: cputbl.h 
 
build68k.o: readcpu.h 
readcpu.o:  readcpu.h 
newcpu.o:   compiler.h

clean:
	$(RM) build68k gencpu *.o
	$(RM) build68k.exe gencpu.exe

distclean: clean
	$(RM) cpudefs.c cpustbl.c cputbl.? cpuemu.c cpuemu?.o