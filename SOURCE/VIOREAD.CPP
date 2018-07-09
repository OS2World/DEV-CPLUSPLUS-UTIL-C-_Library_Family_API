//
//	  *******************************************************************
//		JdeBP C++ Library Routines 	 	General Public Licence v1.00
//			Copyright (c) 1991,1992	 Jonathan de Boyne Pollard
//	  *******************************************************************
//
// Part of FamAPI.LIB
//

#include "famapi.h"
#include "vio.h"

#pragma argsused
//
//	Read a string of character/attribute pairs
//
USHORT _APICALL
VioReadCellStr	  ( char far *PtrCellStr,
					unsigned short far *PtrBufferLength,
					unsigned short Row,
					unsigned short Col,
					unsigned short VioHandle )
{
	unsigned short far *screen = VioDosPointer(Row, Col) ;
	unsigned short StringLen = *PtrBufferLength ;

	++StringLen;		// Round upwards
	StringLen >>= 1 ;	// Convert byte count to word count
	if (!screen) {
		unsigned short savepos = VioDosGetCurPos() ;
		unsigned short pos = (Row << 8) + (Col & 0xFF) ;

		while (StringLen--) {
			VioDosSetCurPos (pos++) ;
			_AH = 0x08 ;
			_BH = 0x00 ;
			geninterrupt(0x10) ;		// Read character at cursor
			*((unsigned short far *)PtrCellStr)++ = _AX ;
		}
		VioDosSetCurPos (savepos) ;
	} else {
		while (StringLen--)
			*((unsigned short far *)PtrCellStr)++ = *screen++ ;
	}

	return NO_ERROR ;
}

#pragma argsused
//
//	Read a string of characters
//
USHORT _APICALL
VioReadCharStr	  ( char far *PtrCharStr,
					unsigned short far *PtrBufferLength,
					unsigned short Row,
					unsigned short Col,
					unsigned short VioHandle)
{
	unsigned short far *screen = VioDosPointer(Row, Col) ;
	unsigned short StringLen = *PtrBufferLength ;

	if (!screen) {
		unsigned short savepos = VioDosGetCurPos() ;
		unsigned short pos = (Row << 8) + (Col & 0xFF) ;

		while (StringLen--) {
			VioDosSetCurPos (pos++) ;
			_AH = 0x08 ;
			_BH = 0x00 ;
			geninterrupt(0x10) ;		// Read character at cursor
			*(PtrCharStr)++ = _AL ;
		}
		VioDosSetCurPos (savepos) ;
	} else {
		while (StringLen--) {
			*(PtrCharStr)++ = *screen ;
			screen += 2;
		}
	}

	return NO_ERROR ;
}
