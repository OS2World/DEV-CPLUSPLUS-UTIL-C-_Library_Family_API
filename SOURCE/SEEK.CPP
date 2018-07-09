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
//	Move the file position
//
USHORT _APICALL
DosChgFilePtr ( unsigned short fd,
				long Pos,
				unsigned short Whence,
				long far * OldPos )
{
	_DX = (USHORT)((ULONG)Pos & 0xFFFF) ;
	_CX = (USHORT)((ULONG)Pos >> 16) ;
	_BX = fd ;
	_AX = Whence ;
	_AH = 0x42 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX ;
	((USHORT far *)OldPos)[0] = _AX ;	// Sneaky trick to prevent the
	((USHORT far *)OldPos)[1] = _DX ;	// compiler extending _AX into _DX.
	return NO_ERROR ;
}
