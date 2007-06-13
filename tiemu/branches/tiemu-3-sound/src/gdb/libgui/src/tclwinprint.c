/* tclwinprint.c -- Tcl routines for printing on Windows.
   Copyright (C) 1997 Cygnus Solutions.
   Written by Ian Lance Taylor <ian@cygnus.com>.

   This file contains routines to support printing on Windows from
   Tcl.  */

#ifdef _WIN32

#include <windows.h>

#include <tcl.h>
#include <tk.h>

#include "subcommand.h"
#include "guitcl.h"

#include <wingdi.h>

#undef PRINT_BUFSIZE
#define PRINT_BUFSIZE 10240

/* FIXME: We need to dig into the Tk window implementation internals
   to convert a Tk Window to an HWND.  */

#include <tkWinInt.h>

/* This implementation is minimal.  It's just enough to print a text
   file.  Additional features can be added as necessary.

   One interesting idea that would fit into the Windows printing
   scheme would be to have printing generate a limited canvas widget,
   and permit Tk scripts to use canvas commands to draw items on the
   page.

   This file defines a Tcl command with subcommands.

   ide_winprint page_setup OPTIONS
       Invoke the Windows Page Setup dialog.  This will record
       information internally that will be used for later printing.

       Supported options:
           -parent WINDOW
	       Set the parent window of the dialog box.  The dialog
	       box is modal with respect to this window.  The default
	       is the main window.

   ide_winprint print_text QUERYPROC TEXTPROC OPTIONS
       Print text.  This will start formatting the print job.  The
       user will still be able to interact with Tk.  Typically, a
       dialog box would be put up with a cancel button to permit the
       user to cancel the print job by calling ide_winprint abort.

       The QUERYPROC argument is a Tcl procedure which tells the print
       job what to do next.  This is invoked alternately with the text
       procedure until the print job is finished.  QUERYPROC is called
       first.  This should return one of the following strings:
	   continue
	       Just invoke the text procedure and continue
	       printing.
	   done
	       The print job is finished.
	   newpage
	       Skip to a new page and continue printing.

       The TEXTPROC argument is a Tcl procedure which returns a single
       line of text to print.  This procedure will be invoked
       alternately with the query procedure until the query procedure
       indicates that the print job is complete.  Page breaks are
       handled automatically.

       Supported options:
           -dialog BOOLEAN
	       Whether to display the Windows Print dialog.  The
	       default is true.  If false, this will use the default
	       printer.
           -parent WINDOW
	       Set the parent window of the dialog box.  The dialog
	       box is modal with respect to this window.  The default
	       is the main window.
	   -name STRING
	       Set the name of the document.  The default name is the
	       empty string.
	   -pageproc PAGEPROC
	       PAGEPROC is executed at the start of each new page.  It
	       will be called with one argument, which is the page
	       number.  It will be called before either QUERYPROC or
	       TEXTPROC is called on this page.  If QUERYPROC never
	       returns newpage, then PAGEPROC will always be invoked
	       after a call to TEXTPROC.  PAGEPROC should return one
	       of the following strings:
	           continue
		       Keep going.
		   done
		       Stop printing.
           -postscript
	       Use PostScript output.
	   -initproc INITPROC
	       INITPROC is called at the start of the print job.

   ide_winprint abort
       Abort a print job in process.  If there is no current print
       job, this does nothing.

   */

/* An instance of this structure is the client data for the
   ide_winprint command.  */

struct winprint_data
{
  /* Information from the Page Setup dialog.  */
  PAGESETUPDLG *page_setup;
  /* This is set non-zero if the print job is aborted.  */
  int aborted;
};

/* Delete the ide_winprint command.  */

static void
winprint_command_deleted (ClientData cd)
{
  struct winprint_data *wd = (struct winprint_data *) cd;

  if (wd->page_setup != NULL)
    {
      /* FIXME: I don't know if we are supposed to free the hDevMode
         and hDevNames fields.  */
      ckfree ((char *) wd->page_setup);
    }

  ckfree ((char *) wd);
}

/* Implement ide_winprint page_setup.  */

static int
winprint_page_setup_command (ClientData cd, Tcl_Interp *interp, int argc,
			     char **argv)
{
  struct winprint_data *wd = (struct winprint_data *) cd;
  Tk_Window parent;
  int i, mode, ret;
  PAGESETUPDLG psd;

  parent = Tk_MainWindow (interp);

  for (i = 2; i < argc; i += 2)
    {
      if (i + 1 >= argc)
	{
	  Tcl_ResetResult (interp);
	  Tcl_AppendStringsToObj (Tcl_GetObjResult (interp),
				  "value for \"", argv[i], "\" missing",
				  (char *) NULL);
	  return TCL_ERROR;
	}

      if (strcmp (argv[i], "-parent") == 0)
	{
	  parent = Tk_NameToWindow (interp, argv[i + 1],
				    Tk_MainWindow (interp));
	  if (parent == NULL)
	    return TCL_ERROR;
	}
      else
	{
	  Tcl_ResetResult (interp);
	  Tcl_AppendStringsToObj (Tcl_GetObjResult (interp),
				  "unknown option \"", argv[i], "\"",
				  (char *) NULL);
	  return TCL_ERROR;
	}
    }

  if (wd->page_setup != NULL)
    psd = *wd->page_setup;
  else
    {
      memset (&psd, 0, sizeof (PAGESETUPDLG));
      psd.lStructSize = sizeof (PAGESETUPDLG);
      psd.Flags = PSD_DEFAULTMINMARGINS;
    }

  if (Tk_WindowId (parent) == None)
    Tk_MakeWindowExist (parent);
  psd.hwndOwner = Tk_GetHWND (Tk_WindowId (parent));

  mode = Tcl_SetServiceMode (TCL_SERVICE_ALL);

  ret = PageSetupDlg (&psd);

  (void) Tcl_SetServiceMode (mode);

  if (! ret)
    {
      DWORD code;

      code = CommDlgExtendedError ();
      if (code == 0)
	{
	  /* The user pressed cancel.  */
	  return TCL_OK;
	}
      else
	{
	  char buf[20];

	  sprintf (buf, "0x%lx", (unsigned long) code);
	  Tcl_ResetResult (interp);
	  Tcl_AppendStringsToObj (Tcl_GetObjResult (interp),
				  "Windows common dialog error ", buf,
				  (char *) NULL);
	  return TCL_ERROR;
	}
    }

  if (wd->page_setup == NULL)
    wd->page_setup = (PAGESETUPDLG *) ckalloc (sizeof (PAGESETUPDLG));

  *wd->page_setup = psd;

  return TCL_OK;
}

/* The abort function needs a static variable (ewww).  */

static struct winprint_data *abort_wd;

/* This is the abort function we pass to the Windows print routine.  */

static BOOL CALLBACK
abort_function (HDC hdc, int code)
{
  while (Tcl_DoOneEvent (TCL_DONT_WAIT))
    ;
  return ! abort_wd->aborted;
}

/* Handle an error in a Windows system call.  */

static void
windows_error (Tcl_Interp *interp, const char *fn)
{
  char buf[20];

  sprintf (buf, "%lu", (unsigned long) GetLastError ());
  Tcl_ResetResult (interp);
  Tcl_AppendStringsToObj (Tcl_GetObjResult (interp),
			  "Windows error in ", fn, ": ", buf, (char *) NULL);
}

/* This structure holds the options for the ide_winprint print_text
   command.  */

struct print_text_options
{
  /* Whether to use the print dialog.  */
  int dialog;
  /* The parent window.  */
  char *parent;
  /* The document name.  */
  char *name;
  /* The page procedure.  */
  char *pageproc;
  /* The init procedure. This is called once before printing. */
  char *initproc;
  /* Print using PostScript? */
  int postscript;
};

/* Handle options for the ide_winprint print_text command.  */

static int
winprint_print_text_options (struct winprint_data *wd, Tcl_Interp *interp,
			     int argc, char **argv,
			     struct print_text_options *pto)
{
  int i;

  pto->dialog = 1;
  pto->parent = NULL;
  pto->name = "";
  pto->pageproc = NULL;
  pto->postscript = 0;
  pto->initproc = NULL;
  
  for (i = 4; i < argc; i += 2)
    {
      if (i + 1 >= argc)
	{
	  Tcl_ResetResult (interp);
	  Tcl_AppendStringsToObj (Tcl_GetObjResult (interp),
				  "value for \"", argv[i], "\" missing",
				  (char *) NULL);
	  return TCL_ERROR;
	}

      if (strcmp (argv[i], "-dialog") == 0)
	{
	  if (Tcl_GetBoolean (interp, argv[i + 1], &pto->dialog) != TCL_OK)
	    return TCL_ERROR;
	}
      else if (strcmp (argv[i], "-parent") == 0)
	pto->parent = argv[i + 1];
      else if (strcmp (argv[i], "-name") == 0)
	pto->name = argv[i + 1];
      else if (strcmp (argv[i], "-pageproc") == 0)
	pto->pageproc = argv[i + 1];
      else if (strcmp (argv[i], "-initproc") == 0)
	pto->initproc = argv[i + 1];
      else if (strcmp (argv[i], "-postscript") == 0)
	pto->postscript = 1;
      else
	{
	  Tcl_ResetResult (interp);
	  Tcl_AppendStringsToObj (Tcl_GetObjResult (interp),
				  "unknown option \"", argv[i], "\"",
				  (char *) NULL);
	  return TCL_ERROR;
	}
    }

  return TCL_OK;
}

/* Invoke the print dialog for the ide_winprint print_text command.
   We always call PrintDlg, even if the -dialog false option was used,
   because it returns the device context we use for printing.  */

static int
winprint_print_text_dialog (struct winprint_data *wd, Tcl_Interp *interp,
			    const struct print_text_options *pto,
			    PRINTDLG *pd, int *cancelled)
{
  int mode, ret;

  *cancelled = 0;

  memset (pd, 0, sizeof (PRINTDLG));
  pd->lStructSize = sizeof (PRINTDLG);

  if (! pto->dialog)
    pd->Flags = PD_RETURNDEFAULT | PD_RETURNDC;
  else
    {
      Tk_Window parent;

      if (pto->parent == NULL)
	parent = Tk_MainWindow (interp);
      else
	{
	  parent = Tk_NameToWindow (interp, pto->parent,
				    Tk_MainWindow (interp));
	  if (parent == NULL)
	    return TCL_ERROR;
	}
      if (Tk_WindowId (parent) == None)
	Tk_MakeWindowExist (parent);
      pd->hwndOwner = Tk_GetHWND (Tk_WindowId (parent));

      if (wd->page_setup != NULL)
	{
	  pd->hDevMode = wd->page_setup->hDevMode;
	  pd->hDevNames = wd->page_setup->hDevNames;
	}

      pd->Flags = PD_NOSELECTION | PD_RETURNDC | PD_USEDEVMODECOPIES;

      pd->nCopies = 1;
      pd->nFromPage = 1;
      pd->nToPage = 1;
      pd->nMinPage = 1;
      pd->nMaxPage = 0xffff;
    }

  mode = Tcl_SetServiceMode (TCL_SERVICE_ALL);

  ret = PrintDlg (pd);

  (void) Tcl_SetServiceMode (mode);

  if (! ret)
    {
      DWORD code;

      code = CommDlgExtendedError ();

      /* For some errors, the print dialog will already have reported
         an error.  We treat those as though the user pressed cancel.
         Unfortunately, I do not know just which errors those are.  */

      if (code == 0 || code == PDERR_NODEFAULTPRN)
	{
	  *cancelled = 1;
	  return TCL_OK;
	}
      else
	{
	  char buf[20];

	  sprintf (buf, "0x%lx", (unsigned long) code);
	  Tcl_ResetResult (interp);
	  Tcl_AppendStringsToObj (Tcl_GetObjResult (interp),
				  "Windows common dialog error ", buf,
				  (char *) NULL);
	  return TCL_ERROR;
	}
    }

  return TCL_OK;
}

/* Get the margins in device units.  */

static void
winprint_get_margins (struct winprint_data *wd, const PRINTDLG *pd,
		      int *top_ptr, int *left_ptr, int *bottom_ptr)
{
  int topmargin, leftmargin, bottommargin;
  int logx, logy;

  if (wd->page_setup == NULL)
    {
      /* Use 1 inch margins.  */
      topmargin = 1000;
      leftmargin = 1000;
      bottommargin = 1000;
    }
  else
    {
      topmargin = wd->page_setup->rtMargin.top;
      leftmargin = wd->page_setup->rtMargin.left;
      bottommargin = wd->page_setup->rtMargin.bottom;
      if ((wd->page_setup->Flags & PSD_INHUNDREDTHSOFMILLIMETERS) != 0)
	{
	  topmargin = (topmargin * 1000) / 2540;
	  leftmargin = (leftmargin * 1000) / 2540;
	  bottommargin = (bottommargin * 1000) / 2540;
	}
    }

  logx = GetDeviceCaps (pd->hDC, LOGPIXELSX);
  logy = GetDeviceCaps (pd->hDC, LOGPIXELSY);

  topmargin = (topmargin * logy) / 1000;
  leftmargin = (leftmargin * logx) / 1000;
  bottommargin = (bottommargin * logy) / 1000;

  *top_ptr = topmargin;
  *left_ptr = leftmargin;
  *bottom_ptr = GetDeviceCaps (pd->hDC, VERTRES) - bottommargin;
}

/* Prepare to start printing.  */

static int
winprint_start (struct winprint_data *wd, Tcl_Interp *interp, PRINTDLG *pd,
		const struct print_text_options *pto, int *cancelled)
{
  DOCINFO di;

  *cancelled = 0;

  wd->aborted = 0;

  /* We have no way to pass information to the abort function, so we
     need to use a global variable.  */
  abort_wd = wd;
  if (! SetAbortProc (pd->hDC, abort_function))
    {
      windows_error (interp, "SetAbortFunc");
      return TCL_ERROR;
    }

  di.cbSize = sizeof (DOCINFO);
  di.lpszDocName = pto->name;
  di.lpszOutput = NULL;
  di.lpszDatatype = NULL;
  di.fwType = 0;

  if (StartDoc (pd->hDC, &di) <= 0)
    {
      if (GetLastError () == ERROR_CANCELLED)
	*cancelled = 1;
      else
	{
	  windows_error (interp, "StartDoc");
	  return TCL_ERROR;
	}
    }

  return TCL_OK;
}

/* Finish printing.  */

static int
winprint_finish (struct winprint_data *wd, Tcl_Interp *interp,
		 PRINTDLG *pd, int error)
{
  int ret;

  ret = TCL_OK;

  if (error || wd->aborted)
    AbortDoc (pd->hDC);
  else
    {
      if (EndDoc (pd->hDC) <= 0)
	{
	  windows_error (interp, "EndDoc");
	  ret = TCL_ERROR;
	}
    }

  DeleteDC (pd->hDC);

  return ret;
}

/* Values the ide_winprint print_text query or page procedure can
   return.  */

enum winprint_query { Q_CONTINUE, Q_NEWPAGE, Q_DONE };

/* Invoke the query or page procedure for ide_winprint print_text.  */

static int
winprint_print_text_invoke (Tcl_Interp *interp, char *proc, const char *name,
			    enum winprint_query *result)
{
  char *q;

  if (Tcl_Eval (interp, proc) == TCL_ERROR)
    return TCL_ERROR;

  q = Tcl_GetStringFromObj (Tcl_GetObjResult (interp), (int *) NULL);
  if (strcmp (q, "continue") == 0)
    *result = Q_CONTINUE;
  else if (strcmp (q, "newpage") == 0)
    *result = Q_NEWPAGE;
  else if (strcmp (q, "done") == 0)
    *result = Q_DONE;
  else
    {
      Tcl_ResetResult (interp);
      Tcl_AppendStringsToObj (Tcl_GetObjResult (interp),
			      "bad return from ", name, " procedure: \"",
			      q, "\"", (char *) NULL);
      return TCL_ERROR;
    }

  return TCL_OK;  
}

/* Implement ide_winprint print_text.  */
static int
winprint_print_command (ClientData cd, Tcl_Interp *interp, int argc,
			     char **argv)
{
  struct winprint_data *wd = (struct winprint_data *) cd;
  char *queryproc;
  char *textproc;
  struct print_text_options pto;
  PRINTDLG pd;
  int cancelled;
  int top, bottom, left;
  TEXTMETRIC tm;
  POINT pt;
  int lineheight;
  int pageno;
  int error=0, done, needquery;
  struct {
	 short len; /* Defined to be 16 bits.... */
	 char buffer[PRINT_BUFSIZE+1];
  } indata;

  queryproc = argv[2];
  textproc = argv[3];
 
  if (winprint_print_text_options (wd, interp, argc, argv, &pto) != TCL_OK)
    return TCL_ERROR;

  if (winprint_print_text_dialog (wd, interp, &pto, &pd, &cancelled) != TCL_OK)
    return TCL_ERROR;
  if (cancelled)
    return TCL_OK;

  if (pto.postscript)
  {
	int eps_printing = 33;
	int result;
	short bresult = 1; /* EPS printing download suppressed */
	result = Escape (pd.hDC, eps_printing, sizeof (BOOL), (LPCSTR)&bresult, NULL);
	if ( result < 0 )
	{
		/* The EPSPRINTING escape failed! */
		Tcl_AppendElement(interp, 
                   "ide_winprint: EPSPRINTING escape implemented but failed");
		DeleteDC (pd.hDC);
		return TCL_ERROR;
	  }
  }
  else
  {
	winprint_get_margins(wd, &pd, &top, &left, &bottom);
  }

  if (winprint_start (wd, interp, &pd, &pto, &cancelled) != TCL_OK)
    {
      DeleteDC (pd.hDC);
      return TCL_ERROR;
    }
  if (cancelled)
    {
      DeleteDC (pd.hDC);
      return TCL_OK;
    }

  /* init and start init-procedure if available */
  if (pto.initproc != NULL)
  {
    	Tcl_DString initStr;
	char buf[64];
	Tcl_DStringInit (&initStr);
	Tcl_DStringAppend (&initStr, pto.initproc, -1);
	
	/* Here we must pass the customer selection from the PrintDialog
	 * as parameters for the init command, */
	/* From page */
	Tcl_DStringAppendElement (&initStr, "-frompage");
	sprintf (buf, "%i", pd.nFromPage);
	Tcl_DStringAppendElement (&initStr, buf);
	/* To Page */
	Tcl_DStringAppendElement (&initStr, "-topage");
	sprintf (buf, "%i", pd.nToPage);
	Tcl_DStringAppendElement (&initStr, buf);
	/* # Copies */
	Tcl_DStringAppendElement (&initStr, "-copies");
	sprintf (buf, "%i", pd.nCopies);
	Tcl_DStringAppendElement (&initStr, buf);
	/* Print Selection? */
	Tcl_DStringAppendElement (&initStr, "-selection");
	Tcl_DStringAppendElement (&initStr, (pd.Flags&PD_SELECTION) ? "1" : "0");
	
	/* Execute tcl/command */
	if (Tcl_Eval (interp, Tcl_DStringValue(&initStr)) != TCL_OK)
	{
	      Tcl_DStringFree (&initStr);
	      return TCL_ERROR;
	}
	Tcl_DStringFree (&initStr);
  }
    
  if (pto.postscript)
  {
    Tcl_DString pageStr;
    int status, retval, len, i;
    char *l, msgbuf[128];
    enum winprint_query q = 0;
    
    /* Note: NT 4.0 seems to leave the default CTM quite tiny! */
    strcpy (indata.buffer, "\r\nsave\r\ninitmatrix\r\n");
    indata.len = strlen(indata.buffer);
    Escape(pd.hDC, PASSTHROUGH, 0, (LPCSTR)&indata, NULL);
    
    /* Init command for page-procedure */
    if (pto.pageproc != NULL)
      {
	Tcl_DStringInit (&pageStr);
	Tcl_DStringAppend (&pageStr, pto.pageproc, -1);
	Tcl_DStringAppendElement (&pageStr, "-1");
      }
    
    /* Start printing */
    while (1)
      {
    	/* Run page-procedure to update the display */
	status = winprint_print_text_invoke (interp, Tcl_DStringValue(&pageStr), "page", &q);
	if (status != TCL_OK || q == Q_DONE)
	  {
	    error = 1;
	    break;
	  }
	
	/* query next characters to send to printer */
	if (winprint_print_text_invoke (interp, queryproc, "query", &q) != TCL_OK)
	  {
	    error = 1;
	    break;
	  }
	if (q != Q_CONTINUE)
	  {
	    done = 1;
	    break;
	  }
	if (Tcl_Eval (interp, textproc) == TCL_ERROR)
	  {
	    error = 1;
	    break;
	  }
	l = Tcl_GetStringFromObj (Tcl_GetObjResult (interp), &len);
	for (i=0; i<len; i+=PRINT_BUFSIZE)
	  {
	    int lpos = min (PRINT_BUFSIZE, len-i);
	    strncpy (indata.buffer, l+i, lpos);
	    indata.buffer[lpos] = 0;
	    indata.len = lpos;
	    
	    retval = Escape (pd.hDC, PASSTHROUGH, 0, (LPCSTR)&indata, NULL);
	    if (retval < 0)
	      {
		Tcl_AppendElement(interp, "ide_winprint: PASSTHROUGH Escape failed");
		error = 1;
		break;
	      }
	    else if (retval != indata.len)
	      {
		sprintf(msgbuf, "ide_winprint: Short write (%d vs. %d)", retval, indata.len);
		Tcl_AppendElement(interp, msgbuf);
		error = 1;
		break;
	      }
	  }
      }
    
    strcpy (indata.buffer, "\r\nrestore\r\n");
    indata.len = strlen(indata.buffer);
    Escape(pd.hDC, PASSTHROUGH, 0, (LPCSTR)&indata, NULL);
  }
  else
    {
      GetTextMetrics (pd.hDC, &tm);
      pt.x = 0;
      pt.y = tm.tmHeight + tm.tmExternalLeading;
      LPtoDP (pd.hDC, &pt, 1);
      lineheight = pt.y;
      
      pageno = 1;
      
      /* The main print loop.  */
      done = 0;
      error = 0;
      needquery = 1;
      while (1)
	{
	  int y;
	  
	  if (wd->aborted)
	    break;
	  
	  /* Start a new page.  */
	  if (pto.pageproc != NULL)
	    {
	      Tcl_DString ds;
	      char buf[20];
	      enum winprint_query q;
	      int status;
	      
	      Tcl_DStringInit (&ds);
	      Tcl_DStringAppend (&ds, pto.pageproc, -1);
	      sprintf (buf, "%d", pageno);
	      Tcl_DStringAppendElement (&ds, buf);
	      
	      status = winprint_print_text_invoke (interp, Tcl_DStringValue (&ds),
						   "page", &q);
	      
	      Tcl_DStringFree (&ds);
	      
	      if (status != TCL_OK)
		{
		  error = 1;
		  break;
		}
	      
	      if (q == Q_DONE)
		{
		  done = 1;
		  break;
		}
	    }
	  
	  if (needquery)
	    {
	      enum winprint_query q;
	      
	      if (winprint_print_text_invoke (interp, queryproc, "query", &q)
		  != TCL_OK)
		{
		  error = 1;
		  break;
		}
	      
	      if (q == Q_DONE)
		{
		  done = 1;
		  break;
		}
	      
	      /* Ignore Q_NEWPAGE, since we're about to start a new page
		 anyhow.  */
	      
	      needquery = 0;
	    }
	  
	  if (StartPage (pd.hDC) <= 0)
	    {
	      windows_error (interp, "StartPage");
	      error = 1;
	      break;
	    }
	  
	  y = top;
	  
	  /* Print a page.  */
	  
	  while (1)
	    {
	      char *l;
	      int len;
	      enum winprint_query q;
	      
	      if (Tcl_Eval (interp, textproc) == TCL_ERROR)
		{
		  error = 1;
		  break;
		}
	      
	      l = Tcl_GetStringFromObj (Tcl_GetObjResult (interp), &len);
	      
	      TextOut (pd.hDC, left, y, l, len);
	      y += lineheight;
	      
	      if (y >= bottom)
		{
		  needquery = 1;
		  break;
		}
	      
	      if (winprint_print_text_invoke (interp, queryproc, "query", &q)
		  != TCL_OK)
		{
		  error = 1;
		  break;
		}
	      
	      if (q == Q_DONE)
		{
		  done = 1;
		  break;
		}
	      else if (q == Q_NEWPAGE)
		break;
	    }
	  
	  if (error)
	    break;
	  
	  if (EndPage (pd.hDC) <= 0)
	    {
	      /* It's OK for EndPage to return an error if the print job
		 was cancelled.  */
	      if (! wd->aborted)
		{
		  windows_error (interp, "EndPage");
		  error = 1;
		}
	      break;
	    }
	  
	  if (done)
	    break;
	  
	  ++pageno;
	}
    }
  
  if (winprint_finish (wd, interp, &pd, error) != TCL_OK)
    error = 1;
  
  if (error)
    return TCL_ERROR;
  
  Tcl_ResetResult (interp);
  return TCL_OK;
}

/* Implement ide_winprint abort.  */

static int
winprint_abort_command (ClientData cd, Tcl_Interp *interp, int argc,
			char **argv)
{
  struct winprint_data *wd = (struct winprint_data *) cd;

  wd->aborted = 1;
  return TCL_OK;
}

/* The subcommand table.  */

static const struct ide_subcommand_table winprint_commands[] =
{
  { "page_setup",	winprint_page_setup_command,	2, -1 },
  { "print_text",	winprint_print_command,	4, -1 },
  { "print",	     winprint_print_command,	6, -1 },
  { "abort",		    winprint_abort_command,		2, 2 },
  { NULL, NULL, 0, 0 }
};

/* This function creates the ide_winprint Tcl command.  */

int
ide_create_winprint_command (Tcl_Interp *interp)
{
  struct winprint_data *wd;
  
  wd = (struct winprint_data *) ckalloc (sizeof *wd);
  wd->page_setup = NULL;
  wd->aborted = 0;
  
  return ide_create_command_with_subcommands (interp, "ide_winprint",
					      winprint_commands,
					      (ClientData) wd,
					      winprint_command_deleted);
}

#endif /* _WIN32 */






