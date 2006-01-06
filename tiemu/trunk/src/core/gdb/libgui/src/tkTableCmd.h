/* 
 * tkTableCmd.h --
 *
 *	This is the header file for the module that implements
 *	command structure lookups.
 *
 * Copyright (c) 1997,1998 Jeffrey Hobbs
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#ifndef _CMD_H_
#define _CMD_H_

#include <string.h>
#include <stdlib.h>
#include <tk.h>

/* structure for use in parsing table commands/values */
typedef struct {
  char *name;		/* name of the command/value */
  int value;		/* >0 because 0 represents an error */
} Cmd_Struct;

extern char *	Cmd_GetName _ANSI_ARGS_((const Cmd_Struct *cmds, int val));
extern int	Cmd_GetValue _ANSI_ARGS_((const Cmd_Struct *cmds,
					  const char *arg));
extern void	Cmd_GetError _ANSI_ARGS_((Tcl_Interp *interp,
					  const Cmd_Struct *cmds,
					  const char *arg));
extern int	Cmd_Parse _ANSI_ARGS_((Tcl_Interp *interp, Cmd_Struct *cmds,
				       const char *arg));
extern int	Cmd_OptionSet _ANSI_ARGS_((ClientData clientData,
					   Tcl_Interp *interp,
					   Tk_Window unused, char *value,
					   char *widgRec, int offset));
extern char *	Cmd_OptionGet _ANSI_ARGS_((ClientData clientData,
					   Tk_Window unused, char *widgRec,
					   int offset,
					   Tcl_FreeProc **freeProcPtr));
extern int	Cmd_BitSet _ANSI_ARGS_((ClientData clientData,
					Tcl_Interp *interp,
					Tk_Window unused, char *value,
					char *widgRec, int offset));
extern char *	Cmd_BitGet _ANSI_ARGS_((ClientData clientData,
					Tk_Window unused, char *widgRec,
					int offset,
					Tcl_FreeProc **freeProcPtr));

#endif /* _CMD_H_ */
