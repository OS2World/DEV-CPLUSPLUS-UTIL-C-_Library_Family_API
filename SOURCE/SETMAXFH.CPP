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
//	Set maximum file handles
//
USHORT _APICALL
DosSetMaxFH ( unsigned short NumHandles )
{
	_BX = NumHandles ;
	_AH = 0x67 ;
	Dos3Call() ;
	return (_FLAGS & 0x0001) ? _AX : 0 ;
}
