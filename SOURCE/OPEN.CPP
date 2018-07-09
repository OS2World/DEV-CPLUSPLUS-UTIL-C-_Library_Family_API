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

#define	O_ANYSHARE	0xF0F0

//
//	Open a file
//
//
// BUG NOTE:  NewSize parameter is ignored
//
// We do not use Microsoft C's method of DosDosOpen()ing the file and then
// checking for truncation flags, because on a Novell network with SEARCH MODE
// set to anything other than 2, this may overwrite files in \PUBLIC.
// Instead we use DosQFileMode, which Netware does not apply SEARCH MODE to.
//
// The down side is that DosQFileMode("*.*",...) behaves in a non-MS-DOS
// fashion under Netware.  Swings and roundabouts ...
//
USHORT _APICALL
DosOpen	( const char far *FileName,
		  unsigned short far *PtrFileHandle,
		  unsigned int far *PtrActionTaken,
		  unsigned long NewSize,
		  unsigned int Attribute,
		  unsigned int OpenFlags,
		  unsigned int OpenMode,
		  unsigned long Reserved)
{
	unsigned short Version ;

	DosGetVersion (&Version) ;

	if (Version > 0x3FF) {
		//
		//	DOS 4.00 or later, use the extended open call
		//
		return DosDosXOpen(FileName, PtrFileHandle, PtrActionTaken,
						   Attribute, OpenFlags, OpenMode) ;
	} else {
		//
		//	DOS 3.xx, use the ordinary open/create calls
		//
		USHORT err ;
		USHORT attr ;

		//
		// First obtain attributes of existing file, if any.
		//
		err = DosQFileMode(FileName, &attr, 0);

		if (err) {
			//
			// If the file does not exist, fail if necessary.
			//
			if (err != ERROR_FILE_NOT_FOUND) return err ;

			*PtrActionTaken = 0x0000 ;		// File did not exist

			if (!(OpenFlags & 0x0010)) return ERROR_OPEN_FAILED ;

			//
			// Now create the file.  If sharing has not been requested
			// then we can finalise here and now.  Otherwise we need
			// to DosClose and then DosDosOpen the file again.  In the
			// latter case, we create the file _A_NORMAL otherwise
			// creating a readonly file for read-write access would not
			// work.
			//
			attr = (OpenMode & O_ANYSHARE) ? _A_NORMAL : Attribute ;

			err = DosDosCreate(FileName, attr, PtrFileHandle);
			if (err) return err;

			*PtrActionTaken |= 0x0002 ;		// File was created

			if (!(OpenFlags & O_ANYSHARE)) return NO_ERROR ;

			DosClose(*PtrFileHandle);

		} else {
			//
			// If the file exists, fail if necessary.
			// Otherwise, drop through to DosDosOpen.
			//
			// Under Netware, we will have reached here if the name contained
			// enough wildcards to match a file in the directory.  Either
			// the call will fail here, or in DosDosOpen further on. Since
			// the call is nonsense, the error return can acceptably
			// be nonsense also.
			//
			*PtrActionTaken = 0x0001 ;	// File existed

			if (!(OpenFlags & 0x000F)) return ERROR_OPEN_FAILED ;

		}

		err = DosDosOpen(FileName, OpenMode, PtrFileHandle);

		if (err) return err;

		//
		// If the file is not a device, then truncate it if need be.
		//
		USHORT type = 0;

		err = DosDosDevIOCtl(*PtrFileHandle, 0x00, NULL, 0, &type);

		if (!err && !(type & 0x80U) && (OpenFlags & 0x0002)) {
			USHORT len;

			*PtrActionTaken |= 0x0002 ;		// File was truncated

			DosDosWrite(*PtrFileHandle, NULL, 0, &len);
		}

		//
		// If we have re-opened a file which we created with DosDosCreate, we
		// should now attempt to change it to the correct attributes.  Note
		// that Attribute could conceivably contain _A_HIDDEN, so we make the
		// test general.
		//
		if ((OpenFlags & 0x0010) && (Attribute != _A_NORMAL) && (OpenMode & O_ANYSHARE)) {
			DosSetFileMode(FileName, Attribute, 0L);
		}

		return NO_ERROR ;
	}
}
