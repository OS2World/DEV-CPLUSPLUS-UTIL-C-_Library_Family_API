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

USHORT _APICALL
DosDosOpen	( const char far *FileName,
			  unsigned short PMode,
			  unsigned short far *HanPtr)
{
	asm push ds ;
	_DS = FP_SEG(FileName) ;
	_DX = FP_OFF(FileName) ;
	_AX = PMode ;
	_AH = 0x3d ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ;
	*HanPtr = _AX ;
	return NO_ERROR ;
}
