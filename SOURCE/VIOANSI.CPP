//
//	  *******************************************************************
//		JdeBP C++ Library Routines 	 	General Public Licence v1.00
//			Copyright (c) 1991,1992	 Jonathan de Boyne Pollard
//	  *******************************************************************
//
// Part of FamAPI.LIB
//

#include "famapi.h"
#include "vio.h"

#pragma argsused
//
//	Get ANSI enable/disable status
//
USHORT _APICALL
VioGetAnsi	( unsigned short far *PtrAnsi,
			  unsigned short VioHandle )
{
	return NO_ERROR;
}

#pragma argsused
//
//	Set ANSI enable/disable status
//
USHORT _APICALL
VioSetAnsi	( unsigned short Ansi,
			  unsigned short VioHandle )
{
	return NO_ERROR;
}

