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
//	Create or Truncate files
//
USHORT _APICALL
DosDosCreate	( const char far *FileName,
				  unsigned short Attr,
				  unsigned short far *HanPtr)
{
	asm push ds ;
	_DS = FP_SEG(FileName) ;
	_DX = FP_OFF(FileName) ;
	_CX = Attr ;
	_AX = 0x3c00 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ;
	*HanPtr = _AX ;
	return NO_ERROR ;
}
