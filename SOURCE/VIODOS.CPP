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
//	Test for EGA/VGA
//
USHORT _APICALL
VioDosGetEGASettings	( USHORT far *ds, USHORT far *ms )
{
	USHORT dipswitch, memsize ;

	_AX = 0x1200;
	_BX = 0xff10;
	geninterrupt(0x10);
	dipswitch = _CL ;
	memsize = _BL ;
	if ( memsize == 0x10 || dipswitch > 11 ) return ERROR_VIO_MODE ;
	*ds = dipswitch ;
	*ms = memsize ;
	return NO_ERROR ;
}

#pragma saveregs
//
//	Test for VGA only
//
USHORT _APICALL
VioDosGetDCC	( USHORT far *active )
{
	_AX = 0x1A00;
	geninterrupt(0x10);
	if (_AL != 0x1A) return ERROR_VIO_MODE ;
	_AX = _BX ;
	*active = _AX ;				// Changes ES:BX
	return NO_ERROR ;
}

