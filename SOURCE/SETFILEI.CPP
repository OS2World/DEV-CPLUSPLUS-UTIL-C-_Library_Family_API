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
//	Query file's information
//
USHORT _APICALL
DosSetFileInfo	( unsigned short FileHandle,
				  unsigned short InfoLevel,
				  DOSFILESTATUS far *PtrFileStatus,
				  unsigned short InfoBufferLength )
{
	USHORT err ;

	if (InfoLevel != 1) return ERROR_INVALID_FUNCTION ;

	if (InfoBufferLength < sizeof(DOSFILESTATUS))
		return ERROR_BUFFER_OVERFLOW ;

	long oldpos, newend ;
	USHORT done ;

	err = DosChgFilePtr(FileHandle, 0L, 1, &oldpos) ;		// SEEK_CUR

	if (err) return err ;

	DosChgFilePtr(FileHandle,
					PtrFileStatus->cbFile, 0, &newend) ;	// SEEK_SET
	DosDosWrite(FileHandle, 0, 0, &done) ;					// Truncate
	DosChgFilePtr(FileHandle, oldpos, 0, &oldpos) ;			// SEEK_SET

	//
	//	Do the file time AFTER the Truncation because on FAT filesystems
	//	a truncate updates the timestamp !
	//
	err = DosDosSetFileTime (FileHandle,
							*(USHORT far *)&PtrFileStatus->fdateLastWrite,
							*(USHORT far *)&PtrFileStatus->ftimeLastWrite ) ;

	if (err) return err ;

	//
	// We cannot set file attributes under DOS by file handle.
	//
	return NO_ERROR ;
}
