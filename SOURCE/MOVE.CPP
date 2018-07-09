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
//	Move a file within the namespace
//
USHORT _APICALL
DosMove		( const char far *OldName,
			  const char far *NewName,
			  unsigned long Reserved )
{
	asm push ds ;
	_DS = FP_SEG(OldName) ;
	_DX = FP_OFF(OldName) ;
	_ES = FP_SEG(NewName) ;
	_DI = FP_OFF(NewName) ;
	_AH = 0x56 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ; else return NO_ERROR ;
}
