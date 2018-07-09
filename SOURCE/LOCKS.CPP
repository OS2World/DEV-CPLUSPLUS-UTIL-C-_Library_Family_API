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

#define OFFSET	0
#define LENGTH	1

//
//	Control record access
//
USHORT _APICALL
DosFileLocks	( unsigned short FileHandle,
				  long far *PtrUnlockRange,
				  long far *PtrLockRange )
{
	if (PtrUnlockRange) {
		long FileOffset = PtrUnlockRange[OFFSET] ;
		long RangeLength = PtrUnlockRange[LENGTH] ;
		_DI = (USHORT)(RangeLength & 0xFFFF) ;
		_SI = (USHORT)(RangeLength >> 16) ;
		_DX = (USHORT)(FileOffset & 0xFFFF) ;
		_CX = (USHORT)(FileOffset >> 16) ;
		_BX = FileHandle ;
		_AX = 0x5C01 ;
		Dos3Call() ;
		if (_FLAGS & 0x0001) return _AX ;
	}
	if (PtrLockRange) {
		long FileOffset = PtrLockRange[OFFSET] ;
		long RangeLength = PtrLockRange[LENGTH] ;
		_DI = (USHORT)(RangeLength & 0xFFFF) ;
		_SI = (USHORT)(RangeLength >> 16) ;
		_DX = (USHORT)(FileOffset & 0xFFFF) ;
		_CX = (USHORT)(FileOffset >> 16) ;
		_BX = FileHandle ;
		_AX = 0x5C00 ;
		Dos3Call() ;
		if (_FLAGS & 0x0001) return _AX ;
	}
	return NO_ERROR ;
}
