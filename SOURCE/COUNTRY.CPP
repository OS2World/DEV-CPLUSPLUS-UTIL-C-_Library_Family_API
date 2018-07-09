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
DosGetCtryInfo ( USHORT Buffersize,
				 DOSCOUNTRYCODE far *PtrCountry,
				 DOSCOUNTRYINFO far *PtrCountryInfo,
				 USHORT far * PtrInfoSize )
{
	struct {
		BYTE InfoID;
		WORD InfoLen;
		DOSCOUNTRYINFO Info;
	} DosInfo ;
	UINT Country  = PtrCountry->country ;
	UINT CodePage = PtrCountry->codepage ;

	_ES = FP_SEG(&DosInfo) ;
	_DI = FP_OFF(&DosInfo) ;
	_DX = Country ? Country : -1 ;
	_BX = CodePage ? CodePage : -1 ;
	_CX = Buffersize + 3 ;
	_AX = 0x6501 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX;

	*PtrInfoSize = _CX ;

	unsigned char far *src = (unsigned char far *)&DosInfo.Info ;
	unsigned char far *dst = (unsigned char far *)PtrCountryInfo ;

	while (Buffersize--)
		*dst++ = *src++ ;

	if (!Country) PtrCountry->country = DosInfo.Info.country ;
	if (!CodePage) PtrCountry->codepage= DosInfo.Info.codepage ;

	return NO_ERROR ;
}