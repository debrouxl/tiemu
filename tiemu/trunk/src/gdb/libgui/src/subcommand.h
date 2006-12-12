/* subcommand.h - Handle commands with subcommands.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Tom Tromey <tromey@cygnus.com>.  */

#ifndef SUBCOMMAND_H
#define SUBCOMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

struct ide_subcommand_table
{
  const char *method;		/* Method name.  If NULL, then this is
				   the last entry in the table.  */
  Tcl_CmdProc *func;		/* The implementation.  */
  int min_args;			/* Minimum number of args.  */
  int max_args;			/* Maximum number of args.  -1 means
				   no maximum.  */
};

/* Define a command with subcommands.  */
int ide_create_command_with_subcommands
  (Tcl_Interp *interp, char *name, const struct ide_subcommand_table *table,
   ClientData, Tcl_CmdDeleteProc *);

#ifdef __cplusplus
}
#endif

#endif /* SUBCOMMAND_H */
