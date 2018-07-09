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
//	Get current directory
//
USHORT _APICALL
DosQCurDir	( unsigned short Drive,
			  char far *Buf,
			  unsigned short far * RetLen)
{
	asm push ds ;
	_DS = FP_SEG(Buf) ;
	_SI = FP_OFF(Buf) ;
	_DX = Drive ;
	_AH = 0x47 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ;
	*RetLen = 0 ;
	while (*Buf++) ++*RetLen ;
	return NO_ERROR ;
}
