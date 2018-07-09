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

#define ISLEAP(yr)	( ((yr)%4==0) && ( ((yr)%100!=0) || ((yr)%400==0) ) )

//
//	Return the day of the week for a given year/month/day
//
static USHORT pascal
DosGetWeekDay ( unsigned int year,
				unsigned char mon,
				unsigned char mday,
				unsigned char far *PtrWeekDay )
{
	static const unsigned char month[] = { 1,4,4, 0,2,5, 0,3,6, 1,4,6 };

#define wday	(*PtrWeekDay)

	// This algorithm from Martin Gardner's column in Scientific American
	//
	// This algorithm relies on the fact that Jan 01 1900 was a Sunday
	// (Day 1) and counts onward from there.  It uses a fast tabular method
	// for the months and bit shifting for the years.  To avoid doing
	// division until the last possible moment, we do not work modulo 7.
	// Since the largest accumulated value (Dec 31 2099) is only 321, which
	// is well within INT_MAX, we need not worry about integer overflow.

	wday = 0;
	while (year > 100) {
		if ((year / 100) % 4 != 0)
			wday -= 2;		// Centuries beginning with leap years lose only
		else				// one weekday over the whole century.  Those not
			wday -= 1;		// so beginning lose two.
		year -= 100;
	}
	wday += year + (year >> 2) + month[mon-1] + mday;
	if (ISLEAP(year) && mon < 2) wday--;

	// Sneaky trick to avoid division.  In any base n, the remainder when
	// a number M is divided by (n-1) is equal to the sum of the digits of
	// M (base n) modulo (n-1).

	while (wday > 7)
		wday = (wday & 7) + ((wday & ~7) >> 3);

	// Standard C library dictates Sunday = 0 whereas this algorithm says
	// Sunday = 1
	--wday ;

#undef wday

	return 0 ;
}

//
//	Retrive current date and time
//
USHORT _APICALL
DosGetDateTime ( DOSDATETIME far *PtrDateTime )
{
	PtrDateTime->timezone = 0 ;		// Assume GMT

	_AH = 0x2a ;
	Dos3Call() ;
	PtrDateTime->year = _CX ;
	PtrDateTime->month = _DH ;
	PtrDateTime->day = _DL ;

	_AH = 0x2c ;
	Dos3Call() ;
	PtrDateTime->hours = _CH ;
	PtrDateTime->minutes = _CL ;
	PtrDateTime->seconds = _DH ;
	PtrDateTime->hundredths = _DL ;

	return DosGetWeekDay ( PtrDateTime->year - 1900,
						   PtrDateTime->month,
						   PtrDateTime->day,
						  &PtrDateTime->weekday ) ;
}
