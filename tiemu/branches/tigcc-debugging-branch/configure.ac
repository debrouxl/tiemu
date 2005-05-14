dnl Process this file with autoconf to produce a configure script.
dnl $Id: configure.in 409 2004-06-07 21:10:20Z roms $

dnl Init.
AC_PREREQ(2.57)
AC_INIT(TiEmu, 1.80-050427, [tiemu-users@lists.sourceforge.net])

AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AC_PREFIX_DEFAULT(/usr/local)

dnl Files to configure.
AC_CONFIG_SRCDIR([src/gui/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  build/Makefile
  glade/Makefile
  help/Makefile
  man/Makefile
  pedrom/Makefile
  pixmaps/Makefile
  skins/Makefile
  src/Makefile
  tiemu.lsm
  tiemu.spec
])

AC_MSG_CHECKING([for something to drink while compiling])
AC_MSG_RESULT([err: no fridge found!])

# Setup libtool.
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S
AC_ISC_POSIX
AC_CHECK_PROG(GROFF, groff, yes, no)
AM_CONDITIONAL(USE_GROFF, test "$GROFF" != "no")

# Checks for libraries.
AM_GNU_GETTEXT
AM_GNU_GETTEXT_VERSION(0.13.1)

PKG_CHECK_MODULES(TICABLES, ticables >= 3.8.0)
AC_SUBST(TICABLES_CFLAGS)
AC_SUBST(TICABLES_LIBS)

PKG_CHECK_MODULES(TIFILES, tifiles >= 0.5.7)
AC_SUBST(TIFILES_CFLAGS)
AC_SUBST(TIFILES_LIBS)

PKG_CHECK_MODULES(TICALCS, ticalcs >= 4.5.2)
AC_SUBST(TICALCS_CFLAGS)
AC_SUBST(TICALCS_LIBS)

PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.4.0)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.4.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES(GLADE, libglade-2.0 >= 2.4.0)
AC_SUBST(GLADE_CFLAGS)
AC_SUBST(GLADE_LIBS)

# Checks for header files.
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([libintl.h])
AC_CHECK_HEADERS([sys/timeb.h])
AC_CHECK_HEADERS(stdint.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_FUNC_MEMCMP
#AC_C_RESTRICT
AC_C_VOLATILE

# Checks for libraries.
AC_CHECK_LIB(m, pow)
AC_CHECK_LIB(pthread, pthread_create) dnl needed by GThread
#AC_CHECK_LIB(comdlg32, GetOpenFileName)

# Checks for library functions.
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT 
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([bzero ftime memset mkdir select strchr strrchr strstr])

# Platform specific tests.
dnl AC_CANONICAL_HOST
case $host in
  *-*-mingw*)            ARCH="-D__WIN32__ -D__MINGW32__ -mwindows -D_WINDOWS" ;;
  *-*-*bsd*)             ARCH="-D__BSD__" ;;
  i[[3456]]86-*-linux-*) ARCH="-D__I386__ -D__LINUX__" ;; 
  arm*-*-linux-*)        ARCH="-D__IPAQ__ -D__LINUX__" ;;
  powerpc-*-linux-*)     ARCH="-D__PPC__  -D__LINUX__" ;;
  powerpc-apple-darwin*) ARCH="-D__PPC__  -D__MACOSX__" ;;
  *)                     ARCH="-D__LINUX__" ;;
esac
CFLAGS="$CFLAGS $ARCH"

# KDE dialogs support
AC_ARG_WITH(kde, AC_HELP_STRING([--with-kde], [Compile with KDE support]), [kde=$withval], [kde=no])
if test "x$kde" = "xyes"; then
        AC_PROG_CXX
        AC_PATH_KDE
        AC_DEFINE(WITH_KDE, 1, [Use KDE support])
fi
AC_SUBST(kde)

# Ensure MSVC-compatible struct packing convention is used when
# compiling for Win32 with gcc.
# What flag to depends on gcc version: gcc3 uses "-mms-bitfields", while
# gcc2 uses "-fnative-struct".
case $host_os in
*mingw*|*cygwin*)
  if test x$GCC = xyes; then
    msnative_struct=''
    AC_MSG_CHECKING([how to get MSVC-compatible struct packing])
    if test -z "$ac_cv_prog_CC"; then
      our_gcc="$CC"
    else
      our_gcc="$ac_cv_prog_CC"
    fi
    case `$our_gcc --version | sed -e 's,\..*,.,' -e q` in
      2.)
        if $our_gcc -v --help 2>/dev/null | grep fnative-struct >/dev/null; then
          msnative_struct='-fnative-struct'
        fi
        ;;
      *)
        if $our_gcc -v --help 2>/dev/null | grep ms-bitfields >/dev/null; then
          msnative_struct='-mms-bitfields'
        fi
        ;;
    esac
    if test x"$msnative_struct" = x ; then
      AC_MSG_RESULT([no way])
      AC_MSG_WARN([produced libraries might be incompatible with MSVC-compiled code])
    else
      CFLAGS="$CFLAGS $msnative_struct"
      AC_MSG_RESULT([${msnative_struct}])
    fi
  fi
  ;;
esac

#AM_CONDITIONAL(OS_WIN32, test "$msnative_struct")

# some extra flags
CFLAGS="$CFLAGS -DGTK_DISABLE_DEPRECATED -DDEBUGGER"
LDFLAGS="$LDFLAGS"
CXXFLAGS="$CFLAGS -fno-rtti -fno-exceptions"

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(CXXFLAGS)

# Run GDB configury
ac_configure_args="$ac_configure_args --target=m68k-coff --disable-tui"
changequote(<<,>>)
ac_configure_args=`echo "$ac_configure_args" | sed "s,'CFLAGS=[^']*',,g"`
changequote([,])
AC_CONFIG_SUBDIRS(src/core/gdb)

# Output.
AC_OUTPUT

echo
echo "To run the TiEmu emulator, you will need a ROM dump. As those files are"
echo "copyrighted by Texas Instruments, they will never be distributed with"
echo "the emulator... In replacement, you can use PedRom which is provided"
echo "with TiEmu."
echo
echo "It you have no ROM, you can get an ROM dump of your calculator"
echo "with the 'ROM dump' function of TiLP."
echo "Otherwise, you can use a TI's FLASH OS upgrade (AMS) as image."
echo
echo "Note: TiLP is available at [http://lpg.ticalc.org/prj_tilp]."
echo
echo "Now, you can type 'make' and 'make install'"
echo ;

