/* tclmsgbox.c -- Tcl code to handle a Windows MessageBox in the background.
   Copyright (C) 1998 Cygnus Solutions.
   Written by Ian Lance Taylor <ian@cygnus.com>.  */

#ifdef _WIN32

#include <tcl.h>
#include <tk.h>

#include <windows.h>

/* FIXME: We use some internal Tcl and Tk Windows stuff.  */
#include <tkWinInt.h>

EXTERN HINSTANCE TclWinGetTclInstance (void);

#include "guitcl.h"

/* This file defines a single Tcl command.

   ide_messageBox CODE [ARGUMENTS]

       This is just like tk_messageBox, except that it does not return
       a value.  Instead, when the user clicks on a button closing the
       message box, this invokes CODE, appending the selected value.

       On Windows, this runs the MessageBox function in another
       thread.  This permits a program which handles IDE requests from
       other programs to not return from the request until the
       MessageBox completes.  This is not possible without using
       another thread, since the MessageBox function call will be
       running its own event loop, and will be higher on the stack
       than the IDE request.

       On Unix tk_messageBox runs in the regular Tk event loop, so
       another thread is not required.

   */

static LRESULT CALLBACK msgbox_wndproc (HWND, UINT, WPARAM, LPARAM);
static int msgbox_eventproc (Tcl_Event *, int);

/* The hidden message box window.  */

static HWND hidden_hwnd;

/* The message number we use to indicate that the MessageBox call has
   completed.  */

#define MSGBOX_MESSAGE (WM_USER + 1)

/* We pass a pointer to this structure to the thread function.  It
   passes it back to the hidden window procedure.  */

struct msgbox_data
{
  /* Tcl interpreter.  */
  Tcl_Interp *interp;
  /* Tcl code to execute when MessageBox completes.  */
  char *code;
  /* Hidden window handle.  */
  HWND hidden_hwnd;
  /* MessageBox arguments.  */
  HWND hwnd;
  char *message;
  char *title;
  int flags;
  /* Result of MessageBox call.  */
  int result;
};

/* This is the structure we pass to Tcl_QueueEvent.  */

struct msgbox_event
{
  /* The base structure for all events.  */
  Tcl_Event header;
  /* The message box data for this event.  */
  struct msgbox_data *md;
};

/* Initialize a hidden window to handle messages from the message box
   thread.  */

static int
msgbox_init ()
{
  WNDCLASS class;

  if (hidden_hwnd != NULL)
    return TCL_OK;

  class.style = 0;
  class.cbClsExtra = 0;
  class.cbWndExtra = 0;
  class.hInstance = TclWinGetTclInstance();
  class.hbrBackground = NULL;
  class.lpszMenuName = NULL;
  class.lpszClassName = "ide_messagebox";
  class.lpfnWndProc = msgbox_wndproc;
  class.hIcon = NULL;
  class.hCursor = NULL;

  if (! RegisterClass (&class))
    return TCL_ERROR;

  hidden_hwnd = CreateWindow ("ide_messagebox", "ide_messagebox", WS_TILED,
			      0, 0, 0, 0, NULL, NULL, class.hInstance, NULL);
  if (hidden_hwnd == NULL)
    return TCL_ERROR;

  return TCL_OK;
}

/* This is called as an exit handler.  */

static void
msgbox_exit (ClientData cd)
{
  if (hidden_hwnd != NULL)
    {
      UnregisterClass ("ide_messagebox", TclWinGetTclInstance ());
      DestroyWindow (hidden_hwnd);
      hidden_hwnd = NULL;

      /* FIXME: Ideally we would kill off any remaining threads and
         somehow free up the associated data.  */
    }
}

/* This is the thread function which actually invokes the MessageBox
   function.  This function runs in a separate thread.  */

static DWORD WINAPI
msgbox_thread (LPVOID arg)
{
  struct msgbox_data *md = (struct msgbox_data *) arg;

  md->result = MessageBox (md->hwnd, md->message, md->title,
			   md->flags | MB_SETFOREGROUND);
  PostMessage (md->hidden_hwnd, MSGBOX_MESSAGE, 0, (LPARAM) arg);
  return 0;
}

/* This function handles Windows events for the hidden window.  When
   the MessageBox function call completes in the thread, this function
   will be called with MSGBOX_MESSAGE.  */

static LRESULT CALLBACK
msgbox_wndproc (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
  struct msgbox_event *me;

  if (message != MSGBOX_MESSAGE)
    return DefWindowProc (hwnd, message, wparam, lparam);

  /* Queue up a Tcl event.  */
  me = (struct msgbox_event *) ckalloc (sizeof *me);
  me->header.proc = msgbox_eventproc;
  me->md = (struct msgbox_data *) lparam;
  Tcl_QueueEvent ((Tcl_Event *) me, TCL_QUEUE_TAIL);

  return 0;
}

/* This function handles Tcl events.  It is invoked when a MessageBox
   has completed.  */

static int
msgbox_eventproc (Tcl_Event *event, int flags)
{
  struct msgbox_event *me = (struct msgbox_event *) event;
  char *resstr;
  Tcl_DString ds;
  int ret;

  /* Only execute the Tcl code if we are waiting for window events.  */
  if ((flags & TCL_WINDOW_EVENTS) == 0)
    return 0;

  /* This switch is copied from Tk_MessageBoxCmd in Tk.  */
  switch (me->md->result)
    {
      case IDABORT:	resstr = "abort";  break;
      case IDCANCEL:	resstr = "cancel"; break;
      case IDIGNORE:	resstr = "ignore"; break;
      case IDNO:	resstr = "no";     break;
      case IDOK:	resstr = "ok";     break;
      case IDRETRY:	resstr = "retry";  break;
      case IDYES:	resstr = "yes";    break;
      default:		resstr = "";
    }

  Tcl_DStringInit (&ds);
  Tcl_DStringAppend (&ds, me->md->code, -1);
  Tcl_DStringAppendElement (&ds, resstr);

  /* FIXME: What if the interpreter has been deleted?  */
  ret = Tcl_GlobalEval (me->md->interp, Tcl_DStringValue (&ds));

  Tcl_DStringFree (&ds);

  /* We are now done with the msgbox_data structure, so we can free
     the fields and the structure itself.  */
  ckfree (me->md->code);
  ckfree (me->md->message);
  ckfree (me->md->title);
  ckfree ((char *) me->md);

  if (ret != TCL_OK)
    Tcl_BackgroundError (me->md->interp);

  return 1;
}

/* This is a direct steal from tkWinDialog.c, for the use of msgbox.
   I kept the same formatting as well, to make it easier to merge
   changes.  */

typedef struct MsgTypeInfo {
    char * name;
    int type;
    int numButtons;
    char * btnNames[3];
} MsgTypeInfo;

#define NUM_TYPES 6

static MsgTypeInfo 
msgTypeInfo[NUM_TYPES] = {
    {"abortretryignore", MB_ABORTRETRYIGNORE, 3, {"abort", "retry", "ignore"}},
    {"ok", 		 MB_OK, 	      1, {"ok"                      }},
    {"okcancel",	 MB_OKCANCEL,	      2, {"ok",    "cancel"         }},
    {"retrycancel",	 MB_RETRYCANCEL,      2, {"retry", "cancel"         }},
    {"yesno",		 MB_YESNO,	      2, {"yes",   "no"             }},
    {"yesnocancel",	 MB_YESNOCANCEL,      3, {"yes",   "no",    "cancel"}}
};

/* This is mostly a direct steal from Tk_MessageBoxCmd in Tk.  I kept
   the same formatting as well, to make it easier to merge changes.  */

static int
msgbox_internal (ClientData clientData, Tcl_Interp *interp, int argc,
		 char **argv, char *code)
{
    int flags;
    Tk_Window parent = NULL;
    HWND hWnd;
    char *message = "";
    char *title = "";
    int icon = MB_ICONINFORMATION;
    int type = MB_OK;
    int modal = MB_SYSTEMMODAL;
    int i, j;
    char *defaultBtn = NULL;
    int defaultBtnIdx = -1;

    for (i=1; i<argc; i+=2) {
	int v = i+1;
	int len = strlen(argv[i]);

	if (strncmp(argv[i], "-default", len)==0) {
	    if (v==argc) {goto arg_missing;}

	    defaultBtn = argv[v];
	}
	else if (strncmp(argv[i], "-icon", len)==0) {
	    if (v==argc) {goto arg_missing;}

	    if (strcmp(argv[v], "error") == 0) {
		icon = MB_ICONERROR;
	    }
	    else if (strcmp(argv[v], "info") == 0) {
		icon = MB_ICONINFORMATION;
	    }
	    else if (strcmp(argv[v], "question") == 0) {
		icon = MB_ICONQUESTION;
	    }
	    else if (strcmp(argv[v], "warning") == 0) {
		icon = MB_ICONWARNING;
	    }
	    else {
	        Tcl_AppendResult(interp, "invalid icon \"", argv[v],
		    "\", must be error, info, question or warning", NULL);
		return TCL_ERROR;
	    }
	}
	else if (strncmp(argv[i], "-message", len)==0) {
	    if (v==argc) {goto arg_missing;}

	    message = argv[v];
	}
	else if (strncmp(argv[i], "-parent", len)==0) {
	    if (v==argc) {goto arg_missing;}

	    parent=Tk_NameToWindow(interp, argv[v], Tk_MainWindow(interp));
	    if (parent == NULL) {
		return TCL_ERROR;
	    }
	}
	else if (strncmp(argv[i], "-title", len)==0) {
	    if (v==argc) {goto arg_missing;}

	    title = argv[v];
	}
	else if (strncmp(argv[i], "-type", len)==0) {
	    int found = 0;

	    if (v==argc) {goto arg_missing;}

	    for (j=0; j<NUM_TYPES; j++) {
		if (strcmp(argv[v], msgTypeInfo[j].name) == 0) {
		    type = msgTypeInfo[j].type;
		    found = 1;
		    break;
		}
	    }
	    if (!found) {
		Tcl_AppendResult(interp, "invalid message box type \"", 
		    argv[v], "\", must be abortretryignore, ok, ",
		    "okcancel, retrycancel, yesno or yesnocancel", NULL);
		return TCL_ERROR;
	    }
	}
	else if (strncmp (argv[i], "-modal", len) == 0) {
	    if (v==argc) {goto arg_missing;}

	    if (strcmp(argv[v], "system") == 0) {
		modal = MB_SYSTEMMODAL;
	    }
	    else if (strcmp(argv[v], "task") == 0) {
		modal = MB_TASKMODAL;
	    }
	    else if (strcmp(argv[v], "owner") == 0) {
		modal = MB_APPLMODAL;
	    }
	    else {
		Tcl_AppendResult(interp, "invalid modality \"", argv[v],
		    "\", must be system, task or owner", NULL);
		return TCL_ERROR;
	    }
	}
	else {
    	    Tcl_AppendResult(interp, "unknown option \"", 
		argv[i], "\", must be -default, -icon, ",
		"-message, -parent, -title or -type", NULL);
		return TCL_ERROR;
	}
    }

    /* Make sure we have a valid hWnd to act as the parent of this message box
     */
    if (parent == NULL && modal == MB_TASKMODAL) {
	hWnd = NULL;
    }
    else {
	if (parent == NULL) {
	    parent = Tk_MainWindow(interp);
	}
	if (Tk_WindowId(parent) == None) {
	    Tk_MakeWindowExist(parent);
	}
	hWnd = Tk_GetHWND(Tk_WindowId(parent));
    }

    if (defaultBtn != NULL) {
	for (i=0; i<NUM_TYPES; i++) {
	    if (type == msgTypeInfo[i].type) {
		for (j=0; j<msgTypeInfo[i].numButtons; j++) {
		    if (strcmp(defaultBtn, msgTypeInfo[i].btnNames[j])==0) {
		        defaultBtnIdx = j;
			break;
		    }
		}
		if (defaultBtnIdx < 0) {
		    Tcl_AppendResult(interp, "invalid default button \"",
			defaultBtn, "\"", NULL);
		    return TCL_ERROR;
		}
		break;
	    }
	}

	switch (defaultBtnIdx) {
	  case 0: flags = MB_DEFBUTTON1; break;
	  case 1: flags = MB_DEFBUTTON2; break;
	  case 2: flags = MB_DEFBUTTON3; break;
	  case 3: flags = MB_DEFBUTTON4; break;
	}
    } else {
	flags = 0;
    }
    
    flags |= icon | type;

    /* At this point we diverge from Tk_MessageBoxCmd.  */
    {
      struct msgbox_data *md;
      HANDLE thread;
      DWORD tid;

      msgbox_init ();

      md = (struct msgbox_data *) ckalloc (sizeof *md);
      md->interp = interp;
      md->code = ckalloc (strlen (code) + 1);
      strcpy (md->code, code);
      md->hidden_hwnd = hidden_hwnd;
      md->hwnd = hWnd;
      md->message = ckalloc (strlen (message) + 1);
      strcpy (md->message, message);
      md->title = ckalloc (strlen (title) + 1);
      strcpy (md->title, title);
      md->flags = flags | modal;

      /* Start the thread.  This will call MessageBox, and then start
         the ball rolling to execute the specified code.  */
      thread = CreateThread (NULL, 0, msgbox_thread, (LPVOID) md, 0, &tid);
      CloseHandle (thread);
    }

    return TCL_OK;

  arg_missing:
    Tcl_AppendResult(interp, "value for \"", argv[argc-1], "\" missing",
	NULL);
    return TCL_ERROR;
}

/* This is the ide_messageBox function.  */

static int
msgbox (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  if (argc < 2)
    {
      char buf[10];

      sprintf (buf, "%d", argc);
      Tcl_AppendResult (interp, "wrong # args: got ", buf,
			" but expected at least 2", (char *) NULL);
      return TCL_ERROR;
    }

  /* Note that we don't bother to pass the correct value for argv[0]
     to msgbox_internal, since it doesn't look at it anyhow.  Note
     that we will pass a NULL clientdata argument.  */
  return msgbox_internal (cd, interp, argc - 1, argv + 1, argv[1]);
}

/* Create the Tcl command.  */

int
ide_create_messagebox_command (Tcl_Interp *interp)
{
  Tcl_CreateExitHandler (msgbox_exit, NULL);
  if (Tcl_CreateCommand (interp, "ide_messageBox", msgbox, NULL, NULL) == NULL)
    return TCL_ERROR;
  return TCL_OK;
}

#endif  /* _WIN32 */
