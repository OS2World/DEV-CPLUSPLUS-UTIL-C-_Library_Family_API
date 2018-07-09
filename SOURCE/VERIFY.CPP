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
//	Query verification status
//
USHORT _APICALL
DosQVerify	( unsigned short far *PtrVerifyFlag )
{
	_AH = 0x54 ;
	Dos3Call() ;
	*PtrVerifyFlag = _AL ;
	return NO_ERROR ;
}

//
//	Set verification status
//
USHORT _APICALL
DosSetVerify	( unsigned short VerifyFlag )
{
	_DL = 0 ;
	_AX = VerifyFlag ;
	_AH = 0x2E ;
	Dos3Call() ;
	return NO_ERROR ;
}
