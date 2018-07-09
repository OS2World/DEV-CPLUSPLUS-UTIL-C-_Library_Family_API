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
//	Read data from a file
//
USHORT _APICALL
DosRead	( unsigned short Fd,
		  void far *Base,
		  unsigned short BufSize,
		  unsigned short far *Done )
{
	asm push ds ;
	_DS = FP_SEG(Base) ;
	_DX = FP_OFF(Base) ;
	_CX = BufSize ;
	_BX = Fd ;
	_AH = 0x3f ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ;
	*Done = _AX ;
	return NO_ERROR ;
}
