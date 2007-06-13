/* 
 * tkTableCmd.c --
 *
 *	This module implements command structure lookups.
 *
 * Copyright (c) 1997,1998 Jeffrey Hobbs
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#include "tkTableCmd.h"

/*
 * Functions for handling custom options that use Cmd_Structs
 */

int
Cmd_OptionSet(ClientData clientData, Tcl_Interp *interp,
	      Tk_Window unused, char *value, char *widgRec, int offset)
{
  Cmd_Struct *p = (Cmd_Struct *)clientData;
  int mode = Cmd_GetValue(p,value);
  if (!mode) {
    Cmd_GetError(interp,p,value);
    return TCL_ERROR;
  }
  *((int*)(widgRec+offset)) = mode;
  return TCL_OK;
}

char *
Cmd_OptionGet(ClientData clientData, Tk_Window unused,
	      char *widgRec, int offset, Tcl_FreeProc **freeProcPtr)
{
  Cmd_Struct *p = (Cmd_Struct *)clientData;
  int mode = *((int*)(widgRec+offset));
  return Cmd_GetName(p,mode);
}

/*
 * Options for bits in an int
 * This will set/clear one bit in an int, the specific bit
 * being passed in clientData
 */
int
Cmd_BitSet(ClientData clientData, Tcl_Interp *interp,
	   Tk_Window unused, char *value, char *widgRec, int offset)
{
  int mode;
  if (Tcl_GetBoolean(interp, value, &mode) != TCL_OK) {
    return TCL_ERROR;
  }
  if (mode) {
    *((int*)(widgRec+offset)) |= (int)clientData;
  } else {
    *((int*)(widgRec+offset)) &= ~((int)clientData);
  }
  return TCL_OK;
}

char *
Cmd_BitGet(ClientData clientData, Tk_Window unused,
	   char *widgRec, int offset, Tcl_FreeProc **freeProcPtr)
{
  return (*((int*)(widgRec+offset)) & (int) clientData)?"1":"0";
}

/*
 * simple Cmd_Struct lookup functions
 */

char *
Cmd_GetName(const Cmd_Struct *cmds, int val)
{
  for(;cmds->name && cmds->name[0];cmds++) {
    if (cmds->value==val) return cmds->name;
  }
  return NULL;
}

int
Cmd_GetValue(const Cmd_Struct *cmds, const char *arg)
{
  int len=strlen(arg);
  for(;cmds->name && cmds->name[0];cmds++) {
    if (!strncmp(cmds->name,arg,len)) return cmds->value;
  }
  return 0;
}

void
Cmd_GetError(Tcl_Interp *interp, const Cmd_Struct *cmds, const char *arg)
{
  int i;
  Tcl_AppendResult(interp, "bad option \"", arg, "\" must be ", (char *) 0);
  for(i=0;cmds->name && cmds->name[0];cmds++,i++) {
    Tcl_AppendResult(interp, (i?", ":""), cmds->name, (char *) 0);
  }
}

/*
 * Parses a command string passed in an arg comparing it with all the
 * command strings in the command array. If it finds a string which is a
 * unique identifier of one of the commands, returns the index . If none of
 * the commands match, or the abbreviation is not unique, then it sets up
 * the message accordingly and returns 0
 */

int
Cmd_Parse (Tcl_Interp *interp, Cmd_Struct *cmds, const char *arg)
{
  int len = (int)strlen(arg);
  Cmd_Struct *matched = (Cmd_Struct *) 0;
  int err = 0;
  Cmd_Struct *next = cmds;
  while (*(next->name)) {
    if (strncmp (next->name, arg, len) == 0) {
      /* have we already matched this one if so make up an error message */
      if (matched) {
	if (!err) {
	  Tcl_AppendResult(interp, "ambiguous option \"", arg,
			   "\" could be ", matched->name, (char *) 0);
	  matched = next;
	  err = 1;
	}
	Tcl_AppendResult(interp, ", ", next->name, (char *) 0);
      } else {
	matched = next;
	/* return on an exact match */
	if (len == (int)strlen(next->name))
	  return matched->value;
      }
    }
    next++;
  }
  /* did we get multiple possibilities */
  if (err) return 0;
  /* did we match any at all */
  if (matched) {
    return matched->value;
  } else {
    Tcl_AppendResult(interp, "bad option \"", arg, "\" must be ",
		     (char *) NULL);
    next = cmds;
    while (1) {
      Tcl_AppendResult(interp, next->name, (char *) NULL);
      /* the end of them all ? */
      if (!*((++next)->name)) return 0;
      /* or the last one at least */
      if (*((next + 1)->name))
	Tcl_AppendResult(interp, ", ", (char *) NULL);
      else
	Tcl_AppendResult(interp, " or ", (char *) NULL);
    }
  }
}
