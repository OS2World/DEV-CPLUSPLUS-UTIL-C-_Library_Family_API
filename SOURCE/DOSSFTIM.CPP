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
//	Set a handle's file time
//
USHORT _APICALL
DosDosSetFileTime (int fd, unsigned short Date, unsigned short Time)
{
	_DX = Date ;
	_CX = Time ;
	_BX = fd ;
	_AX = 0x5701 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX ; else return NO_ERROR ;
}
