//
//	  *******************************************************************
//		JdeBP C++ Library Routines 	 	General Public Licence v1.00
//			Copyright (c) 1991,1992	 Jonathan de Boyne Pollard
//	  *******************************************************************
//
// Part of FamAPI.LIB
//

#include "famapi.h"
#include "dosdos.h"

//
//	Duplicate a file handle
//
USHORT _APICALL
DosDupHandle ( unsigned short From, unsigned short far *To)
{
	if ((_CX = *To) == (unsigned short)-1)
		_AX = 0x4500 ;				// Duplicate a handle
	else
		_AX = 0x4600 ;				// Force duplicate a handle
	_BX = From ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX ; else return NO_ERROR ;
}
