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
//	Close a file
//
USHORT _APICALL
DosClose	(unsigned short fd)
{
	_BX = fd ;
	_AH = 0x3e ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX; else return NO_ERROR ;
}
