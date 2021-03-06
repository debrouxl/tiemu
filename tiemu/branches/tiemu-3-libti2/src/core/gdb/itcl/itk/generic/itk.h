/*
 * ------------------------------------------------------------------------
 *      PACKAGE:  [incr Tk]
 *  DESCRIPTION:  Building mega-widgets with [incr Tcl]
 *
 *  [incr Tk] provides a framework for building composite "mega-widgets"
 *  using [incr Tcl] classes.  It defines a set of base classes that are
 *  specialized to create all other widgets.
 *
 *  ADDING [incr Tk] TO A Tcl-BASED APPLICATION:
 *
 *    To add [incr Tk] facilities to a Tcl application, modify the
 *    Tcl_AppInit() routine as follows:
 *
 *    1) Include the header files for [incr Tcl] and [incr Tk] near
 *       the top of the file containing Tcl_AppInit():
 *
 *         #include "itcl.h"
 *         #include "itk.h"
 *
 *    2) Within the body of Tcl_AppInit(), add the following lines:
 *
 *         if (Itcl_Init(interp) == TCL_ERROR) {
 *             return TCL_ERROR;
 *         }
 *         if (Itk_Init(interp) == TCL_ERROR) {
 *             return TCL_ERROR;
 *         }
 *
 *    3) Link your application with libitcl.a and libitk.a
 *
 *    NOTE:  An example file "tkAppInit.c" containing the changes shown
 *           above is included in this distribution.
 *
 * ========================================================================
 *  AUTHOR:  Michael J. McLennan
 *           Bell Labs Innovations for Lucent Technologies
 *           mmclennan@lucent.com
 *           http://www.tcltk.com/itcl
 *
 *     RCS:  $Id: itk.h,v 1.9 2001/05/25 00:15:04 davygrvy Exp $
 * ========================================================================
 *           Copyright (c) 1993-1998  Lucent Technologies, Inc.
 * ------------------------------------------------------------------------
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */
#ifndef ITK_H
#define ITK_H

/*
 * A special definition used to allow this header file to be included
 * in resource files so that they can get obtain version information from
 * this file.  Resource compilers don't like all the C stuff, like typedefs
 * and procedure declarations, that occur below.
 */

#ifndef RC_INVOKED

#include "itclInt.h"
#include "tk.h"

# undef TCL_STORAGE_CLASS
#ifdef BUILD_itk
# define TCL_STORAGE_CLASS DLLEXPORT
#else
# define TCL_STORAGE_CLASS
#endif

/*
 *  List of options in alphabetical order:
 */
typedef struct ItkOptList {
    Tcl_HashTable *options;     /* list containing the real options */
    Tcl_HashEntry **list;       /* gives ordering of options */
    int len;                    /* number of entries in order list */
    int max;                    /* maximum size of order list */
} ItkOptList;

/*
 *  List of options created in the class definition:
 */
typedef struct ItkClassOptTable {
    Tcl_HashTable options;        /* option storage with fast lookup */
    ItkOptList order;             /* gives ordering of options */
} ItkClassOptTable;

/*
 *  Each option created in the class definition:
 */
typedef struct ItkClassOption {
    ItclMember *member;           /* info about this option */
    char *resName;                /* resource name in X11 database */
    char *resClass;               /* resource class name in X11 database */
    char *init;                   /* initial value for option */
} ItkClassOption;

#include "itkDecls.h"

/*
 *  This function is contained in the itkstub static library
 */

#ifdef USE_ITK_STUBS

CONST char *		Itk_InitStubs _ANSI_ARGS_((Tcl_Interp *interp,
			    char *version, int exact));
#endif

/*
 * Public functions that are not accessible via the stubs table.
 */

# undef TCL_STORAGE_CLASS
# define TCL_STORAGE_CLASS DLLIMPORT

#endif /* RC_INVOKED */
#endif /* ITK_H */
