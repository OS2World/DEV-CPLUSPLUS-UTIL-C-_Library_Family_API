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
//	Write data to a file
//
USHORT _APICALL
DosWrite	( unsigned short fd,
			  void far *base,
			  unsigned short bufsize,
			  unsigned short far *done)
{
	// OS/2 does not truncate on zero length writes
	if (bufsize != 0)
		return DosDosWrite (fd, base, bufsize, done) ;
	else
		return NO_ERROR ;	// Even if file handle is bad
}
