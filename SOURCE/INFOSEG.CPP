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
//	Get the address of the "global" and "local" info segments
//
USHORT _APICALL
DosGetInfoSeg	( unsigned short far *PtrGSeg,
				  unsigned short far *PtrLSeg )
{
	static USHORT GSeg = 0, LSeg = 0 ;

	GINFOSEG far *gi ;
	LINFOSEG far *li ;

	if (GSeg == 0) {
		USHORT err = DosAllocSeg((unsigned short) sizeof(GINFOSEG), &GSeg, 0) ;
		if (err) return err ;

		gi = (GINFOSEG far *)MK_FP(GSeg, 0) ;

		gi->timezone = 0 ;
		gi->cusecTimerInterval = 55 ;

		USHORT Version ;

		DosGetVersion( &Version ) ;

		gi->uchMinorVersion = Version & 0xff ;
		gi->uchMajorVersion = Version >> 8 ;

		if (gi->uchMajorVersion >= 5) {
			_AX = 0x3306 ;
			Dos3Call() ;
		} else
			_DL = 0 ;

		gi->chRevisionLetter = _DL ;

		gi->sgCurrent = gi->sgMax = 1 ;	// Only one screen group.
		gi->cHugeShift = 12 ;			// Hardcoded value !
		gi->fProtectModeOnly = 0 ;		// Obviously, since we are in DOS.
		DosDosQPSPSeg(&gi->pidForeground) ;
		gi->fDynamicSched = 0 ;
		gi->csecMaxWait = 1 ;
		gi->cmsecMinSlice = gi->cmsecMaxSlice = 1 ;

		gi->bootdrive;

		// What about "UCHAR   amecRAS[32];" ?
	}
	if (LSeg == 0) {
		USHORT err = DosAllocSeg((unsigned short) sizeof(LINFOSEG), &LSeg, 0) ;
		if (err) return err ;

		li = (LINFOSEG far *)MK_FP(LSeg, 0) ;

		DosDosQPSPSeg(&li->pidCurrent) ;

		struct psp far *psp = (struct psp far *)MK_FP(li->pidCurrent, 0) ;

		li->pidParent = psp->parentpsp ;
		li->prtyCurrent = 0 ;			// Unknown ?
		li->tidCurrent = 1 ;			// Always the primary thread
		li->sgCurrent = li->sgSub = 1 ;	// Always the single screen group
		li->fForeground = 1 ;			// Always in the foreground
	}

	//
	//	Always update the current date and time on every call.
	//
	//	This does not set gi->time and gi->msecs !
	//

	DOSDATETIME dt ;

	DosGetDateTime(&dt) ;

	gi->timezone = dt.timezone ;
	gi->year = dt.year ;
	gi->month = dt.month ;
	gi->day = dt.day ;
	gi->hour = dt.hours ;
	gi->minutes = dt.minutes ;
	gi->seconds = dt.seconds ;
	gi->hundredths = dt.hundredths ;
	gi->weekday = dt.weekday ;

	*PtrGSeg = GSeg ;
	*PtrLSeg = LSeg ;

	return NO_ERROR ;
}