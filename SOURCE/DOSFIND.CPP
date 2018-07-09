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
//	DOS Set DTA
//
void _APICALL
DosDosSetDTA ( const void far *DTA )
{
	asm push ds ;
	_DS = FP_SEG(DTA) ;
	_DX = FP_OFF(DTA) ;
	_AX = 0x1a00 ;
	Dos3Call() ;
	asm pop ds ;
}

//
//	DOS Find first occurrence of a file
//
USHORT _APICALL
DosDosFindFirst ( const char far *FileName,
				  unsigned short Attr,
				  struct DOSFind_t far *find)
{
	DosDosSetDTA(find) ;
	asm push ds ;
	_DS = FP_SEG(FileName) ;
	_DX = FP_OFF(FileName) ;
	_CX = Attr ;
	_AX = 0x4e00 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ; else return NO_ERROR ;
}

//
//	DOS Find next occurrence of a file
//
USHORT _APICALL
DosDosFindNext	( struct DOSFind_t far * find)
{
	DosDosSetDTA(find) ;
	_AX = 0x4f00 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX ; else return NO_ERROR ;
}
