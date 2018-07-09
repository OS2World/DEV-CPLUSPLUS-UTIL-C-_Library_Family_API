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
//	Stop the process for a while
//
void _APICALL
DosSleep ( unsigned long delay )
{
	while (delay) {
		if (delay > USHRT_MAX) {
			DosDosSleep ( USHRT_MAX ) ;
			delay -= USHRT_MAX ;
		} else {
			DosDosSleep ( delay ) ;
			delay = 0 ;
		}
	}

	//
	// Yield current timeslice under Windoze or OS/2
	//
	// Under native OS/2 a DosSleep(0) relinquishes only if a higher
	// priority thread is waiting, and a DosSleep(1) always relinquishes
	// to waiting threads.  Whatever happens, the CPU is yielded.
	//
	// This code, as does the rest of the library, assumes DOS 3.3 or
	// later.  Earlier versions of DOS have a 0:0 INT 2F vector.
	//
	_AX = 0x1680 ;
	geninterrupt(0x2f) ;
}