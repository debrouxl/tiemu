 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Various stuff missing in some OSes.
  *
  * Copyright 1997 Bernd Schmidt
  */

#include <stdlib.h>
#include <string.h>
#include "sysdeps.h"

#include "uconfig.h"
#include "options.h"

void *xmalloc(size_t n)
{
    void *a = malloc (n);
    if (a == NULL) {
	fprintf (stderr, "virtual memory exhausted\n");
	abort ();
    }
    return a;
}
