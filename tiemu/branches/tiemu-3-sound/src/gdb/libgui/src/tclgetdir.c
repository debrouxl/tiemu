/* tclgetdir.c -- TCL code to browse for a directory.
   Copyright (C) 1997, 1998 Cygnus Solutions.
   Written by Ian Lance Taylor <ian@cygnus.com>.  */

#ifdef _WIN32
#include <windows.h>
#ifndef _GNU_H_WINDOWS_H   /* if not using old Cygwin Win32 headers */
#include <shlobj.h>
#endif
#endif

#include <tcl.h>
#include <tk.h>

#include "guitcl.h"

/* This file defines one TCL command.

   ide_get_directory
       Allows the user to select a directory.  Returns the selected
       directory as a string.  */

#ifdef _WIN32

#include <tkWinInt.h>
/* a call back to set the initial selected directory */

/* defines currently missing from Cygwin32 */
#ifndef BFFM_INITIALIZED


LPITEMIDLIST WINAPI SHBrowseForFolderA(LPBROWSEINFO lpbi);

/* message from browser */
#define BFFM_INITIALIZED        1
#define BFFM_SELCHANGED         2

/* messages to browser */
#define BFFM_SETSTATUSTEXTA     (WM_USER + 100)
#define BFFM_ENABLEOK           (WM_USER + 101)
#define BFFM_SETSELECTIONA      (WM_USER + 102)
#define BFFM_SETSELECTIONW      (WM_USER + 103)
#define BFFM_SETSTATUSTEXTW     (WM_USER + 104)

#ifdef UNICODE
#define SHBrowseForFolder   SHBrowseForFolderW
#define BFFM_SETSTATUSTEXT  BFFM_SETSTATUSTEXTW
#define BFFM_SETSELECTION   BFFM_SETSELECTIONW
#else
#define SHBrowseForFolder   SHBrowseForFolderA
#define BFFM_SETSTATUSTEXT  BFFM_SETSTATUSTEXTA
#define BFFM_SETSELECTION   BFFM_SETSELECTIONA
#endif

#endif /* ! BFFM_INITIALIZED */

/* FIXME: We need to dig into the Tk window implementation internals.  */

int CALLBACK MyBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg==BFFM_INITIALIZED)
    {
       SendMessage(hwnd,BFFM_SETSELECTION,(WPARAM)TRUE,(LPARAM)lpData);
    }
  return 0;
}

/* Implement the Windows version of the ide_get_directory command.  */
static int
get_directory_command (ClientData cd, Tcl_Interp *interp, int argc,
		       char **argv)
{
  BROWSEINFO bi;
  char buf[MAX_PATH + 1];
  Tk_Window parent;
  int i, oldMode;
  LPITEMIDLIST idlist;
  char *p;
  int atts;
  Tcl_DString tempBuffPtr;
#if (TCL_MAJOR_VERSION >= 8) && (TCL_MINOR_VERSION >= 1)
  Tcl_DString titleDString;
  Tcl_DString initialDirDString;
  Tcl_DString resultDString;

  Tcl_DStringInit(&titleDString);
  Tcl_DStringInit(&initialDirDString);
#endif

  Tcl_DStringInit(&tempBuffPtr);

  bi.hwndOwner = NULL;
  bi.pidlRoot = NULL;
  bi.pszDisplayName = buf;
  bi.lpszTitle = NULL;
  bi.ulFlags = 0;
  bi.lpfn = NULL;
  bi.lParam = 0;
  bi.iImage = 0;

  parent = Tk_MainWindow (interp);

  for (i = 1; i < argc; i += 2)
    {
      int v;
      int len;

      v = i + 1;
      len = strlen (argv[i]);

      if (strncmp (argv[i], "-parent", len) == 0)
	{
	  if (v == argc)
	    goto arg_missing;

	  parent = Tk_NameToWindow (interp, argv[v],
				    Tk_MainWindow (interp));
	  if (parent == NULL)
	    return TCL_ERROR;
	}
      else if (strncmp (argv[i], "-title", len) == 0)
	{

	  if (v == argc)
	    goto arg_missing;

#if (TCL_MAJOR_VERSION >= 8) && (TCL_MINOR_VERSION >= 1)
	  Tcl_UtfToExternalDString(NULL, argv[v], -1, &titleDString);
	  bi.lpszTitle = Tcl_DStringValue(&titleDString);
#else
	  bi.lpszTitle = argv[v];
#endif
	}
      else if (strncmp (argv[i], "-initialdir", len) == 0)
	{
	  if (v == argc)
	    goto arg_missing;

	  /* bi.lParam will be passed to the callback function.(save the need for globals)*/
	  bi.lParam = (LPARAM) Tcl_TranslateFileName(interp, argv[v], &tempBuffPtr);
#if (TCL_MAJOR_VERSION >= 8) && (TCL_MINOR_VERSION >= 1)
	  Tcl_UtfToExternalDString(NULL, (char *) bi.lParam, -1, &initialDirDString);
	  bi.lParam = (LPARAM) Tcl_DStringValue(&initialDirDString);
#endif
	  bi.lpfn   = MyBrowseCallbackProc;
	}
      else
	{
	  Tcl_AppendResult (interp, "unknown option \"", argv[i],
			    "\", must be -parent or -title", (char *) NULL);
	  return TCL_ERROR;
	}
    }

  if (Tk_WindowId (parent) == None)
    Tk_MakeWindowExist (parent);

  bi.hwndOwner = Tk_GetHWND (Tk_WindowId (parent));

  oldMode = Tcl_SetServiceMode(TCL_SERVICE_ALL);
  idlist = SHBrowseForFolder (&bi);
  Tcl_SetServiceMode(oldMode);

  if (idlist == NULL)
    {
      /* User pressed the cancel button.  */
      return TCL_OK;
    }

  if (! SHGetPathFromIDList (idlist, buf))
    {
      Tcl_SetResult (interp, "could not get path for directory", TCL_STATIC);
      return TCL_ERROR;
    }

  /* Ensure the directory exists.  */
  atts = GetFileAttributesA (buf);
  if (atts == -1 || ! (atts & FILE_ATTRIBUTE_DIRECTORY))
    {
      Tcl_AppendResult (interp, "path \"", buf, "\" is not a directory",
			(char *) NULL);
      /* FIXME: free IDLIST.  */
      return TCL_ERROR;
    }

  /* FIXME: We are supposed to free IDLIST using the shell task
     allocator, but cygwin32 doesn't define the required interfaces
     yet.  */

  

  /* Normalize the path for Tcl.  */
#if (TCL_MAJOR_VERSION >= 8) && (TCL_MINOR_VERSION >= 1)
  Tcl_ExternalToUtfDString(NULL, buf, -1, &resultDString);
  p = Tcl_DStringValue(&resultDString);
#else
  p = buf;
#endif
  for (; *p != '\0'; ++p)
    if (*p == '\\')
      *p = '/';

  Tcl_ResetResult(interp);
#if (TCL_MAJOR_VERSION >= 8) && (TCL_MINOR_VERSION >= 1)
  Tcl_SetResult(interp, Tcl_DStringValue(&resultDString), TCL_VOLATILE);
  Tcl_DStringFree(&resultDString);
  Tcl_DStringFree(&titleDString);
  Tcl_DStringFree(&initialDirDString);
#else
  Tcl_SetResult(interp, buf, TCL_VOLATILE);
#endif
  Tcl_DStringFree(&tempBuffPtr);

  return TCL_OK;

 arg_missing:
  Tcl_AppendResult(interp, "value for \"", argv[argc - 1], "\" missing",
		   NULL);
  return TCL_ERROR;
}


#else /* ! _WIN32 */

/* Use our modified file dialog, and hope the user picks a directory.  */

static int
get_directory_command (ClientData cd, Tcl_Interp *interp, int argc,
		       char **argv)
{
  char **new_args;
  char *merge;
  int result, i;

  /* We can't directly run Tk_GetOpenFile, because it wants some
     ClientData that we're best off not knowing.  So instead we
     re-eval.  This is a lot less efficient, but it doesn't really
     matter.  */

  new_args = (char **) ckalloc ((argc + 2) * sizeof (char *));

  new_args[0] = "tk_getOpenFile";
  new_args[1] = "-choosedir";
  new_args[2] = "1";

  for (i = 1; i < argc; ++i)
    new_args[2 + i] = argv[i];

  merge = Tcl_Merge (argc + 2, new_args);
  result = Tcl_GlobalEval (interp, merge);

  ckfree (merge);
  ckfree ((char *) new_args);

  return result;
}

#endif /* ! _WIN32 */

/* This function creates the ide_get_directory TCL command.  */

int
ide_create_get_directory_command (Tcl_Interp *interp)
{
  if (Tcl_CreateCommand (interp, "ide_get_directory", get_directory_command,
			 NULL, NULL) == NULL)
    return TCL_ERROR;
  return TCL_OK;
}
