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

//
//	Test for presence of 6845
//
static
int
test_6845 ( int addr )
{
	unsigned char prev, curr ;
	int delay = 0 ;

	outportb(addr, '\x0a');	// Write to CRTC register 10 -- cursor start line
	++addr;
	prev = inportb(addr);
	outportb(addr, '\xff');
	while (delay < 0x100) ++delay;
	curr = inportb(addr);
	outportb(addr, prev);
	return (curr == prev);
}

#pragma argsused
//
//	Get video configuration
//
//	Known bugs :	Does not return memory board size for VGA/CGA/MONO
//
USHORT _APICALL
VioGetConfig  ( unsigned short ConfigID,
				VIOCONFIGINFO far *PtrConfig,
				unsigned short VioHandle )
{
	enum testflags { VGA = 8, EGA = 4, CGA = 2, MONO = 1 } ;
	int test = VGA|EGA|CGA|MONO ;

	//
	//	Only the VGA BIOS distiguishes active and inactive displays.
	//	However it does not give PRIMARY or SECONDARY displays.
	//

	USHORT DCC ;		// VGA display combination code
#define	ACTIVE	0xff

	// Call VGA VBIOS -- Return card and monitor
	if (!VioDosGetDCC(&DCC)) {
		PtrConfig->adapter = PtrConfig->display = 0 ;
		switch (DCC & ACTIVE) {
			case 1:	PtrConfig->adapter = 0 ; break ;	// MDA/HGC card
			case 2: PtrConfig->adapter = 1 ; break ;	// CGA card
			case 4:
			case 5:	PtrConfig->adapter = 2 ; break ;	// EGA card
			case 7:
			case 8: PtrConfig->adapter = 3 ; break ;	// VGA card
		}
		switch (DCC & ACTIVE) {
			case 1:	PtrConfig->display = 0 ; break ;	// Monochrome
			case 2: PtrConfig->display = 1 ; break ;	// Colour
			case 4:	PtrConfig->display = 2 ; break ;	// HiRes EGA
			case 5:	PtrConfig->display = 0 ; break ;	// Monochrome
			case 7: PtrConfig->display = 3 ; break ;	// Analogue Monochrome
			case 8: PtrConfig->display = 4 ; break ;	// Analogue Colour
		}
		test &= ~(VGA|EGA|CGA) ;
		if (PtrConfig->adapter) test &= ~(MONO) ;
	}
	// Call EGA VBIOS -- Return DIP switch settings
	if (test & EGA) {
		USHORT dipswitch, memsize ;
		if (!VioDosGetEGASettings(&dipswitch, &memsize)) {
			switch (dipswitch) {
				case 0:
				case 1:
				case 2:
				case 3:		PtrConfig->display = 0 ; break ;	// Monochrome
				case 4:
				case 5:		PtrConfig->display = 1 ; break ;	// Colour
				case 6:
				case 7:		PtrConfig->display = 2 ; break ;	// Colour EGA
				case 8:
				case 9:		PtrConfig->display = 4 ; break ;	// HiRes EGA
				case 10:
				case 11:	PtrConfig->display = 3 ; break ;	// Monochrome
			}
			if (dipswitch > 5) {
				PtrConfig->adapter = 2 ;		// EGA card
			} else if (dipswitch > 3) {
				PtrConfig->adapter = 1 ;		// CGA card
			} else {
				PtrConfig->adapter = 0 ;		// MDA/HGC card
			}
			PtrConfig->cbMemory = (memsize + 1) << 16 ;
			test &= ~(EGA|CGA|MONO) ;
		}
	}
	if (test & CGA) {
		if (test_6845(0x03d4)) {
			PtrConfig->adapter = 1;
			PtrConfig->display = 1;
			test &= ~(CGA|MONO) ;
		}
	}
	if (test & MONO) {
		if (test_6845(0x03b4)) {
			int counter, prev;

			PtrConfig->adapter = PtrConfig->display = 0 ;

			prev = inp(0x03ba);
			for (counter = 0; counter < 0x1000; counter ++) {
				if (inp(0x03ba) != prev) break ;		// Hercules card
			}
			test &= ~(MONO) ;
		}
	}

	return (test != 0) ? ERROR_VIO_MODE : NO_ERROR;
}