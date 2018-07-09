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
//	Extended open (DOS 4.0 and later)
//
USHORT _APICALL
DosDosXOpen	( const char far *FileName,
			  unsigned short far *FileHandle,
			  unsigned int far *ActionTaken,
			  unsigned int Attribute,
			  unsigned int OpenFlags,
			  unsigned int OpenMode)
{
	asm push ds ;
	_DS = FP_SEG(FileName) ;
	_SI = FP_OFF(FileName) ;
	_DX = OpenFlags & 0xff ;
	_CX = Attribute ;
	_BX = OpenMode ;
	_AX = 0x6c00 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ;
	*FileHandle = _AX ;
	*ActionTaken = _CX ;
	return NO_ERROR ;
}
