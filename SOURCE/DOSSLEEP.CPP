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
//	Read the value of timer 0, faking an ascending counter
//
static unsigned short
timer	( void )
{
	disable() ;
	outportb (0x43, 0) ;
	int val = ~(inportb (0x40) + ((unsigned short)inportb(0x40) << 8)) ;
	enable() ;
	return val ;
}

//
//	Check the timer for mode 2 or mode 3 and return the delay multiplier
//
//	Mode 3 decrements by 2, so the timer will never have an odd value
//
static unsigned short
init_timer	( void )
{
	int i ;

	for (i = 0; i < UCHAR_MAX; i++) {
		if (~timer() & 1)
			return 1193U ;
	}

	return 1193U << 1 ;
}

//
//	wait a specific interval
//
void _APICALL
DosDosSleep	( unsigned short milliseconds )
{
	static unsigned short multiplier = 0UL ;

	if (!multiplier) multiplier = init_timer() ;

	unsigned short then = timer() ;
	unsigned short now ;
	unsigned long when = then ;

	// Do not do long multiplication because Borland C++ does not support
	// that inline, using library calls instead.

	while (milliseconds--) when += multiplier ;

	while ((now = timer()) < when) {
		if (now < then) {
			if (when > USHRT_MAX)
				when -= USHRT_MAX + 1UL ;
			else
				break ;
		}
		then = now;
	}
}