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
//	Get cursor info
//
USHORT _APICALL
VioGetCurType ( VIOCURSORINFO far *pcinfo,
				unsigned short )
{
	_AX = 0x0300 ;
	geninterrupt(0x10) ;
	pcinfo->yStart = _CH ;
	pcinfo->cEnd = _CL ;
	pcinfo->cx = 1 ;
	pcinfo->attr = (0 << 4) + 7 ;
	return NO_ERROR ;
}

#pragma argsused
//
//	Set cursor info
//
USHORT _APICALL
VioSetCurType ( VIOCURSORINFO far *pcinfo,
				unsigned short )
{
	_AX = 0x0100 ;
	_CH = pcinfo->yStart ;
	_CL = pcinfo->cEnd ;
	geninterrupt(0x10) ;
	return NO_ERROR ;
}

#pragma argsused
//
//	Get cursor position
//
USHORT _APICALL
VioGetCurPos  ( unsigned short far *RowPtr,
				unsigned short far *ColPtr,
				unsigned short VioHandle )
{
	unsigned short pos = VioDosGetCurPos() ;
	*RowPtr = pos >> 8 ;
	*ColPtr = pos & 0xFF ;
	return NO_ERROR ;
}

#pragma argsused
//
//	Set cursor position
//
USHORT _APICALL
VioSetCurPos  ( unsigned short Row,
				unsigned short Col,
				unsigned short VioHandle )
{
	VioDosSetCurPos ((Row << 8) + (Col & 0xFF)) ;
	return NO_ERROR ;
}
