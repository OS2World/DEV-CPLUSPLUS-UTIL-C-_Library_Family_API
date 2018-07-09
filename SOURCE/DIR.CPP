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
//	Make a directory
//
USHORT _APICALL
DosMkdir	(const char far *DirName, unsigned long Reserved)
{
	_DS = FP_SEG(DirName) ;
	_DX = FP_OFF(DirName) ;
	_AH = 0x39 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX; else return NO_ERROR ;
}

//
//	Change directory
//
USHORT _APICALL
DosChdir	(const char far *DirName)
{
	asm push ds ;
	_DS = FP_SEG(DirName) ;
	_DX = FP_OFF(DirName) ;
	_AH = 0x3B ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX; else return NO_ERROR ;
}