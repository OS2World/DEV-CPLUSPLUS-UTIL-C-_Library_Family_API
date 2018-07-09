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
//	Obtain version number
//
USHORT _APICALL
DosGetVersion ( unsigned short far *PtrVersion )
{
	_AH = 0x30 ;
	Dos3Call() ;
	asm xchg ah,al ;
	*PtrVersion = _AX ;
	return NO_ERROR ;
}
