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
//	Scrolling Down
//
//	Known bugs : Does not take account of the character to use
//
USHORT _APICALL
VioScrollDn ( unsigned short TopRow,
			  unsigned short LeftCol,
			  unsigned short BotRow,
			  unsigned short RightCol,
			  unsigned short Lines,
			  unsigned char far *PtrCell,
			  unsigned short VioHanedle)
{
	volatile unsigned char far *BIOSrows = (volatile unsigned char far *)MK_FP(0x0040, 0x0084) ;

	if (Lines == 0) return ERROR_VIO_INVALID_PARMS ;
	if (BotRow == (USHORT)-1) BotRow = *(BIOSrows) + 1 ;
	if (RightCol == (USHORT)-1) RightCol = VioDosScreenMode() >> 8 ;

	if (Lines == (USHORT)-1) Lines = 0 ;	// Translate OS/2 to BIOS convention

	_BH = PtrCell[1] ;				// Attribute
	_AH = 0x07 ;
	_AL = Lines ;
	_CH = TopRow ;
	_CL = LeftCol ;
	_DH = BotRow ;
	_DL = RightCol ;
	geninterrupt(0x10) ;
	return NO_ERROR ;
}

#pragma argsused
//
//	Scrolling Up
//
//	Known bugs : Does not take account of the character to use
//
USHORT _APICALL
VioScrollUp ( unsigned short TopRow,
			  unsigned short LeftCol,
			  unsigned short BotRow,
			  unsigned short RightCol,
			  unsigned short Lines,
			  unsigned char far *PtrCell,
			  unsigned short VioHandle)
{
	volatile unsigned char far *BIOSrows = (volatile unsigned char far *)MK_FP(0x0040, 0x0084) ;

	if (Lines == 0) return ERROR_VIO_INVALID_PARMS ;
	if (BotRow == (USHORT)-1) BotRow = *(BIOSrows) + 1 ;
	if (RightCol == (USHORT)-1) RightCol = VioDosScreenMode() >> 8 ;

	if (Lines == (USHORT)-1) Lines = 0 ;	// Translate OS/2 to BIOS convention

	_BH = PtrCell[1] ;				// Attribute
	_AH = 0x06 ;
	_AL = Lines ;
	_CH = TopRow ;
	_CL = LeftCol ;
	_DH = BotRow ;
	_DL = RightCol ;
	geninterrupt(0x10) ;
	return NO_ERROR ;
}

#pragma argsused
//
//	Scrolling Left
//
USHORT _APICALL
VioScrollLf ( unsigned short,
			  unsigned short,
			  unsigned short,
			  unsigned short,
			  unsigned short,
			  unsigned char far *,
			  unsigned short );

#pragma argsused
//
//	Scrolling Right
//
USHORT _APICALL
VioScrollRt ( unsigned short,
			  unsigned short,
			  unsigned short,
			  unsigned short,
			  unsigned short,
			  unsigned char far *,
			  unsigned short );