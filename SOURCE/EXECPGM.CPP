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
//	Execute a program
//
USHORT _APICALL
DosExecPgm	( char far *PtrFailName,
			  unsigned short LengthFailName,
			  unsigned short ExecFlags,
			  char far *PtrArgs,
			  char far *PtrEnv,
			  RESULTCODES far *PtrResults,
			  char far *PtrPgmName)
{
	return ERROR_BAD_EXE_FORMAT ;
}
