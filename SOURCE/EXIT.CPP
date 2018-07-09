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
//	Exit thread/process
//
void  _APICALL
DosExit		( unsigned short Terminate, unsigned short ECode )
{
	DosDosRestoreVectors() ;
	_AX = ECode ;
	_AH = 0x4C ;
	Dos3Call() ;
}
