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
//	The function chain invoked by a signal handler starts at the latest
//	setting of DosSetSigHandler() and *may* chain through to NoFunction() .
//
static void _APICALL NoFunction (USHORT, USHORT) { }
static void far (_APICALL *CtrlBreakFunction)(USHORT, USHORT) = NoFunction ;
static void far (_APICALL *CtrlCFunction)(USHORT, USHORT) = NoFunction ;
static unsigned int CtrlBreakAction = 0 ;
static unsigned int CtrlCAction = 0 ;

//
//	The actual software interrupt handlers invoke the signal call chains.
//
static void (interrupt *WasInt23Handler)() = 0L ;
static void interrupt Int23Handler()
{
	asm push ds ;
	DosDosSetVect( 0x23, WasInt23Handler) ;
	if (CtrlBreakAction != 1) (*CtrlBreakFunction)(0, 0) ;
	if (CtrlCAction != 1) (*CtrlCFunction)(0, 0) ;
	DosDosSetVect( 0x23, Int23Handler) ;
	asm pop ds ;
}

//
//	Set signal handlers
//
//	We do not need to save and restore INT23 vector ourselves (unless we
//	are actually in the middle of an INT23 handler), because DOS does that
//	for us using a field in the PSP.
//
USHORT _APICALL
DosSetSigHandler	( void far (_APICALL *PtrSigHandler)(USHORT, USHORT),
					  void far (_APICALL * far *PtrPrevSigHandler)(USHORT, USHORT),
					  unsigned int far *PtrAction,
					  unsigned int Action,
					  unsigned int SigNumber )
{
	unsigned int far *PtrNewAction ;
	void far (_APICALL * far *PtrNewFunction)(USHORT, USHORT) ;

	if (!WasInt23Handler)
		DosDosGetVect( 0x23, &WasInt23Handler) ;

	//
	//	We only support CTRL-BREAK and CTRL-C.  DOS does not support
	//	broken pipes or user "flags".
	//
	switch (SigNumber) {
		case SIG_CTRLBREAK :
			PtrNewAction = &CtrlBreakAction ;
			PtrNewFunction = &CtrlBreakFunction ;
			break ;

		case SIG_CTRLC :
			PtrNewAction = &CtrlCAction ;
			PtrNewFunction = &CtrlCFunction ;
			break ;

		default:
			return ERROR_NOT_SUPPORTED ;
	}

	switch (Action) {
		case 0x0000:	//	Install default signal handler

			*PtrPrevSigHandler = (*PtrNewFunction);
			*PtrAction = (*PtrNewAction) ;
			(*PtrNewFunction) = NoFunction ;
			(*PtrNewAction) = Action ;

			DosDosSetVect( 0x23, WasInt23Handler) ;
			break ;

		case 0x0001:	//	Ignore signal

			*PtrPrevSigHandler = (*PtrNewFunction);
			*PtrAction = (*PtrNewAction) ;
			(*PtrNewFunction) = NoFunction ;
			(*PtrNewAction) = Action ;

			DosDosSetVect( 0x23, Int23Handler) ;
			break ;

		case 0x0002:	//	Set specified handler function

			*PtrPrevSigHandler = (*PtrNewFunction);
			*PtrAction = (*PtrNewAction) ;
			(*PtrNewFunction) = PtrSigHandler ;
			(*PtrNewAction) = Action ;

			DosDosSetVect( 0x23, Int23Handler) ;
			break ;

		case 0x0003:	// Disallow flags from other processes ...
		case 0x0004:	// Reset pending signal
		default:
			return ERROR_NOT_SUPPORTED ;
	}
	return NO_ERROR ;
}
