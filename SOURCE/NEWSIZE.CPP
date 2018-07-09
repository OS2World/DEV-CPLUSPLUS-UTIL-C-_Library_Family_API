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
//	Change file size
//
USHORT _APICALL
DosNewSize ( unsigned short fd, unsigned long size )
{
	long oldpos, endpos;
	unsigned short err, nbytes;

	if (!(err = DosChgFilePtr(fd, 0L, 1, &oldpos))		// SEEK_CUR
	&&  !(err = DosChgFilePtr(fd, size, 0, &endpos))	// SEEK_SET
	&&  !(err = DosDosWrite(fd, NULL, 0, &nbytes))		// Internal version
	&&  !(err = DosChgFilePtr(fd, oldpos, 0, &oldpos)))	// SEEK_SET
		return NO_ERROR ;

	return err;
}
