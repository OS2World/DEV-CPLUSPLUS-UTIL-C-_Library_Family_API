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
//	Set a file's attributes
//
USHORT _APICALL
DosSetFileMode	( const char far *FileName,
				  unsigned short Attribute,
				  unsigned long Reserved)
{
	asm push ds ;
	_DS = FP_SEG(FileName) ;
	_DX = FP_OFF(FileName) ;
	_CX = Attribute ;
	_AX = 0x4301 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ; else return NO_ERROR ;
}
