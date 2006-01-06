/* tclmapi.c - Tcl interface to MAPI.
   Copyright (C) 1997 Cygnus Solutions
   Written by Tom Tromey <tromey@cygnus.com>.  */

#ifdef _WIN32

#include <windows.h>
#include <mapi.h>

#include <stdio.h>
#include <tcl.h>

#include "guitcl.h"
#include "subcommand.h"

/* Usage for the mapi command:
   mapi simple-send TO-ADDRESS SUBJECT TEXT.

   This command has been deliberately kept very simple; it only does
   what we need.  However it can be extended by adding new subcommands
   if necessary.  */

static int
mapi_command (ClientData cd, Tcl_Interp *interp, int argc, char *argv[])
{
  MapiMessage message;
  MapiRecipDesc to;
  ULONG result;

  message.ulReserved = 0;
  message.lpszSubject = argv[3];
  message.lpszNoteText = argv[4];
  message.lpszMessageType = NULL;
  message.lpszDateReceived = NULL;
  message.lpszConversationID = NULL;
  message.flFlags = 0;
  message.lpOriginator = NULL;
  message.nRecipCount = 1;
  message.lpRecips = &to;
  message.nFileCount = 0;
  message.lpFiles = NULL;

  to.ulReserved = 0;
  to.ulRecipClass = MAPI_TO;
  to.lpszName = "";
  /* FIXME: smtp:address?  */
  to.lpszAddress = argv[2];
  to.ulEIDSize = 0;
  to.lpEntryID = NULL;

  result = MAPISendMail (0, 0, &message, MAPI_LOGON_UI, 0);
  if (result != SUCCESS_SUCCESS)
    {
      /* We could decode the error here.  */
      char buf[20];

      sprintf (buf, "0x%lx", result);
      Tcl_AppendResult (interp, argv[0], ": failed with status ",
			buf, (char *) NULL);
      return TCL_ERROR;
    }

  return TCL_OK;
}

static const struct ide_subcommand_table mapi_table[] =
{
  { "simple-send", mapi_command, 5, 5 },
  { NULL, NULL, 0, 0 }
};

int
ide_create_mapi_command (Tcl_Interp *interp)
{
  return ide_create_command_with_subcommands (interp, "ide_mapi",
					      mapi_table, NULL, NULL);
}

#endif /* _WIN32 */
