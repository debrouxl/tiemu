dnl Process this file with autoconf to produce a configure script.
dnl $Id$

dnl Init.
AC_PREREQ(2.57)
AC_INIT(SkinEdit, 1.26, [tiemu-users@lists.sourceforge.net])

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
  po/Makefile.in
  src/Makefile
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
#AM_GNU_GETTEXT
#AM_GNU_GETTEXT_VERSION(0.13.1)

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

# Output.
AC_OUTPUT
