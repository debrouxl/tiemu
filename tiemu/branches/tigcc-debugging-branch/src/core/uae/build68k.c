/* 
 * UAE - The Un*x Amiga Emulator
 *
 * Read 68000 CPU specs from file "table68k" and build table68k.c
 *
 * Copyright 1995,1996 Bernd Schmidt
 */

#include <stdlib.h>
#include "sysdeps.h"
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "uconfig.h"
#include "options.h"
#include "readcpu.h"

static FILE *tablef;
static int nextch = 0;

static void getnextch(void)
{
    do {
	nextch = fgetc(tablef);
	if (nextch == '%') {
	    do {
		nextch = fgetc(tablef);
	    } while (nextch != EOF && nextch != '\n');	
	}
    } while (nextch != EOF && isspace(nextch));	
}

int main(int argc, char **argv)
{
    int no_insns = 0;

    printf ("#include <stdlib.h>\n");   
    printf ("#include \"sysdeps.h\"\n");
    printf ("#include \"uconfig.h\"\n");
    printf ("#include \"options.h\"\n");
    printf ("#include \"readcpu.h\"\n");
    printf ("struct instr_def defs68k[] = {\n");
    tablef = fopen("table68k","r");
    if (tablef == NULL) {
	fprintf(stderr, "table68k not found\n");
	exit(1);
    }
    getnextch();
    while (nextch != EOF) {
	int cpulevel, plevel;
	long int opc;
	int i;
	
	char patbits[16];
	char opcstr[256];
	int bitpos[16];
	
	UWORD bitmask,bitpattern;
	int n_variable;
	
	n_variable = 0;
	bitmask = bitpattern = 0;
	memset (bitpos, 0, sizeof(bitpos));
	for(i=0; i<16; i++) {
	    int currbit;
	    bitmask <<= 1;
	    bitpattern <<= 1;
	
	    switch (nextch) {
	     case '0': currbit = bit0; bitmask |= 1; break;
	     case '1': currbit = bit1; bitmask |= 1; bitpattern |= 1; break;
	     case 'c': currbit = bitc; break;
	     case 'C': currbit = bitC; break;
	     case 'f': currbit = bitf; break;
	     case 'i': currbit = biti; break;
	     case 'I': currbit = bitI; break;
	     case 'j': currbit = bitj; break;
	     case 'J': currbit = bitJ; break;
	     case 's': currbit = bits; break;
	     case 'S': currbit = bitS; break;
	     case 'd': currbit = bitd; break;
	     case 'D': currbit = bitD; break;
	     case 'r': currbit = bitr; break;
	     case 'R': currbit = bitR; break;
	     case 'z': currbit = bitz; break;
	     default: abort();
	    }
	    if (!(bitmask & 1)) {
		bitpos[n_variable] = currbit;
		n_variable++;
	    }
	    
	    if (nextch == '0' || nextch == '1') 
		bitmask |= 1;
	    if (nextch == '1') 
		bitpattern |= 1;
	    patbits[i] = nextch;
	    getnextch();
	}
	while (isspace(nextch) || nextch == ':') 
	    getnextch();
	switch (nextch) {
	 case '0': cpulevel = 0; break;
	 case '1': cpulevel = 1; break;
	 case '2': cpulevel = 2; break;
	 default: abort();
	}
	getnextch();
	
	switch (nextch) {
	 case '0': plevel = 0; break;
	 case '1': plevel = 1; break;
	 case '2': plevel = 2; break;
	 case '3': plevel = 3; break;
	 default: abort();
	}
	getnextch();

	while (isspace(nextch))
	    getnextch();
	
	if (nextch != ':')
	    abort();
	fgets(opcstr, 250, tablef);
	getnextch();
	if (cpulevel <= 0) {
	    int j;
	    /* Remove superfluous spaces from the string */
	    char *opstrp = opcstr, *osendp;
	    int slen = 0;

	    while (isspace(*opstrp))
		opstrp++;
	    
	    osendp = opstrp;
	    while (*osendp) {
		if (!isspace (*osendp))
		    slen = osendp - opstrp + 1;
		osendp++;
	    }
	    opstrp[slen] = 0;
	    
	    if (no_insns > 0)
		printf(",\n");
	    no_insns++;
	    printf("{ %d, %d, {", bitpattern, n_variable);
	    for (j = 0; j < 16; j++) {
		printf("%d", bitpos[j]);
		if (j < 15)
		    printf(",");
	    }
	    printf ("}, %d, %d, \"%s\"}", bitmask, plevel, opstrp);
	}
    }
    printf("};\nint n_defs68k = %d;\n", no_insns);
    fclose(tablef);
    return 0;
}
