This directory contains the header file and CLSID declarations allowing you to
easily make calls to TiEmu through the OLE Automation IPC interface. The files
tiemuole.c and tiemuole.h are copies of src/com_ole/oleaut.h and
src/com_ole/oleaut_i.c, respectively, which have been generated from
src/com_ole/oleaut.idl using widl or midl.

In order to use TiEmu through OLE Automation:
* compile and link tiemuole.c with your project
* include "tiemuole.h" into your C or C++ files which will be using TiEmu
* obtain a "TiEmu.TiEmuOLE" COM object through GetActiveObject. If this fails,
  you have to run TiEmu manually (e.g. using ShellExecute).
  WARNING: TiEmu currently does NOT provide a class factory. Therefore, you
           can't create objects automatically. You have to run TiEmu if it isn't
           running yet, and then use GetActiveObject.
  The interface pointer should be an ITiEmuOLE *.
* You can now call any function in the TiEmu OLE Automation interface through
  your ITiEmuOLE * interface pointer.

For your convenience, sample code (oletest.c) is provided under the GPL.

IMPORTANT: This is for Win32 only. For *nix, please look at the DCOP interface,
           as defined in the dcopsdk directory.
