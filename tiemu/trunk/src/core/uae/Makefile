#
# Extra rules for building the emulation engine
#
#CPUFLAGS = -O3 -mpentium -fomit-frame-pointer -Wall -Wno-unused -Wno-format -W -Wmissing-prototypes -Wstrict-prototypes -DX86_ASSEMBLY -D__inline__=inline -DSTATFS_NO_ARGS=2 -DSTATBUF_BAVAIL=f_bavail

# trying to be portable... *TRYING*
#CFLAGS   = -mms-bitfields
CPUFLAGS = $(CFLAGS) -fomit-frame-pointer -Wno-unused -Wno-format -W -Wmissing-prototypes -Wstrict-prototypes -D__inline__=inline -DSTATFS_NO_ARGS=2 -DSTATBUF_BAVAIL=f_bavail #-Wall

gen: cpudefs.c cpustbl.c cputbl.h cpu0.c cpu1.c cpu2.c cpu3.c cpu4.c cpu5.c cpu6.c cpu7.c cpu8.c cpu9.c cpuA.c cpuB.c cpuC.c cpuD.c cpuE.c cpuF.c

all: cpudefs.c cpustbl.o cputbl.h cpu0.o cpu1.o cpu2.o cpu3.o cpu4.o cpu5.o cpu6.o cpu7.o cpu8.o cpu9.o cpuA.o cpuB.o cpuC.o cpuD.o cpuE.o cpuF.o

build68k: build68k.o
	@echo "-> Compiling 68k builder..."
	$(CC) $(LDFLAGS) -o $@ $?
gencpu: gencpu.o readcpu.o cpudefs.o
	@echo "-> Compiling CPU generator..."
	$(CC) $(LDFLAGS) -o $@ gencpu.o readcpu.o cpudefs.o
cpudefs.c: build68k table68k
	@echo "-> Building CPU definitions..."
	./build68k >cpudefs.c
cpustbl.c: gencpu
	@echo "-> Generating CPU table (cpustbl.c)..."
	./gencpu s >cpustbl.c
cputbl.c: gencpu
	@echo "-> Generating CPU table (cputbl.c)..."
	./gencpu t >cputbl.c
cputbl.h: gencpu
	@echo "-> Generating CPU table (cputbl.h)..."
	./gencpu h >cputbl.h
cpu0.c: gencpu
	@echo "-> Generating CPU table (0)..."
	./gencpu f 0 >cpu0.c
cpu1.c: gencpu
	@echo "-> Generating CPU table (1)..."
	./gencpu f 1 >cpu1.c
cpu2.c: gencpu
	@echo "-> Generating CPU table (2)..."
	./gencpu f 2 >cpu2.c
cpu3.c: gencpu
	@echo "-> Generating CPU table (3)..."
	./gencpu f 3 >cpu3.c
cpu4.c: gencpu
	@echo "-> Generating CPU table (4)..."
	./gencpu f 4 >cpu4.c
cpu5.c: gencpu
	@echo "-> Generating CPU table (5)..."
	./gencpu f 5 >cpu5.c
cpu6.c: gencpu
	@echo "-> Generating CPU table (6)..."
	./gencpu f 6 >cpu6.c
cpu7.c: gencpu
	@echo "-> Generating CPU table (7)..."
	./gencpu f 7 >cpu7.c
cpu8.c: gencpu
	@echo "-> Generating CPU table (8)..."
	./gencpu f 8 >cpu8.c
cpu9.c: gencpu
	@echo "-> Generating CPU table (9)..."
	./gencpu f 9 >cpu9.c
cpuA.c: gencpu
	@echo "-> Generating CPU table (A)..."
	./gencpu f 10 >cpuA.c
cpuB.c: gencpu
	@echo "-> Generating CPU table (B)..."
	./gencpu f 11 >cpuB.c
cpuC.c: gencpu
	@echo "-> Generating CPU table (C)..."
	./gencpu f 12 >cpuC.c
cpuD.c: gencpu
	@echo "-> Generating CPU table (D)..."
	./gencpu f 13 >cpuD.c
cpuE.c: gencpu
	@echo "-> Generating CPU table (E)..."
	./gencpu f 14 >cpuE.c
cpuF.c: gencpu
	@echo "-> Generating CPU table (F)..."
	./gencpu f 15 >cpuF.c

cpu0.o: cpu0.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu1.o: cpu1.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu2.o: cpu2.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu3.o: cpu3.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu4.o: cpu4.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu5.o: cpu5.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu6.o: cpu6.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu7.o: cpu7.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu8.o: cpu8.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpu9.o: cpu9.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpuA.o: cpuA.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpuB.o: cpuB.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpuC.o: cpuC.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpuD.o: cpuD.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpuE.o: cpuE.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cpuF.o: cpuF.c cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c

cpustbl.o: cputbl.h
	$(CC) $(INCLUDES) -c $(INCDIRS) $(CPUFLAGS) $*.c
cputbl.o: cputbl.h 
 
build68k.o: readcpu.h 
readcpu.o:  readcpu.h 

clean:
	$(RM) build68k gencpu *.o

distclean: clean
	$(RM) cpudefs.c cpustbl.c cputbl.? cpu?.?