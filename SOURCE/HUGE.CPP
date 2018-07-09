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
//	Get shift count for huge segments
//
USHORT _APICALL
DosGetHugeShift	( unsigned short far *PtrShift )
{
	*PtrShift = 0x1000 ;
	return NO_ERROR ;
}

//
//	Allocate a huge segment set
//
USHORT _APICALL
DosAllocHuge ( USHORT NumSegs,
			   USHORT PartialSeg,
			   USHORT far *PtrSelector,
			   USHORT MaxNumSegs,
			   USHORT AllocFlags )
{
	if (NumSegs > 0xF) return ERROR_NOT_ENOUGH_MEMORY ; // Limit to 1Mb

	if (PartialSeg) NumSegs-- ;

	// Round up to next paragraph

	if (PartialSeg & 0xF)
		_BX = (NumSegs << 12) + (PartialSeg >> 4) + 1 ;
	else
		_BX = (NumSegs << 12) + (PartialSeg >> 4) ;

	_AH = 0x48 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX;
	*PtrSelector = _AX ;
	return NO_ERROR ;
}

//
//	Re-Allocate a huge segment set
//
USHORT _APICALL
DosReallocHuge ( USHORT NumSegs,
				 USHORT PartialSeg,
				 USHORT Selector)
{
	if (NumSegs > 0xF) return ERROR_NOT_ENOUGH_MEMORY ; // Limit to 1Mb

	if (PartialSeg) NumSegs-- ;

	// Round up to next paragraph

	if (PartialSeg & 0xF)
		_BX = (NumSegs << 12) + (PartialSeg >> 4) + 1 ;
	else
		_BX = (NumSegs << 12) + (PartialSeg >> 4) ;

	_ES = Selector ;
	_AH = 0x4A ;
	Dos3Call() ;
	return (_FLAGS & 0x0001) ? _AX : NO_ERROR;
}
