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
//	Query file's attributes
//
USHORT _APICALL
DosQFileMode ( const char far *FileName,
			   unsigned short far *Attribute,
			   unsigned long Reserved )
{
	asm push ds ;
	_DS = FP_SEG(FileName) ;
	_DX = FP_OFF(FileName) ;
	_AX = 0x4300 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ;
	*Attribute = _CX ;
	return NO_ERROR ;
}
