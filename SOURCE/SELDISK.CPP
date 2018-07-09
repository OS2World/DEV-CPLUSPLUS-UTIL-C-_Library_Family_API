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
//	Select current disk
//
USHORT _APICALL
DosSelectDisk ( unsigned short disk )
{
	unsigned short ndrives;
	return DosDosSelCurDrive(disk, &ndrives);
}
