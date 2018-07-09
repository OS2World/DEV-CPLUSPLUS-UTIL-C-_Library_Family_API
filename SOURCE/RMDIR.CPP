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
//	Remove a directory
//
USHORT _APICALL
DosRmdir	( const char far *DirName, unsigned long Reserved )
{
	asm push ds ;
	_DS = FP_SEG(DirName) ;
	_DX = FP_OFF(DirName) ;
	_AH = 0x3a ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) {
		if (_AX != ERROR_CURRENT_DIRECTORY) return _AX ;

		//
		// Special case for wildcards
		//
		while (*DirName)
			if (*DirName == '?' || *DirName == '*')
				return ERROR_PATH_NOT_FOUND ;
			else
				DirName++ ;
		return ERROR_CURRENT_DIRECTORY;
	}
	return NO_ERROR ;
}
