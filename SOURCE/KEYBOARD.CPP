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
//	Read from the keyboard
//
USHORT _APICALL
KbdCharIn ( KBDKEYINFO far * pkinfo,
			unsigned short nowait,
			unsigned short KbdHandle)
{
	if (nowait) {
		_AH = 0x11 ;						// BIOS poll keyboard
		geninterrupt(0x16) ;
		if (_FLAGS & 0x0040) {				// If ZF is set
			pkinfo->fbStatus &= ~0x0040 ;	// No character received
			return 0 ;						// But still success
		}
	}

	_AH = 0x10 ;							// BIOS read keyboard
	geninterrupt(0x16) ;
	pkinfo->chChar = _AL ;
	pkinfo->chScan = _AH ;
	_AH = 0x12 ;
	geninterrupt(0x16) ;					// BIOS get shift state
	pkinfo->fsState = _AX ;
	pkinfo->fbStatus |= 0x0040 ;			// Final character received

	return 0 ;
}

#pragma argsused
//
//	Read from the keyboard
//
USHORT _APICALL
KbdPeek ( KBDKEYINFO far * pkinfo,
		  unsigned short KbdHandle)
{
	unsigned short ax ;
	_AH = 0x11 ;							// BIOS poll keyboard
	geninterrupt(0x16) ;
	ax = _AX ;
	if (_FLAGS & 0x0040) {					// If ZF is set
		pkinfo->fbStatus &= ~0x0040 ;		// No character received
		return 0 ;							// But still success
	}
	_AX = ax ;
	pkinfo->chChar = _AL ;
	pkinfo->chScan = _AH ;
	_AH = 0x12 ;
	geninterrupt(0x16) ;					// BIOS get shift state
	pkinfo->fsState = _AL ;
	pkinfo->fbStatus |= 0x0040 ;			// Final character received

	return 0 ;
}

#pragma argsused
//
//	Flush the keyboard buffer
//
USHORT _APICALL
KbdFlushBuffer ( unsigned short KbdHandle)
{
	do {
		_AH = 0 ;
		geninterrupt(0x16) ;
	} while (_FLAGS & 0x4000) ;

	return 0 ;
}
