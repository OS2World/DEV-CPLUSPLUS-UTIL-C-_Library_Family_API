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
//	Get current drive
//
USHORT _APICALL
DosDosQCurDrive ( unsigned short far *Drive )
{
	_AH = 0x19 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX ;
	*Drive = _AX ;
	return NO_ERROR ;
}
