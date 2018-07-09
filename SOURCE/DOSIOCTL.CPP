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
//	I/O Control
//
USHORT _APICALL
DosDosDevIOCtl	( int fd,
				  unsigned short func,
				  void far * parms,
				  unsigned short data,
				  unsigned short far *retval)
{
	asm push ds ;
	_DS = FP_SEG(parms) ;
	_DX = FP_OFF(parms) ;
	_BX = fd ;
	_CX = data ;
	_AX = func ;
	_AH = 0x44 ;
	Dos3Call() ;
	asm pop ds ;
	if (_FLAGS & 0x0001) return _AX ;
	*retval = (func == 0) ? _DX : _AX ;
	return NO_ERROR ;
}
