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
//	Stop the process for a while using the BIOS timer
//
void _APICALL
DosDosSleep18 ( unsigned long delay )
{
	volatile unsigned long far *biostime = (volatile unsigned long far *)MK_FP(0x40, 0x6c);

	while (delay > 0) {
		unsigned long now = *biostime;
		while (*biostime == now) ;
		delay--;
	}
}
