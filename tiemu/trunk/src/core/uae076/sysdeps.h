 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Try to include the right system headers and get other system-specific
  * stuff right & other collected kludges.
  *
  * If you think about modifying this, think twice. Some systems rely on
  * the exact order of the #include statements. That's also the reason
  * why everything gets included unconditionally regardless of whether
  * it's actually needed by the .c file.
  *
  * Copyright 1996, 1997 Bernd Schmidt
  */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_VALUES_H
#include <values.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UTIME_H
#include <utime.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#ifdef HAVE_SYS_VFS_H
#include <sys/vfs.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif

#ifdef HAVE_SYS_STATFS_H
#include <sys/statfs.h>
#endif

#ifdef HAVE_SYS_STATVFS_H
#include <sys/statvfs.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#if HAVE_DIRENT_H
# include <dirent.h>
#else
# define dirent direct
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <errno.h>
#include <assert.h>

#if EEXIST == ENOTEMPTY
#define BROKEN_OS_PROBABLY_AIX
#endif

#ifdef __NeXT__
#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IXUSR S_IEXEC
#define S_ISDIR(val) (S_IFDIR & val)
struct utimbuf
{
    time_t actime;
    time_t modtime;
};
#endif

#if defined(__GNUC__) && defined(AMIGA)
/* gcc on the amiga need that __attribute((regparm)) must */
/* be defined in function prototypes as well as in        */
/* function definitions !                                 */
#define REGPARAM2 REGPARAM
#else /* not(GCC & AMIGA) */
#define REGPARAM2
#endif

/* sam: some definitions so that SAS/C can compile UAE */
#if defined(__SASC) && defined(AMIGA)
#define REGPARAM2 
#define REGPARAM
#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IXUSR S_IEXECUTE
#define S_ISDIR(val) (S_IFDIR & val)
#define mkdir(x,y) mkdir(x)
#define truncate(x,y) 0
#define creat(x,y) open("T:creat",O_CREAT|O_TEMP|O_RDWR) /* sam: for zfile.c */
#define strcasecmp stricmp
#define utime(file,time) 0
struct utimbuf
{
    time_t actime;
    time_t modtime;
};
#endif

#ifdef __DOS__
#include <pc.h>
#include <io.h>
#else
#undef O_BINARY
#define O_BINARY 0
#endif

/* Acorn specific stuff */
#ifdef ACORN

#define S_IRUSR S_IREAD
#define S_IWUSR S_IWRITE
#define S_IXUSR S_IEXEC

#define strcasecmp stricmp

#endif

#ifndef L_tmpnam
#define L_tmpnam 128 /* ought to be safe */
#endif

/* If char has more then 8 bits, good night. */
typedef unsigned char uae_u8;
typedef signed char uae_s8;

typedef struct { uae_u8 RGB[3]; } RGB;

#if SIZEOF_SHORT == 2
typedef unsigned short uae_u16;
typedef short uae_s16;
#elif SIZEOF_INT == 2
typedef unsigned int uae_u16;
typedef int uae_s16;
#else
#error No 2 byte type, you lose.
#endif

#if SIZEOF_INT == 4
typedef unsigned int uae_u32;
typedef int uae_s32;
#elif SIZEOF_LONG == 4
typedef unsigned long uae_u32;
typedef long uae_s32;
#else
#error No 4 byte type, you lose.
#endif

typedef uae_u32 uaecptr;

#undef uae_s64
#undef uae_u64

#if SIZEOF_LONG_LONG == 8
#define uae_s64 long long
#define uae_u64 long long
#define VAL64(a) (a ## LL)
#define UVAL64(a) (a ## uLL)
#elif SIZEOF___INT64 == 8
#define uae_s64 __int64
#define uae_u64 unsigned __int64
#define VAL64(a) (a)
#define UVAL64(a) (a)
#elif SIZEOF_LONG == 8
#define uae_s64 long;
#define uae_u64 unsigned long;
#define VAL64(a) (a ## l)
#define UVAL64(a) (a ## ul)
#endif

#ifdef HAVE_STRDUP
#define my_strdup strdup
#else
extern char *my_strdup (const char*s);
#endif

#ifndef HAVE_GETOPT
extern char *optarg;
extern int getopt (int, char * const *, const char *);
#endif

extern void *xmalloc(size_t);

/* We can only rely on GNU C getting enums right. Mickeysoft VSC++ is known
 * to have problems, and it's likely that other compilers choke too. */
#ifdef __GNUC__
#define ENUMDECL typedef enum
#define ENUMNAME(name) name
#else
#define ENUMDECL enum
#define ENUMNAME(name) ; typedef int name
#endif

/*
 * Porters to weird systems, look! This is the preferred way to get
 * filesys.c (and other stuff) running on your system. Define the
 * appropriate macros and implement wrappers in a machine-specific file.
 *
 * I guess the Mac port could use this (Ernesto?)
 */

#undef DONT_HAVE_POSIX
#undef DONT_HAVE_REAL_POSIX /* define if open+delete doesn't do what it should */
#undef DONT_HAVE_STDIO
#undef DONT_HAVE_MALLOC

#ifdef _WIN32
//#define DONT_HAVE_POSIX
/* Eeek, Mathias, does Windows _really_ not have ANSI stdio? I've accepted
 * that Macs are awfully broken, but is Windows really as bad? */
//#define DONT_HAVE_STDIO
//#define DONT_HAVE_MALLOC
#endif

#ifdef DONT_HAVE_POSIX

#define access posixemu_access
extern int posixemu_access(char *,int);
#define open posixemu_open
extern int posixemu_open(char *,int,int);
#define close posixemu_close
extern void posixemu_close(int);
#define read posixemu_read
extern int posixemu_read(int, char *, int);
#define write posixemu_write
extern int posixemu_write(int, char *, int);
#undef lseek
#define lseek posixemu_seek
extern int posixemu_seek(int, int, int);
#define stat(a,b) posixemu_stat(a,b)
extern int posixemu_stat(char *, STAT *);
#define mkdir posixemu_mkdir
extern int mkdir(char *, int);
#define rmdir posixemu_rmdir
extern int posixemu_rmdir(char *);
#define unlink posixemu_unlink
extern int posixemu_unlink(char *);
#define rename posixemu_rename
extern int posixemu_rename(char *, char *);
#define chmod posixemu_chmod
extern int posixemu_chmod(char *, int);
#define tmpnam posixemu_tmpnam
extern void posixemu_tmpnam(char *);
#define utime posixemu_utime
extern void posixemu_utime(const char *, struct utimbuf *);
/* This isn't the best place for this, but it fits reasonably well. The logic
 * is that you probably don't have POSIX errnos if you don't have the above
 * functions. */
extern long dos_errno(void);

#endif

#ifdef DONT_HAVE_REAL_POSIX

extern FILE *uae_fopen_del (const char *name, const char *mode);
extern int uae_open_del (const char *name, int flags, int mode);

#else

static __inline__ FILE *uae_fopen_del (const char *name, const char *mode)
{
    FILE *fd;
    fd = fopen (name, mode);
    if (fd) {
	unlink (name);
    }
    return fd;
}

static __inline__ int uae_open_del (const char *name, int flags, int mode)
{
    int fd;
    fd = open (name, mode, flags);
    if (fd >= 0) {
	unlink (name);
    }
    return fd;
}

#endif

#ifdef DONT_HAVE_STDIO

/* Mathias: what about fopen? */
extern int stdioemu_fseek(FILE *,int,int);
#define fseek(a,b,c) stdioemu_fseek(a,b,c)
extern int stdioemu_fread(char *,int,int, FILE *);
#define fread(a,b,c,d) stdioemu_fread(a,b,c,d)
extern int stdioemu_fwrite(char *,int,int, FILE *);
#define fwrite(a,b,c,d) stdioemu_fwrite(a,b,c,d)
extern int stdioemu_ftell(FILE *);
#define ftell(a) stdioemu_ftell(a)
extern int stdioemu_fclose(FILE *);
#define fclose(a) stdioemu_fclose(a)

#endif

#ifdef DONT_HAVE_MALLOC

#define malloc(a) mallocemu_malloc(a)
extern void *mallocemu_malloc(int size);
#define free(a) mallocemu_free(a)
extern void mallocemu_free(void *ptr);

#endif

#ifdef X86_ASSEMBLY
#define ASM_SYM_FOR_FUNC(a) __asm__(a)
#else
#define ASM_SYM_FOR_FUNC(a)
#endif

#if defined USE_COMPILER
#undef NO_PREFETCH_BUFFER
#undef NO_EXCEPTION_3
#define NO_EXCEPTION_3
#define NO_PREFETCH_BUFFER
#endif

//#include "target.h"
