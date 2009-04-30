dnl Process this file with autoconf to produce a configure script.
dnl $Id$

dnl Init.
AC_PREREQ(2.57)
AC_INIT(tiemu-skinedit, 1.27, [tiemu-users@lists.sourceforge.net])

AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AC_PREFIX_DEFAULT(/usr/local)

dnl Files to configure.
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  build/Makefile
  man/Makefile
  pixmaps/Makefile
  src/Makefile
  po/Makefile.in
])

# Setup libtool.
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Checks for programs.
AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_BIGENDIAN
AC_CHECK_PROG(GROFF, groff, yes, no)
AM_CONDITIONAL(USE_GROFF, test "$GROFF" != "no")

# Checks for libraries.
AM_GNU_GETTEXT(external)

PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.4.0)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.4.0)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

# Checks for header files.
AC_CHECK_HEADERS(stdint.h)

# Checks for library functions.
AC_FUNC_MALLOC

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

# Output.
AC_OUTPUT

echo "Now, you can type 'make' and 'make install'."