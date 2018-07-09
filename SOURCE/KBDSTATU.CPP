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

#pragma argsused
//
//	Change the keyboard status word
//
USHORT _APICALL
KbdSetStatus ( KBDINFO far *PtrKbdInfo, unsigned short KbdHandle )
{
	if (PtrKbdInfo->cb != sizeof(*PtrKbdInfo))
		return ERROR_KBD_INVALID_LENGTH ;

	if (PtrKbdInfo->fsMask & ~(0x04|0x01))
		return ERROR_KBD_INVALID_INPUT_MASK ;

	//	It is a pity that we have to fudge the value in directly.
	*(UCHAR far *)MK_FP(0x40, 0x17) = (UCHAR)PtrKbdInfo->fsState ;

	return NO_ERROR ;
}

#pragma argsused
//
//	Obtain the keyboard status word
//
USHORT _APICALL
KbdGetStatus ( KBDINFO far *PtrKbdInfo, unsigned short KbdHandle )
{
	if (PtrKbdInfo->cb != sizeof(*PtrKbdInfo))
		return ERROR_KBD_INVALID_LENGTH ;

	PtrKbdInfo->fsMask = 0 ;			// Everything is turned off
	PtrKbdInfo->chTurnAround = 0 ;		// Ignored in Family API
	PtrKbdInfo->fsInterim = 0 ;			// All interim support turned off
	_AH = 0x12 ;
	geninterrupt(0x16) ;				// BIOS get shift state
	PtrKbdInfo->fsState = _AX ;

	return NO_ERROR ;
}
