// C Source File
// Created 25/02/2005; 10:25:37

#include <tigcclib.h>

// Main Function
void _main(void)
{
	HSym sym;
	HANDLE folder;
	HANDLE top;
	
	// Use SymFindHome
	sym = SymFindHome(SYMSTR("main"));
	folder = sym.folder;
	
	// Use DerefSym
	top = DerefSym(SymFindHome(SYMSTR("main")))->handle;
	printf("%04x %04x\n", folder, top);
}
