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
//	Change drive
//
USHORT _APICALL
DosDosSelCurDrive ( unsigned short Drive, unsigned short far *NDrives)
{
	_DX = Drive ;
	_AH = 0x0e ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX ;
	*NDrives = _AX ;
	return NO_ERROR ;
}
