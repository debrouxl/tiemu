# tcl.m4 --
#
#	This file provides a set of autoconf macros to help TEA-enable
#	a Tcl extension.
#
# Copyright (c) 1999 Scriptics Corporation.
#
# See the file "license.terms" for information on usage and redistribution
# of this file, and for a DISCLAIMER OF ALL WARRANTIES.

#------------------------------------------------------------------------
# SC_PATH_TCLCONFIG --
#
#	Locate the tclConfig.sh file and perform a sanity check on
#	the Tcl compile flags
#
# Arguments:
#	none
#
# Results:
#
#	Adds the following arguments to configure:
#		--with-tcl=...
#
#	Defines the following vars:
#		TCL_BIN_DIR	Full path to the directory containing
#				the tclConfig.sh file
#------------------------------------------------------------------------

AC_DEFUN(SC_PATH_TCLCONFIG, [
    #
    # Ok, lets find the tcl configuration
    # First, look for one uninstalled.
    # the alternative search directory is invoked by --with-tcl
    #

    if test x"${no_tcl}" = x ; then
	# we reset no_tcl in case something fails here
	no_tcl=true
	AC_ARG_WITH(tcl, [  --with-tcl              directory containing tcl configuration (tclConfig.sh)], with_tclconfig=${withval})
	AC_MSG_CHECKING([for Tcl configuration])
	AC_CACHE_VAL(ac_cv_c_tclconfig,[

	    # First check to see if --with-tcl was specified.
	    if test x"${with_tclconfig}" != x ; then
		if test -f "${with_tclconfig}/tclConfig.sh" ; then
		    ac_cv_c_tclconfig=`(cd ${with_tclconfig}; pwd)`
		else
		    AC_MSG_ERROR([${with_tclconfig} directory doesn't contain tclConfig.sh])
		fi
	    fi

	    # then check for a private Tcl installation
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in \
			../tcl \
			`ls -dr ../tcl[[8-9]].[[0-9]]* 2>/dev/null` \
			../../tcl \
			`ls -dr ../../tcl[[8-9]].[[0-9]]* 2>/dev/null` \
			../../../tcl \
			`ls -dr ../../../tcl[[8-9]].[[0-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tclConfig.sh" ; then
			ac_cv_c_tclconfig=`(cd $i/unix; pwd)`
			break
		    fi
		    if test -f "$i/win/tclConfig.sh" ; then
			ac_cv_c_tclconfig=`(cd $i/win; pwd)`
			break
		    fi
		done
	    fi

	    # check in a few common install locations
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in `ls -d ${prefix}/lib 2>/dev/null` \
			`ls -d /usr/local/lib 2>/dev/null` ; do
		    if test -f "$i/tclConfig.sh" ; then
			ac_cv_c_tclconfig=`(cd $i; pwd)`
			break
		    fi
		done
	    fi

	    # check in a few other private locations
	    if test x"${ac_cv_c_tclconfig}" = x ; then
		for i in \
			${srcdir}/../tcl \
			`ls -dr ${srcdir}/../tcl[[8-9]].[[0-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tclConfig.sh" ; then
		    ac_cv_c_tclconfig=`(cd $i/unix; pwd)`
		    break
		    fi
		    if test -f "$i/win/tclConfig.sh" ; then
		    ac_cv_c_tclconfig=`(cd $i/win; pwd)`
		    break
		    fi
		done
	    fi
	])

	if test x"${ac_cv_c_tclconfig}" = x ; then
	    TCL_BIN_DIR="# no Tcl configs found"
	    AC_MSG_ERROR(Can't find Tcl configuration definitions)
	    exit 0
	else
	    no_tcl=
	    TCL_BIN_DIR=${ac_cv_c_tclconfig}
	    AC_MSG_RESULT(found $TCL_BIN_DIR/tclConfig.sh)
	fi
    fi
])

#------------------------------------------------------------------------
# SC_PATH_TKCONFIG --
#
#	Locate the tkConfig.sh file
#
# Arguments:
#	none
#
# Results:
#
#	Adds the following arguments to configure:
#		--with-tk=...
#
#	Defines the following vars:
#		TK_BIN_DIR	Full path to the directory containing
#				the tkConfig.sh file
#------------------------------------------------------------------------

AC_DEFUN(SC_PATH_TKCONFIG, [
    #
    # Ok, lets find the tk configuration
    # First, look for one uninstalled.
    # the alternative search directory is invoked by --with-tk
    #

    if test x"${no_tk}" = x ; then
	# we reset no_tk in case something fails here
	no_tk=true
	AC_ARG_WITH(tk, [  --with-tk               directory containing tk configuration (tkConfig.sh)], with_tkconfig=${withval})
	AC_MSG_CHECKING([for Tk configuration])
	AC_CACHE_VAL(ac_cv_c_tkconfig,[

	    # First check to see if --with-tkconfig was specified.
	    if test x"${with_tkconfig}" != x ; then
		if test -f "${with_tkconfig}/tkConfig.sh" ; then
		    ac_cv_c_tkconfig=`(cd ${with_tkconfig}; pwd)`
		else
		    AC_MSG_ERROR([${with_tkconfig} directory doesn't contain tkConfig.sh])
		fi
	    fi

	    # then check for a private Tk library
	    if test x"${ac_cv_c_tkconfig}" = x ; then
		for i in \
			../tk \
			`ls -dr ../tk[[8-9]].[[0-9]]* 2>/dev/null` \
			../../tk \
			`ls -dr ../../tk[[8-9]].[[0-9]]* 2>/dev/null` \
			../../../tk \
			`ls -dr ../../../tk[[8-9]].[[0-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tkConfig.sh" ; then
			ac_cv_c_tkconfig=`(cd $i/unix; pwd)`
			break
		    fi
		    if test -f "$i/win/tkConfig.sh" ; then
			ac_cv_c_tkconfig=`(cd $i/win; pwd)`
			break
		    fi
		done
	    fi
	    # check in a few common install locations
	    if test x"${ac_cv_c_tkconfig}" = x ; then
		for i in `ls -d ${prefix}/lib 2>/dev/null` \
			`ls -d /usr/local/lib 2>/dev/null` ; do
		    if test -f "$i/tkConfig.sh" ; then
			ac_cv_c_tkconfig=`(cd $i; pwd)`
			break
		    fi
		done
	    fi
	    # check in a few other private locations
	    if test x"${ac_cv_c_tkconfig}" = x ; then
		for i in \
			${srcdir}/../tk \
			`ls -dr ${srcdir}/../tk[[8-9]].[[0-9]]* 2>/dev/null` ; do
		    if test -f "$i/unix/tkConfig.sh" ; then
			ac_cv_c_tkconfig=`(cd $i/unix; pwd)`
			break
		    fi
		    if test -f "$i/win/tkConfig.sh" ; then
			ac_cv_c_tkconfig=`(cd $i/win; pwd)`
			break
		    fi
		done
	    fi
	])
	if test x"${ac_cv_c_tkconfig}" = x ; then
	    TK_BIN_DIR="# no Tk configs found"
	    AC_MSG_ERROR(Can't find Tk configuration definitions)
	    exit 0
	else
	    no_tk=
	    TK_BIN_DIR=${ac_cv_c_tkconfig}
	    AC_MSG_RESULT(found $TK_BIN_DIR/tkConfig.sh)
	fi
    fi

])

#------------------------------------------------------------------------
# SC_LOAD_TCLCONFIG --
#
#	Load the tclConfig.sh file
#
# Arguments:
#	
#	Requires the following vars to be set:
#		TCL_BIN_DIR
#
# Results:
#
#	Subst the following vars:
#		TCL_BIN_DIR
#		TCL_SRC_DIR
#		TCL_LIB_FILE
#
#------------------------------------------------------------------------

AC_DEFUN(SC_LOAD_TCLCONFIG, [
    AC_MSG_CHECKING([for existence of $TCL_BIN_DIR/tclConfig.sh])

    if test -f "$TCL_BIN_DIR/tclConfig.sh" ; then
        AC_MSG_RESULT([loading])
	. $TCL_BIN_DIR/tclConfig.sh
    else
        AC_MSG_RESULT([file not found])
    fi

    #
    # The eval is required to do the TCL_DBGX substitution in the
    # TCL_LIB_FILE variable
    #

    eval TCL_LIB_FILE=${TCL_LIB_FILE}
    eval TCL_LIB_FLAG=${TCL_LIB_FLAG}

    AC_SUBST(TCL_DBGX)
    AC_SUBST(TCL_BIN_DIR)
    AC_SUBST(TCL_SRC_DIR)
    AC_SUBST(TCL_LIB_FILE)
    AC_SUBST(TCL_LIBS)
    AC_SUBST(TCL_DEFS)
    AC_SUBST(TCL_SHLIB_LD_LIBS)
    AC_SUBST(TCL_EXTRA_CFLAGS)
    AC_SUBST(TCL_LD_FLAGS)
    AC_SUBST(TCL_LIB_FILE)
    AC_SUBST(TCL_STUB_LIB_FILE)
    AC_SUBST(TCL_LIB_SPEC)
    AC_SUBST(TCL_BUILD_LIB_SPEC)
    AC_SUBST(TCL_STUB_LIB_SPEC)
    AC_SUBST(TCL_BUILD_STUB_LIB_SPEC)
])

#------------------------------------------------------------------------
# SC_LOAD_TKCONFIG --
#
#	Load the tkConfig.sh file
#
# Arguments:
#	
#	Requires the following vars to be set:
#		TK_BIN_DIR
#
# Results:
#
#	Sets the following vars that should be in tkConfig.sh:
#		TK_BIN_DIR
#------------------------------------------------------------------------

AC_DEFUN(SC_LOAD_TKCONFIG, [
    AC_MSG_CHECKING([for existence of $TK_BIN_DIR/tkConfig.sh])

    if test -f "$TK_BIN_DIR/tkConfig.sh" ; then
        AC_MSG_RESULT([loading])
	. $TK_BIN_DIR/tkConfig.sh
    else
        AC_MSG_RESULT([could not find $TK_BIN_DIR/tkConfig.sh])
    fi

    AC_SUBST(TK_BIN_DIR)
    AC_SUBST(TK_SRC_DIR)
    AC_SUBST(TK_LIB_FILE)
    AC_SUBST(TK_XINCLUDES)
])

#------------------------------------------------------------------------
# SC_ENABLE_SHARED --
#
#	Allows the building of shared libraries
#
# Arguments:
#	none
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-shared=yes|no
#
#	Defines the following vars:
#		STATIC_BUILD	Used for building import/export libraries
#				on Windows.
#
#	Sets the following vars:
#		SHARED_BUILD	Value of 1 or 0
#------------------------------------------------------------------------

AC_DEFUN(SC_ENABLE_SHARED, [
    AC_MSG_CHECKING([how to build libraries])
    AC_ARG_ENABLE(shared,
	[  --enable-shared         build and link with shared libraries [--enable-shared]],
	[tcl_ok=$enableval], [tcl_ok=no])

# CYGNUS LOCAL
    case "${host}" in
        *mingw32* | *windows32*)
            # Default to shared build for Windows
            if test "${enable_shared+set}" != set; then
                tcl_ok=yes
            fi
        ;;
    esac
# END CYGNUS LOCAL

    if test "$tcl_ok" = "yes" ; then
	AC_MSG_RESULT([shared])
	SHARED_BUILD=1
    else
	AC_MSG_RESULT([static])
	SHARED_BUILD=0
	AC_DEFINE(STATIC_BUILD)
    fi
])

#------------------------------------------------------------------------
# SC_ENABLE_THREADS --
#
#	Specify if thread support should be enabled
#
# Arguments:
#	none
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-threads
#
#	Sets the following vars:
#		THREADS_LIBS	Thread library(s)
#
#	Defines the following vars:
#		TCL_THREADS
#		_REENTRANT
#
#------------------------------------------------------------------------

AC_DEFUN(SC_ENABLE_THREADS, [
    AC_MSG_CHECKING(for building with threads)
    AC_ARG_ENABLE(threads, [  --enable-threads        build with threads],
	[tcl_ok=$enableval], [tcl_ok=no])

    if test "$tcl_ok" = "yes"; then
	TCL_THREADS=1
	AC_DEFINE(TCL_THREADS)
	AC_DEFINE(_REENTRANT)

	case "${host}" in
	    *mingw32* | *windows32*)
		    AC_MSG_RESULT(yes)
		;;
	    *)
		AC_CHECK_LIB(pthread,pthread_mutex_init,tcl_ok=yes,tcl_ok=no)
		if test "$tcl_ok" = "yes"; then
		    # The space is needed
		    THREADS_LIBS=" -lpthread"
		    AC_MSG_RESULT(yes)
		else
		    TCL_THREADS=0
		    AC_MSG_RESULT(no)
		    AC_MSG_WARN("Don t know how to find pthread lib on your system - you must disable thread support or edit the LIBS in the Makefile...")
		fi
		;;
	esac
    else
	TCL_THREADS=0
	AC_MSG_RESULT(no (default))
    fi

])

#------------------------------------------------------------------------
# SC_ENABLE_SYMBOLS --
#
#	Specify if debugging symbols should be used
#
# Arguments:
#	none
#	
#	Requires the following vars to be set:
#		CFLAGS_DEBUG
#		CFLAGS_OPTIMIZE
#		LDFLAGS_DEBUG
#		LDFLAGS_OPTIMIZE
#	
# Results:
#
#	Adds the following arguments to configure:
#		--enable-symbols
#
#	Defines the following vars:
#		CFLAGS_DEFAULT	Sets to CFLAGS_DEBUG if true
#				Sets to CFLAGS_OPTIMIZE if false
#		LDFLAGS_DEFAULT	Sets to LDFLAGS_DEBUG if true
#				Sets to LDFLAGS_OPTIMIZE if false
#		DBGX		Debug library extension
#
#------------------------------------------------------------------------

AC_DEFUN(SC_ENABLE_SYMBOLS, [
    case "${host}" in
	*mingw32* | *windows32*)
	    tcl_dbgx=d
	;;
	*)
	    tcl_dbgx=g
	;;
    esac

    AC_MSG_CHECKING([for build with symbols])
    AC_ARG_ENABLE(symbols, [  --enable-symbols        build with debugging symbols [--disable-symbols]],    [tcl_ok=$enableval], [tcl_ok=no])
    if test "$tcl_ok" = "yes"; then
	CFLAGS_DEFAULT="${CFLAGS_DEBUG}"
	LDFLAGS_DEFAULT="${LDFLAGS_DEBUG}"
	DBGX=${tcl_dbgx}
	TCL_DBGX=${tcl_dbgx}
	AC_MSG_RESULT([yes])
    else
	CFLAGS_DEFAULT="${CFLAGS_OPTIMIZE}"
	LDFLAGS_DEFAULT="${LDFLAGS_OPTIMIZE}"
	DBGX=""
	TCL_DBGX=""
	AC_MSG_RESULT([no])
    fi

    AC_SUBST(TCL_DBGX)
    AC_SUBST(CFLAGS_DEFAULT)
    AC_SUBST(LDFLAGS_DEFAULT)
])

#------------------------------------------------------------------------
# SC_MAKE_LIB --
#
#	Generate a line that can be used to build a shared/unshared library
#	in a platform independent manner.
#
# Arguments:
#	none
#
#	Requires:
#
# Results:
#
#	Defines the following vars:
#		MAKE_LIB	Makefile rule for building a library
#		MAKE_SHARED_LIB	Makefile rule for building a shared library
#		MAKE_UNSHARED_LIB	Makefile rule for building a static
#				library
#------------------------------------------------------------------------

AC_DEFUN(SC_MAKE_LIB, [
    case "${host}" in
	*mingw32* | *windows32*)
	    if test "${CC}" = "cl"; then
		MAKE_STATIC_LIB="\${STLIB_LD} -out:\[$]@ \$(\[$]@_OBJECTS) "
		MAKE_SHARED_LIB="\${SHLIB_LD} \${SHLIB_LDFLAGS} \${SHLIB_LD_LIBS} \$(LDFLAGS) -out:\[$]@ \$(\[$]@_OBJECTS) "
	    else
		MAKE_STATIC_LIB="\${STLIB_LD} \[$]@ \$(\[$]@_OBJECTS) "
		POST_MAKE_STATIC_LIB="\${RANLIB} \[$]@"
		MAKE_SHARED_LIB="\${SHLIB_LD} -o \[$]@ \$(\[$]@_OBJECTS) \${SHLIB_LDFLAGS} \$(LDFLAGS) \${SHLIB_LD_LIBS} -Wl,--out-implib,\$(patsubst %.dll,lib%.a,\[$]@)"
	    fi
	    ;;
	*)
	    MAKE_STATIC_LIB="\${STLIB_LD} \[$]@ \$(\[$]@_OBJECTS)"
	    POST_MAKE_STATIC_LIB="\${RANLIB} \[$]@"
	    MAKE_SHARED_LIB="\${SHLIB_LD} -o \[$]@ \$(\[$]@_OBJECTS) \${SHLIB_LDFLAGS} \${SHLIB_LD_LIBS}"
	    ;;
    esac

    if test "${SHARED_BUILD}" = "1" ; then
	MAKE_LIB=${MAKE_SHARED_LIB}
    else
	MAKE_LIB=${MAKE_STATIC_LIB}
	POST_MAKE_LIB=${POST_MAKE_STATIC_LIB}
    fi

    AC_SUBST(MAKE_LIB)
    AC_SUBST(MAKE_SHARED_LIB)
    AC_SUBST(MAKE_STATIC_LIB)
    AC_SUBST(POST_MAKE_LIB)
    AC_SUBST(POST_MAKE_STATIC_LIB)
])

#------------------------------------------------------------------------
# SC_PRIVATE_TCL_HEADERS --
#
#	Locate the private Tcl include files
#
# Arguments:
#
#	Requires:
#		TCL_SRC_DIR	Assumes that SC_LOAD_TCLCONFIG has
#				 already been called.
#
# Results:
#
#	Substs the following vars:
#		TCL_TOP_DIR_NATIVE
#		TCL_GENERIC_DIR_NATIVE
#		TCL_UNIX_DIR_NATIVE
#		TCL_WIN_DIR_NATIVE
#		TCL_BMAP_DIR_NATIVE
#		TCL_TOOL_DIR_NATIVE
#		TCL_PLATFORM_DIR_NATIVE
#		TCL_BIN_DIR_NATIVE
#		TCL_INCLUDES
#------------------------------------------------------------------------

AC_DEFUN(SC_PRIVATE_TCL_HEADERS, [
    AC_MSG_CHECKING(for Tcl private include files)

    case "${host}" in
	*mingw32* | *windows32*)
	    TCL_TOP_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}`\"
	    TCL_GENERIC_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/generic`\"
	    TCL_UNIX_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/unix`\"
	    TCL_WIN_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/win`\"
	    TCL_BMAP_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/bitmaps`\"
	    TCL_TOOL_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/tools`\"
	    TCL_COMPAT_DIR_NATIVE=\"`${CYGPATH} ${TCL_SRC_DIR}/compat`\"
	    TCL_PLATFORM_DIR_NATIVE=${TCL_WIN_DIR_NATIVE}
	;;
	*)
	    TCL_TOP_DIR_NATIVE='$(TCL_SRC_DIR)'
	    TCL_GENERIC_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/generic'
	    TCL_UNIX_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/unix'
	    TCL_WIN_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/win'
	    TCL_BMAP_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/bitmaps'
	    TCL_TOOL_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/tools'
	    TCL_COMPAT_DIR_NATIVE='$(TCL_TOP_DIR_NATIVE)/compat'
	    TCL_PLATFORM_DIR_NATIVE=${TCL_UNIX_DIR_NATIVE}
	;;
    esac

    AC_SUBST(TCL_TOP_DIR_NATIVE)
    AC_SUBST(TCL_GENERIC_DIR_NATIVE)
    AC_SUBST(TCL_UNIX_DIR_NATIVE)
    AC_SUBST(TCL_WIN_DIR_NATIVE)
    AC_SUBST(TCL_BMAP_DIR_NATIVE)
    AC_SUBST(TCL_TOOL_DIR_NATIVE)
    AC_SUBST(TCL_PLATFORM_DIR_NATIVE)

    TCL_INCLUDES="-I${TCL_GENERIC_DIR_NATIVE} -I${TCL_PLATFORM_DIR_NATIVE}"
    AC_SUBST(TCL_INCLUDES)
    AC_MSG_RESULT(Using srcdir found in tclConfig.sh: ${TCL_SRC_DIR})
])

#------------------------------------------------------------------------
# SC_PRIVATE_TK_HEADERS --
#
#	Locate the private Tk include files
#
# Arguments:
#
#	Requires:
#		TK_SRC_DIR	Assumes that SC_LOAD_TKCONFIG has
#				 already been called.
#
# Results:
#
#	Substs the following vars:
#		TK_INCLUDES
#------------------------------------------------------------------------

AC_DEFUN(SC_PRIVATE_TK_HEADERS, [
    AC_MSG_CHECKING(for Tk private include files)

    case "${host}" in
	*mingw32* | *windows32*)
	    TK_UNIX_DIR_NATIVE=\"`${CYGPATH} ${TK_SRC_DIR}/unix`\"
	    TK_WIN_DIR_NATIVE=\"`${CYGPATH} ${TK_SRC_DIR}/win`\"
	    TK_GENERIC_DIR_NATIVE=\"`${CYGPATH} ${TK_SRC_DIR}/generic`\"
	    TK_XLIB_DIR_NATIVE=\"`${CYGPATH} ${TK_SRC_DIR}/xlib`\"
	    TK_PLATFORM_DIR_NATIVE=${TK_WIN_DIR_NATIVE}

	    TK_INCLUDES="-I${TK_GENERIC_DIR_NATIVE} -I${TK_PLATFORM_DIR_NATIVE} -I${TK_XLIB_DIR_NATIVE}"
	;;
	*)
	    TK_GENERIC_DIR_NATIVE='$(TK_TOP_DIR_NATIVE)/generic'
	    TK_UNIX_DIR_NATIVE='$(TK_TOP_DIR_NATIVE)/unix'
	    TK_WIN_DIR_NATIVE='$(TK_TOP_DIR_NATIVE)/win'
	    TK_PLATFORM_DIR_NATIVE=${TK_UNIX_DIR_NATIVE}

	    TK_INCLUDES="-I${TK_GENERIC_DIR_NATIVE} -I${TK_PLATFORM_DIR_NATIVE}"
	;;
    esac

    AC_SUBST(TK_UNIX_DIR_NATIVE)
    AC_SUBST(TK_WIN_DIR_NATIVE)
    AC_SUBST(TK_GENERIC_DIR_NATIVE)
    AC_SUBST(TK_XLIB_DIR_NATIVE)
    AC_SUBST(TK_PLATFORM_DIR_NATIVE)

    AC_SUBST(TK_INCLUDES)
    AC_MSG_RESULT(Using srcdir found in tkConfig.sh)
])
