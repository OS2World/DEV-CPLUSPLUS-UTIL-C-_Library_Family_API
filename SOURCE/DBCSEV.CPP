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
//	Obtain internationalisation information
//
USHORT _APICALL
DosGetDBCSEv ( USHORT BufferSize,
			   DOSCOUNTRYCODE far *PtrCountry,
			   char far *Buffer )
{
	UINT Country  = PtrCountry->country ;
	UINT CodePage = PtrCountry->codepage ;

	_ES = FP_SEG(Buffer) ;
	_DI = FP_OFF(Buffer) ;
	_DX = Country ? Country : -1 ;
	_BX = CodePage ? CodePage : -1 ;
	_CX = BufferSize ;
	_AX = 0x6507 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX;

	return NO_ERROR ;
}
