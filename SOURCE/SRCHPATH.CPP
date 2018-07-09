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
//	Search path for a file (allowing wildcards)
//
//	NOTE: We must not use the str...() functions from the C++ library.
//
USHORT _APICALL
DosSearchPath	( unsigned short Search,
				  const char far *PtrPath,
				  const char far *PtrFileName,
				  unsigned char far *PtrBuffer,
				  unsigned short BufferLength )
{
	const char far *TheEnv ;

	if (Search & 0x0002) {
		USHORT err = DosScanEnv(PtrPath, &TheEnv) ;
		if (err) return err ;
	} else
		TheEnv = PtrPath ;

	char namebuf[256] ;
	struct DOSFind_t find ;

	if (Search & 0x0001) {
		const char far *PtrName = PtrFileName ;
		char far *PtrBuf = namebuf ;

		*PtrBuf++ = '.';
		*PtrBuf++ = '\\';

		// strcat (namebuf, PtrFileName)
		while (PtrBuf < &namebuf[sizeof(namebuf)] && *PtrName)
			*PtrBuf++ = *PtrName++;

		*PtrBuf++ = '\000';

		if (!DosDosFindFirst ((const char far *)namebuf,
							  _A_NORMAL|_A_SUBDIR|_A_HIDDEN|_A_SYSTEM,
							  &find)) {
			// strncpy(PtrBuffer, namebuf, BufferLength)
			PtrBuf = namebuf ;
			while (BufferLength--)
				if (*PtrBuffer = *PtrBuf)
					PtrBuffer++, PtrBuf++;
				else
					break ;

			return *PtrBuf ? ERROR_BUFFER_OVERFLOW : NO_ERROR ;
		}

	}

	while (*TheEnv) {
		const char far *PtrName = PtrFileName ;
		char far *PtrBuf = namebuf ;

		while (PtrBuf < &namebuf[sizeof(namebuf)] && *TheEnv && *TheEnv != ';')
			*PtrBuf++ = *TheEnv++ ;

		if (PtrBuf > namebuf
		&&  PtrBuf < &namebuf[sizeof(namebuf)]
		&&  (PtrBuf[-1] != '\\')
		&&  (PtrBuf[-1] != '/'))
			*PtrBuf++ = '\\';

		// strcat (namebuf, PtrFileName)
		while (PtrBuf < &namebuf[sizeof(namebuf)] && *PtrName)
			*PtrBuf++ = *PtrName++;

		*PtrBuf++ = '\000';

		if (!DosDosFindFirst ((const char far *)namebuf,
							  _A_NORMAL|_A_SUBDIR|_A_HIDDEN|_A_SYSTEM,
							  &find)) {
			// strncpy(PtrBuffer, namebuf, BufferLength)
			PtrBuf = namebuf ;
			while (BufferLength--)
				if (*PtrBuffer = *PtrBuf)
					PtrBuffer++, PtrBuf++;
				else
					break ;

			return *PtrBuf ? ERROR_BUFFER_OVERFLOW : NO_ERROR ;
		}

		while (*TheEnv && *TheEnv != ';') TheEnv++ ;

		if (*TheEnv == ';') TheEnv++ ;
	}

	return ERROR_FILE_NOT_FOUND ;
}
