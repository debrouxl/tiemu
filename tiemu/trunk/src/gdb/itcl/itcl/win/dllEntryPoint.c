/* 
 * dllEntryPoint.c --
 *
 *	This file implements the Dll entry point as needed by Windows.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* CYGNUS LOCAL */
#include <tcl.h>

#ifdef __CYGWIN32__
/*
 * The following declaration is for the VC++ DLL entry point.
 */

BOOL APIENTRY		DllMain _ANSI_ARGS_((HINSTANCE hInst,
			    DWORD reason, LPVOID reserved));

/* cygwin32 requires an impure pointer variable, which must be
   explicitly initialized when the DLL starts up.  */
struct _reent *_impure_ptr;
extern struct _reent *_imp__reent_data;

/*
 *----------------------------------------------------------------------
 *
 * DllMain --
 *
 *	DLL entry point.
 *
 * Results:
 *	TRUE on sucess, FALSE on failure.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

BOOL APIENTRY
DllMain(hInstance, reason, reserved)
    HINSTANCE hInstance;
    DWORD reason;
    LPVOID reserved;
{
    /* CYGNUS LOCAL */
    /* cygwin32 requires the impure data pointer to be initialized
       when the DLL starts up.  */
    _impure_ptr = _imp__reent_data;
    /* END CYGNUS LOCAL */
    
    return(TRUE);
}

/* END CYGNUS LOCAL */
#else /* __CYGWIN32__ */

#if defined(_MSC_VER)
#   define DllEntryPoint DllMain
#endif

/*
 *----------------------------------------------------------------------
 *
 * DllEntryPoint --
 *
 *	This wrapper function is used by Windows to invoke the
 *	initialization code for the DLL.  If we are compiling
 *	with Visual C++, this routine will be renamed to DllMain.
 *
 * Results:
 *	Returns TRUE;
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

BOOL APIENTRY
DllEntryPoint(hInst, reason, reserved)
    HINSTANCE hInst;		/* Library instance handle. */
    DWORD reason;		/* Reason this function is being called. */
    LPVOID reserved;		/* Not used. */
{
    return TRUE;
}

#endif
