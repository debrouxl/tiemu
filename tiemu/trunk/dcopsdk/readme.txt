This directory contains the stub object allowing you to easily make calls to
TiEmu through the DCOP IPC interface. The files tiemu_stub.* have been generated
from src/kde/dcop.h using dcopidl and dcopidl2cpp.

In order to use TiEmu through DCOP:
* compile and link tiemu_stub.cpp with your project
* include "tiemu.h" into your C++ files which will be using TiEmu
* create a TiEmuDCOP_stub object. The app parameter should be "tiemu_PID", where
  PID is the PID of a running TiEmu process (you can use
  DCOPClient::registeredApplications() to find it). The id parameter should be
  "TiEmuDCOP".
* You can now call any function in the TiEmu DCOP interface through your
  TiEmuDCOP_stub object.

IMPORTANT: This is for *nix only. For Win32, please look at the OLE interface,
           as defined in oleaut.h in the src/com_ole directory.
