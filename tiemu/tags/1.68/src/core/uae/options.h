 /*
  * UAE - The Un*x Amiga Emulator
  *
  * cpp magic
  *
  * Copyright 1995, 1996 Bernd Schmidt, Ed Hanway
  */

#ifdef WIN32
#define INLINE_DECLARATION __inline
#else
#define INLINE_DECLARATION __inline__
#endif

#define CPU_EMU_SIZE 0

#if 0



#ifdef __mac__
/* Apparently, no memcpy :-/ */
static __inline__ void *memcpy(void *to, void *from, int size)
{
    BlockMove(from, to, size);
}
#endif

/* strdup() may be non-portable if you have a weird system */
static char *my_strdup(const char*s)
{
    /* The casts to char * are there to shut up the compiler on HPUX */
    char *x = (char*)malloc(strlen((char *)s) + 1);
    strcpy(x, (char *)s);
    return x;
}

#undef REGPARAM
#define fast_memcmp memcmp
#define memcmpy generic_memcmpy

static __inline__ int generic_memcmpy(void *foo, const void *bar, int len)
{
    int res = memcmp(foo, bar, len);
    if (res)
	memcpy(foo, bar, len);
    return res;
}

#if defined(__GNUC_MINOR__)

#ifdef __i386__

//#define INTEL_FLAG_OPT    //fuck !! asm compile po, fait chier

#undef fast_memcmp
#undef memcmpy

static __inline__ int fast_memcmp(const void *foo, const void *bar, int len)
{
    int differs, baz;
    __asm__ __volatile__ ("subl $4, %2\n"
			  "jc  2f\n"
			  "1:\n"
			  "movl (%0),%%ebx\n"
			  "cmpl (%1),%%ebx\n"
			  "jne 5f\n"
			  "addl $4, %0\n"
			  "addl $4, %1\n"
			  "subl $4, %2\n"
			  "jnc  1b\n"
			  "2:\n"
			  "addl $4, %2\n"
			  "jz 4f\n"
			  "3:\n"
			  "movb (%0),%%bl\n"
			  "cmpb (%1),%%bl\n"
			  "jne 5f\n"
			  "incl %0\n"
			  "incl %1\n"
			  "decl %2\n"
			  "jnz 3b\n"
			  "4:\n"
			  "movl $0, %3\n"
			  "jmp 6f\n"
			  "5:\n"
			  "movl $1, %3\n"
			  "6:\n"
			  : "=&r" (foo), "=&r" (bar), "=&rm" (len), "=rm" (differs),
			    "=&b" (baz)
			  : "0" (foo), "1" (bar), "2" (len), "3" (baz) : "cc");
    return differs;
}

static __inline__ int memcmpy(void *foo, const void *bar, int len)
{
    int differs, baz = 0, uupzuq = 0;
/*    printf("%p %p %d\n", foo, bar, len);*/
    __asm__ __volatile__ ("subl %1, %2\n"
			  "movl $0, %0\n"
			  "subl $16, %3\n"
			  "jc 7f\n"
			  
			  "8:\n"
			  "movl (%1),%%ecx\n"
			  "movl (%2,%1),%%ebx\n"
			  "xorl %%ebx, %%ecx\n"
			  "movl %%ebx, (%1)\n"
			  "orl %%ecx, %0\n"
			  
			  "movl 4(%2,%1),%%ebx\n"
			  "movl 4(%1),%%ecx\n"
			  "xorl %%ebx, %%ecx\n"
			  "movl %%ebx, 4(%1)\n"
			  "orl %%ecx, %0\n"
			  
			  "movl 8(%2,%1),%%ebx\n"
			  "movl 8(%1),%%ecx\n"
			  "xorl %%ebx, %%ecx\n"
			  "movl %%ebx, 8(%1)\n"
			  "orl %%ecx, %0\n"
			  
			  "movl 12(%2,%1),%%ebx\n"
			  "movl 12(%1),%%ecx\n"
			  "xorl %%ebx, %%ecx\n"
			  "movl %%ebx, 12(%1)\n"
			  "orl %%ecx, %0\n"
			  
			  "addl $16, %1\n"
			  "subl $16, %3\n"
			  "jnc  8b\n"
			  
			  "7:\n"
			  "addl $16, %3\n"
			  "subl $4, %3\n"
			  "jc  2f\n"
			  
			  "1:\n"
			  "movl (%2,%1),%%ebx\n"
			  "movl (%1),%%ecx\n"
			  "xorl %%ebx, %%ecx\n"
			  "movl %%ebx, (%1)\n"
			  "orl %%ecx, %0\n"
			  "addl $4, %1\n"
			  "subl $4, %3\n"
			  "jnc  1b\n"
			  
			  "2:\n"
			  "addl $4, %3\n"
			  "jz 6f\n"
			  "xorl %%ecx, %%ecx\n"
			  
			  "3:\n"
			  "movb (%2,%1),%%bl\n"
			  "movb (%1),%%cl\n"
			  "xorl %%bl,%%cl\n"
			  "movb %%bl,(%1)\n"
			  "orl %%ecx, %0\n"
			  "incl %1\n"
			  "decl %3\n"
			  "jnz 3b\n"
			  
			  "6:\n"
			  : "=m" (differs)
			  : "r" (foo), "r" (bar), "r" (len), "b" (baz), "c" (uupzuq) : "cc", "memory");
    /* Now tell the compiler that foo, bar and len have been modified 
     * If someone finds a way to express all this cleaner in constraints that
     * GCC 2.7.2 understands, please FIXME */
    __asm__ __volatile__ ("" : "=rm" (foo), "=rm" (bar), "=rm" (len) : :  "ebx", "ecx", "edx", "eax", "esi", "memory");
/*    printf("bar\n");*/
    return differs;
}
#endif

#if __GNUC__ > 2 || __GNUC_MINOR__ > 6

#if defined(__i386__) && !defined(USE_PROFILING)
#define REGPARAM __attribute__((regparm(3)))
#endif /* __i386__ */

#endif /* GCC version 2.7 or higher */
#endif /* GCC 2 */

#ifndef REGPARAM
#define REGPARAM
#endif



/*
 * You can specify numbers from 0 to 5 here. It is possible that higher
 * numbers will make the CPU emulation slightly faster, but if the setting
 * is too high, you will run out of memory while compiling.
 * Best to leave this as it is.
 */
#define CPU_EMU_SIZE 0

/* Include Debugger in emulator */
#define DEBUGGER

/* Use the Pentium Internal Cycle Counter for timing */
#define PENT_COUNTER

/* Define for unix-like interface optiions */
#define IN_UNIX







#endif
