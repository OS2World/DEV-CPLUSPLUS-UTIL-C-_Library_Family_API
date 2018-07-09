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
//	Dummy open function
//
USHORT _APICALL
KbdOpen	( USHORT far *KbdHandle )
{
	*KbdHandle = 0 ;
	return NO_ERROR ;
}

#pragma argsused
//
//	Dummy close function
//
USHORT _APICALL
KbdClose ( USHORT KbdHandle )
{
	return NO_ERROR ;
}

#pragma argsused
//
//	Dummy get focus
//
USHORT _APICALL
KbdGetFocus ( USHORT NoWait,
			  USHORT KbdHandle )
{
	return NO_ERROR ;
}

#pragma argsused
//
//	Dummy free focus
//
USHORT _APICALL
KbdFreeFocus ( USHORT KbdHandle )
{
	return NO_ERROR ;
}