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
//	Write data to a file
//
USHORT _APICALL
DosDosWrite	( unsigned short fd,
			  const void far *base,
			  unsigned short bufsize,
			  unsigned short far *done)
{
	asm push ds ;
	_DS = FP_SEG(base) ;
	_DX = FP_OFF(base) ;
	_CX = bufsize ;
	_BX = fd ;
	_AH = 0x40 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ;
	*done = _AX ;
	return NO_ERROR ;
}
