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
//	Obtain environment selector
//
USHORT _APICALL
DosGetEnv	( unsigned short far *PtrEnvSelector,
			  unsigned short far *PtrCommandOffset )
{
	USHORT PSPSeg ;
	USHORT err = DosDosQPSPSeg(&PSPSeg) ;

	if (err) return err ;

	struct psp far *psp = (struct psp far *)MK_FP(PSPSeg, 0) ;

	//
	// Scan through the environment segment to find command line.
	//
	UCHAR far *env = (UCHAR far *)MK_FP((USHORT)psp->environment, 0) ;

	while (*env++) { while (*env++) ; }	// Skip environment strings

	//
	//	To convert a DOS environment segment into an OS/2 environment
	//	segment, we need to lose the 0x0001 marker, shift the program name
	//	up, and append an empty string (argv[0]) and the command tail
	//	to the segment.  This involves reallocating the segment, and
	//	must be done only once.
	//
	//	We can be sure of a 0x0001 marker because this Family API library
	//	is for use on DOS 3.3 or later only.
	//
	static UCHAR changed = 0 ;
	if (!changed) {
		changed = 1 ;

		unsigned char far *ptr = env ;		// Save position
		ptr += sizeof(USHORT) ;				// Skip string count
		while (*env++ = *ptr++) {}			// Copy full pathname down

		*PtrCommandOffset = FP_OFF(env) ;

		USHORT CmdLen = psp->cmdlen ;
		USHORT NewSize = FP_OFF(ptr) + 1 + CmdLen + 1 ;

		if (DosReallocSeg ( NewSize, FP_SEG(env) ) ) {
			//
			//	The segment will not expand in place, so allocate
			//	a new one.
			//
			USHORT NewEnvSeg ;
			if ( err = DosAllocSeg(NewSize, &NewEnvSeg, 0) ) {
				//
				//	PANIC ! We cannot append the command line. Return an
				//	out of memory error, and since we have a sizeof(USHORT)
				//	to spare anyway, make two empty strings with it.
				//	The caller will have to deal with this situation.
				//
				*env++ = 0 ;
				*env++ = 0 ;
				return err ;
			} else {
				UCHAR far *p = (UCHAR far *)MK_FP(NewEnvSeg, 0) ;
				UCHAR far *q = (UCHAR far *)MK_FP(FP_SEG(env), 0) ;
				while (NewSize--) *p++ = *q++ ;
				psp->environment = NewEnvSeg ;	// So DOS knows about it
				DosFreeSeg(FP_SEG(env)) ;		// Release old segment
				env = (UCHAR far *)MK_FP(NewEnvSeg, FP_OFF(env)) ;
			}
		}
		*env++ = 0 ;						// argv[0] is empty
		ptr = &psp->cmdline[0] ;
		while (CmdLen--) *env++ = *ptr++ ;	// copy command tail
		*env++ = 0 ;						// and NUL terminate it

		*PtrEnvSelector = FP_SEG(env) ;

	} else {
		while (*env++) {}					// Skip full pathname
		*PtrCommandOffset = FP_OFF(env) ;
		*PtrEnvSelector = FP_SEG(env) ;
	}

	return err ;
}
