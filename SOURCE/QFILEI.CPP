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
DosQFileInfo	( unsigned short FileHandle,
				  unsigned short InfoLevel,
				  DOSFILESTATUS far *PtrFileStatus,
				  unsigned short InfoBufferLength )
{
	FDATE date ;
	FTIME time ;
	USHORT err ;

	if (InfoLevel != 1) return ERROR_INVALID_FUNCTION ;

	if (InfoBufferLength < sizeof(DOSFILESTATUS))
		return ERROR_BUFFER_OVERFLOW ;

	err = DosDosQFileTime (FileHandle, (USHORT far *)&date, (USHORT far *)&time) ;

	if (err) return err ;

	PtrFileStatus->fdateCreation =
	PtrFileStatus->fdateLastAccess =
	PtrFileStatus->fdateLastWrite = date ;
	PtrFileStatus->ftimeCreation =
	PtrFileStatus->ftimeLastAccess =
	PtrFileStatus->ftimeLastWrite = time ;

	long oldpos ;

	err = DosChgFilePtr(FileHandle, 0L, 1, &oldpos) ;		// SEEK_CUR

	if (err) return err ;

	DosChgFilePtr(FileHandle, 0L, 2,
					(long far *)&PtrFileStatus->cbFile) ;	// SEEK_END
	DosChgFilePtr(FileHandle, oldpos, 0, &oldpos) ;			// SEEK_SET

	// Round up to 4K multiple
	PtrFileStatus->cbFileAlloc = (PtrFileStatus->cbFile + 0x0FFF) & ~0x0FFF ;

	//
	// We cannot obtain this under DOS, so make a reasonable assumption.
	//
	PtrFileStatus->attrFile = _A_NORMAL ;

	return NO_ERROR ;
}
