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
//	Query current disk
//
USHORT _APICALL
DosQCurDisk ( unsigned short far *disk, unsigned long far *dmap )
{
	unsigned short err, ndrives;

	if (err = DosDosQCurDrive(disk)) return err;

	if (err = DosDosSelCurDrive(*disk, &ndrives)) return err ;

	*dmap = (1 << ndrives) - 1;

	return 0;
}
