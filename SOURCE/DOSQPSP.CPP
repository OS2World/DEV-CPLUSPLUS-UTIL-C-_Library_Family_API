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
DosDosQPSPSeg	( unsigned short far * PtrPSPSeg )
{
	_AX = 0x6200 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX ;
	_AX = _BX ;
	*PtrPSPSeg = _AX ;		// This mangles ES:BX
	return NO_ERROR ;
}
