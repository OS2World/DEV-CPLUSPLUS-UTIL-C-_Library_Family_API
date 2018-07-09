//
//	  *******************************************************************
//		JdeBP C++ Library Routines 	 	General Public Licence v1.00
//			Copyright (c) 1991,1992	 Jonathan de Boyne Pollard
//	  *******************************************************************
//
// Part of FamAPI.LIB
//

#include "famapi.h"
#include "vio.h"

#pragma argsused
//
//	Write a string of characters, TTY fashion
//
USHORT _APICALL
VioWrtTTy ( const char far * PtrString,
			unsigned short StringLen,
			unsigned short VioHandle )
{
	while (StringLen--) {
		_AH = 0x08 ;
		_BH = 0x00 ;
		geninterrupt(0x10) ;		// Read cell at cursor
		_AH = 0x0E ;
		asm push BX ;				// (pointer modifies ES:BX)
		_AL = *(PtrString)++ ;		// Character (keep attribute)
		asm pop BX ;
		geninterrupt(0x10) ;		// Write a cell, TTY style
	}

	return NO_ERROR ;
}
