/* tclsizebox.c -- Tcl code to create a sizebox on Windows.
   Copyright (C) 1997, 1998 Cygnus Solutions.
   Written by Ian Lance Taylor <ian@cygnus.com>.  */

#ifdef _WIN32

#include <windows.h>

#include <tcl.h>
#include <tk.h>

#include "guitcl.h"

/* We need to make some Tk internal calls.  The only alternative is to
   actually move this code into Tk.  */

#include <tkWinInt.h>

/* These should really be defined in the cygwin32 header files.  */

#ifndef GetStockPen
#define GetStockPen(p) ((HPEN) GetStockObject (p))
#define GetStockBrush(b) ((HBRUSH) GetStockObject (b))
#define SelectPen(dc, p) (SelectObject (dc, (HGDIOBJ) p))
#define SelectBrush(dc, b) (SelectObject (dc, (HGDIOBJ) b))
#define DeleteBrush(b) (DeleteObject ((HGDIOBJ) b))
#endif

/* This file defines the Tcl command sizebox.

   sizebox PATHNAME [OPTIONS]

   Creates a sizebox named PATHNAME.  This accepts the standard window
   options.  This should be attached to the lower right corner of a
   window in order to work as expected.  */

/* We use 

/* We use an instance of the structure as the Windows user data for
   the window.  */

struct sizebox_userdata
{
  /* The real window procedure.  */
  WNDPROC wndproc;
  /* The Tk window.  */
  Tk_Window tkwin;
};

/* The window procedure we use for a sizebox.  The default sizebox
   handling doesn't seem to erase the background if the sizebox is not
   exactly the correct size, so we handle that here.  */

static LRESULT CALLBACK
sizebox_wndproc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  struct sizebox_userdata *su;

  su = (struct sizebox_userdata *) GetWindowLong (hwnd, GWL_USERDATA);

  switch (msg)
    {
    case WM_ERASEBKGND:
      /* The default sizebox handling doesn't seem to erase the
         background if the sizebox is not exactly the correct size, so
         we handle that here.  */
      if (Tk_Height (su->tkwin) != GetSystemMetrics (SM_CYHSCROLL)
	  || Tk_Width (su->tkwin) != GetSystemMetrics (SM_CXVSCROLL))
	{
	  HDC hdc = (HDC) wparam;
	  RECT r;
	  HPEN hpen;
	  HBRUSH hbrush;

	  GetClientRect (hwnd, &r);
	  hpen = SelectPen (hdc, GetStockPen (NULL_PEN));
	  hbrush = SelectBrush (hdc, GetSysColorBrush (COLOR_3DFACE));
	  Rectangle (hdc, r.left, r.top, r.right + 1, r.bottom + 1);
	  hbrush = SelectBrush (hdc, hbrush);
	  DeleteBrush (hbrush);
	  SelectPen (hdc, hpen);
	  return 1;
	}
      break;

      /* We need to handle cursor handling here.  We also use Tk
         cursor handling via a call to Tk_DefineCursor, but we can't
         rely on it, because it will only take effect if Tk sees a
         MOUSEMOVE event which won't happen if the mouse moves
         directly from outside any Tk window to the sizebox.  */
    case WM_SETCURSOR:
      SetCursor (LoadCursor (NULL, IDC_SIZENWSE));
      return 1;
    }

  return CallWindowProc (su->wndproc, hwnd, msg, wparam, lparam);
}

/* This is called by the Tk dispatcher for various events.  */

static void
sizebox_event_proc (ClientData cd, XEvent *event_ptr)
{
  HWND hwnd = (HWND) cd;
  struct sizebox_userdata *su;

  if (! hwnd)
    return;

  if (event_ptr->type == DestroyNotify)
    {
      su = (struct sizebox_userdata *) GetWindowLong (hwnd, GWL_USERDATA);
      SetWindowLong (hwnd, GWL_USERDATA, 0);
      SetWindowLong (hwnd, GWL_WNDPROC, (LONG) su->wndproc);
      ckfree ((char *) su);
      DestroyWindow (hwnd);
    }
}

/* Create a sizebox window.  */

static Window
sizebox_create (Tk_Window tkwin, Window parent, ClientData cd)
{
  POINT pt;
  Tk_Window parwin;
  HWND parhwnd;
  HWND hwnd;
  struct sizebox_userdata *su;
  Window result;

  /* We need to tell Windows that the parent of the sizebox is the
     toplevel which holds it.  Otherwise the sizebox will try to
     resize the child window, which doesn't make much sense.  */

  pt.x = Tk_X (tkwin);
  pt.y = Tk_Y (tkwin);
  ClientToScreen (TkWinGetHWND (parent), &pt);

  parwin = (Tk_Window) TkWinGetWinPtr (parent);
  while (! Tk_IsTopLevel (parwin))
    parwin = Tk_Parent (parwin);
  parhwnd = TkWinGetWrapperWindow (parwin);

  ScreenToClient (parhwnd, &pt);

  hwnd = CreateWindow ("SCROLLBAR", NULL,
		       WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP,
		       pt.x, pt.y, Tk_Width (tkwin), Tk_Height (tkwin),
		       parhwnd, NULL, Tk_GetHINSTANCE (), NULL);

  su = (struct sizebox_userdata *) ckalloc (sizeof *su);
  su->tkwin = tkwin;
  su->wndproc = (WNDPROC) GetWindowLong (hwnd, GWL_WNDPROC);
  SetWindowLong (hwnd, GWL_USERDATA, (LONG) su);
  SetWindowLong (hwnd, GWL_WNDPROC, (LONG) sizebox_wndproc);

  SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0,
	       SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

  result = Tk_AttachHWND (tkwin, hwnd);

  Tk_CreateEventHandler (tkwin, StructureNotifyMask, sizebox_event_proc,
			 hwnd);

  return result;
}

/* The class procedure table for a sizebox widget.  This is an
   internal Tk structure.  */

static TkClassProcs sizebox_procs =
{
  sizebox_create,		/* createProc */
  NULL,				/* geometryProc */
  NULL				/* modalProc */
};

/* The implementation of the sizebox command.  */

static int
sizebox_command (ClientData cd, Tcl_Interp *interp, int argc, char **argv)
{
  Tk_Window tkmain;
  Tk_Window new;
  Tk_Cursor cursor;

  if (argc < 2)
    {
      Tcl_ResetResult (interp);
      Tcl_AppendStringsToObj(Tcl_GetObjResult (interp),
			     "wrong # args: should be \"",
			     argv[0], " pathname ?options?\"", (char *) NULL);
      return TCL_ERROR;
    }

  tkmain = Tk_MainWindow (interp);
  if (tkmain == NULL)
    return TCL_ERROR;

  new = Tk_CreateWindowFromPath (interp, tkmain, argv[1], (char *) NULL);
  if (new == NULL)
    return TCL_ERROR;

  Tk_SetClass (new, "Sizebox");

  /* This is a Tk internal function.  */
  TkSetClassProcs (new, &sizebox_procs, NULL);

  /* FIXME: We should handle options here, but we currently don't have
     any.  */

  Tk_GeometryRequest (new, GetSystemMetrics (SM_CXVSCROLL),
		      GetSystemMetrics (SM_CYHSCROLL));

  cursor = Tk_GetCursor (interp, new, Tk_GetUid ("size_nw_se"));
  if (cursor == None)
    return TCL_ERROR;
  Tk_DefineCursor (new, cursor);

  Tcl_SetResult (interp, Tk_PathName (new), TCL_STATIC);
  return TCL_OK;
}

/* Create the sizebox command.  */

int
ide_create_sizebox_command (Tcl_Interp *interp)
{
  if (Tcl_CreateCommand (interp, "ide_sizebox", sizebox_command, NULL,
			 NULL) == NULL)
    return TCL_ERROR;
  return TCL_OK;
}

#endif /* _WIN32 */
