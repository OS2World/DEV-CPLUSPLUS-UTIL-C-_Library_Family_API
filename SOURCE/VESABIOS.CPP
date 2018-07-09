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

#pragma saveregs
//
//	Return the current screen mode and number of columns
//
USHORT _APICALL
VioDosScreenMode ( void )
{
	_AH = 0x0f ;
	geninterrupt(0x10) ;
	return _AX ;
}

#pragma saveregs
//
//	Return the current cursor position (Row << 8) + Col ;
//
USHORT _APICALL
VioDosGetCurPos ( void )
{
	_AX = 0x0300 ;
	_BH = 0 ;
	geninterrupt(0x10) ;
	return _DX ;
}

#pragma saveregs
//
//	Set the current cursor position (Row << 8) + Col ;
//
void _APICALL
VioDosSetCurPos ( unsigned short pos )
{
	_AX = 0x0200 ;
	_BH = 0 ;
	_DX = pos ;
	geninterrupt(0x10) ;
}

struct VESAModeInfo {
	WORD ModeAttributes ;
	BYTE WinAAttribtes ;
	BYTE WinBAttribtes ;
	WORD WinGranularity ;
	WORD WinSize ;
	WORD WinASegment ;
	WORD WinBSegment ;
	LONG WinFuncPtr ;
	WORD BytesPerScanLine ;

	// Optional, provided if ModeAttributes & 0x0002 is true

	WORD XResolution ;
	WORD YResolution ;
	BYTE XCharSize ;
	BYTE YCharSize ;
	BYTE NumberOfPlanes ;
	BYTE BitePerPixel ;
	BYTE NumberOfBanks ;
	BYTE MemoryModel ;
	BYTE BankSize ;
	BYTE NumberOfImagePages ;
	BYTE Reserved ;
	BYTE Reserved2[225] ;
} ;

//
//	Construct a far pointer to text memory, or return 0:0 for graphics mode
//
unsigned short far * _APICALL
VioDosPointer	( unsigned short Row,
				  unsigned short Col )
{
	unsigned short mode = VioDosScreenMode () ;
	unsigned short width = mode >> 8 ;
	unsigned short seg ;

	mode &= 0xFF ;

	//
	//	Modes are excluded from SVGA processing if they *only* represent
	//	text modes.  Some VGA modes may be either text or graphics modes
	//	according to the make of VGA BIOS.  In which case we check for
	//	Super VGA, and if it is not present, we force the BIOS calls to be
	//	used instead of direct-to-memory.
	//
	if ( (mode&0x7F) > 0x03 && (mode&0x7F) != 0x07 &&
		mode != 0x2a && mode != 0x4d && mode != 0x4e && mode != 0x4f ) {
		static struct VESAModeInfo far SVGABuf ;

		//
		//	Test for SVGA BIOS
		//
		_AX = 0x4F03 ;						// Return current VESA mode
		geninterrupt(0x10) ;
		if (_AL != 0x4F || _AH != 0x00)		// Unsupported, or call failed
			return (unsigned short far *)MK_FP(0, 0) ;

		_CX = _BX ;							// Video mode
		_ES = FP_SEG(&SVGABuf) ;
		_DI = FP_OFF(&SVGABuf) ;
		_AX = 0x4F01 ;						// Return VESA mode information
		if (_AL != 0x4F || _AH != 0x00)		// Unsupported, or call failed
			return (unsigned short far *)MK_FP(0, 0) ;

		if ( SVGABuf.ModeAttributes & 0x0010 )
			return (unsigned short far *)MK_FP(0, 0) ;	// Graphics mode

		if ( SVGABuf.ModeAttributes & 0x0002 )	// Extended information
			width = SVGABuf.XResolution ;

		seg = 0xB800 ;

	} else

		seg = ((mode&0x7F) == 0x07) ? 0xB000 : 0xB800 ;

	return (unsigned short far *)MK_FP(seg, ( (Row * width) + Col ) << 1 ) ;
}