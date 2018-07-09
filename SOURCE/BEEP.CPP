//
//    *******************************************************************
//      JdeBP C++ Library Routines      General Public Licence v1.00
//          Copyright (c) 1991,1992  Jonathan de Boyne Pollard
//    *******************************************************************
//
// Part of FamAPI.LIB
//

#include "famapi.h"
#include "dosdos.h"

/* The speaker port (bits 2-7 used elsewhere) */
#define SPEAKER 0x61

//
//  enable or disable the speaker, and tones sent to it
//
static void
speaker ( int on, int speak )
{
    int val = inp(SPEAKER);

    val = (val & ~3) | (speak ? 2 : 0) | (on ? 1 : 0);
    outp(SPEAKER, val);
}

//
//  Make a sound
//
void _APICALL
DosBeep ( unsigned short frequency, unsigned short duration )
{
    unsigned short count = 0 ;
    long remainder = 1193180L ;

    //
    // Borland C++ long division would call a helper routine in the library.
    //
    while ((remainder -= (long)frequency) > 0) count ++ ;

    outp(0x43, 0xb6);
    outp(0x42, count % 256);
    outp(0x42, count / 256);

    speaker(1, 1);
    DosDosSleep(duration) ;
    speaker(1, 0);
}
