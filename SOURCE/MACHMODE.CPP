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
//	Get machine mode
//
USHORT _APICALL
DosGetMachineMode ( unsigned char far *pmode )
{
	return *pmode = 0 ;		// Real mode
}