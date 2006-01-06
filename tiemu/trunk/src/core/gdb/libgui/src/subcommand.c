/* subcommand.c - Automate handling of subcommands in Tcl.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Tom Tromey <tromey@cygnus.com>.  */

#include <string.h>

#include <tcl.h>

#include "subcommand.h"

/* A pointer to this structure is the clientdata for
   subcommand_implementation.  */

struct subcommand_clientdata
{
  const struct ide_subcommand_table *commands;
  ClientData subdata;
  Tcl_CmdDeleteProc *delete;
};

/* This is called when one of our commands is deleted.  */
static void
subcommand_deleted (ClientData cd)
{
  struct subcommand_clientdata *data = (struct subcommand_clientdata *) cd;

  if (data->delete)
    (*data->delete) (data->subdata);
  ckfree ((char *) data);
}

/* This function implements any Tcl command registered as having
   subcommands.  The ClientData here must be a pointer to the command
   table.  */
static int
subcommand_implementation (ClientData cd, Tcl_Interp *interp,
			   int argc, char *argv[])
{
  struct subcommand_clientdata *data = (struct subcommand_clientdata *) cd;
  const struct ide_subcommand_table *commands = data->commands;
  int i;

  if (argc < 2)
    {
      Tcl_AppendResult (interp, "wrong # args: must be \"",
			argv[0], " key ?arg ...?\"", (char *) NULL);
      return (TCL_ERROR);
    }

  for (i = 0; commands[i].method != NULL; ++i)
    {
      if (! strcmp (argv[1], commands[i].method))
	{
	  if (argc < commands[i].min_args)
	    {
	      char buf[20];
	      Tcl_AppendResult (interp, "wrong # args: got ", (char *) NULL);
	      sprintf (buf, "%d", argc);
	      Tcl_AppendResult (interp, buf, " but expected at least ",
				(char *) NULL);
	      sprintf (buf, "%d", commands[i].min_args);
	      Tcl_AppendResult (interp, buf, (char *) NULL);
	      return (TCL_ERROR);
	    }

	  if (commands[i].max_args > 0 && argc > commands[i].max_args)
	    {
	      char buf[20];
	      Tcl_AppendResult (interp, "wrong # args: got ", (char *) NULL);
	      sprintf (buf, "%d", argc);
	      Tcl_AppendResult (interp, buf, " but expected at most ",
				(char *) NULL);
	      sprintf (buf, "%d", commands[i].max_args);
	      Tcl_AppendResult (interp, buf, (char *) NULL);
	      return (TCL_ERROR);
	    }

	  return (commands[i].func (data->subdata, interp, argc, argv));
	}
    }

  Tcl_AppendResult (interp, "unrecognized key \"", argv[1],
		    "\"; must be one of ", (char *) NULL);
  for (i = 0; commands[i].method != NULL; ++i)
    Tcl_AppendResult (interp, "\"", commands[i].method,
		      (commands[i + 1].method == NULL) ? "\"" : "\", ",
		      (char *) NULL);
  return (TCL_ERROR);
}

/* Define a command with subcommands.  */
int
ide_create_command_with_subcommands (Tcl_Interp *interp, char *name,
				     const struct ide_subcommand_table *table,
				     ClientData subdata,
				     Tcl_CmdDeleteProc *delete)
{
  int i;
  struct subcommand_clientdata *data;

  /* Sanity check.  */
  for (i = 0; table[i].method != NULL; ++i)
    {
      if ((table[i].min_args > table[i].max_args && table[i].max_args != -1)
	  || table[i].min_args < 2
	  || table[i].max_args < -1)
	{
	  Tcl_AppendResult (interp, "subcommand \"", table[i].method,
			    "\" of command \"", name,
			    "\" has bad argument count",
			    (char *) NULL);
	  return (TCL_ERROR);
	}
    }

  data = (struct subcommand_clientdata *) ckalloc (sizeof *data);
  data->commands = table;
  data->subdata = subdata;
  data->delete = delete;

  if (Tcl_CreateCommand (interp, name, subcommand_implementation,
			 (ClientData) data, subcommand_deleted) == NULL)
    return (TCL_ERROR);

  return (TCL_OK);
}
