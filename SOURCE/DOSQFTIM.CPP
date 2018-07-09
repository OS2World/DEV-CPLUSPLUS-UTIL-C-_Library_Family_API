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
//	Query handle's file time
//
USHORT _APICALL
DosDosQFileTime ( int fd,
				  unsigned short far *PtrDate,
				  unsigned short far *PtrTime)
{
	_BX = fd ;
	_AX = 0x5700 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX ;
	*PtrDate = _DX ;
	*PtrTime = _CX ;
	return NO_ERROR ;
}
