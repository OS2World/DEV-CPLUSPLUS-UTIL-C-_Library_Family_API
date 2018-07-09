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

#pragma argsused
//
//	Register a mouse subsystem
//
USHORT _APICALL
MouRegister ( const UCHAR far *PtrModuleName,
			  const UCHAR far *PtrEntryName,
			  ULONG Functions )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Deregister a mouse subsystem
//
USHORT _APICALL
MouDeRegister ( void )
{
	return NO_ERROR ;
}

#pragma argsused
//
//	Flush the mouse queue
//
USHORT _APICALL
MouFlushQue ( USHORT MouseHandle )
{
	return NO_ERROR ;
}

#pragma argsused
//
//	Get the button mask for the hotkey
//
USHORT _APICALL
MouGetHotKey ( USHORT far *PtrButtons,
			   USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Set the button mask for the hotkey
//
USHORT _APICALL
MouSetHotKey ( USHORT far *PtrButtons,
			   USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Get the current position of the mouse
//
USHORT _APICALL
MouGetPtrPos ( PTRLOC far *PtrPosition,
			   USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Set the current position of the mouse
//
USHORT _APICALL
MouSetPtrPos ( PTRLOC far *PtrPosition,
			   USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Alter the shape of the mouse
//
USHORT _APICALL
MouSetPtrShape ( BYTE far *PtrBuffer,
				 PTRSHAPE far *PtrInfo,
				 USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Alter the shape of the mouse
//
USHORT _APICALL
MouGetPtrShape ( BYTE far *PtrBuffer,
				 PTRSHAPE far *PtrInfo,
				 USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Retrieve the configuration of the mouse driver
//
USHORT _APICALL
MouGetDevStatus ( USHORT far *PtrDevStatus,
				  USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Get the number of buttons on the mouse
//
USHORT _APICALL
MouGetNumButtons ( USHORT far *PtrButtons,
				   USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Get the number of mickeys per centimetre
//
USHORT _APICALL
MouGetNumMickeys ( USHORT far *PtrMickeys,
				   USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Read a mouse event from the queue
//
USHORT _APICALL
MouReadEventQue ( MOUEVENTINFO far *PtrEvent,
				  USHORT far *PtrWait,
				  USHORT MouseHandle )
{
	PtrEvent->fs = PtrEvent->row = PtrEvent->col = PtrEvent->Time = 0 ;
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Get the number of elements waiting in the queue
//
USHORT _APICALL
MouGetNumQueEl ( MOUQUEINFO far *PtrNumEvents,
				 USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Obtain the event wask for the queue
//
USHORT _APICALL
MouGetEventMask ( USHORT far *PtrEventMask,
				  USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Alter the event wask for the queue
//
USHORT _APICALL
MouSetEventMask ( USHORT far *PtrEventMask,
				  USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Obtain the scale factor
//
USHORT _APICALL
MouGetScaleFact ( SCALEFACT far *PtrFactors,
				  USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Alter the scale factor
//
USHORT _APICALL
MouSetScaleFact ( SCALEFACT far *PtrFactors,
				  USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Open the mouse driver
//
USHORT _APICALL
MouOpen ( const UCHAR far *PtrDriverName,
		  USHORT far *PtrMouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Close the mouse driver
//
USHORT _APICALL
MouClose ( USHORT MouseHandle )
{
	return NO_ERROR ;
}

#pragma argsused
//
//	Hide the mouse pointer within the given rectangle
//
USHORT _APICALL
MouRemovePtr ( NOPTRRECT far *PtrRect,
			   USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Allow the pointer to become visible again
//
USHORT _APICALL
MouDrawPtr ( USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Reconfigure the mouse driver
//
USHORT _APICALL
MouSetDevStatus ( USHORT far *PtrDevStatus,
				  USHORT MouseHandle )
{
	return ERROR_MOUSE_NO_DEVICE ;
}

#pragma argsused
//
//	Subsystem calls !
//
USHORT _APICALL MouInitReal ( const UCHAR far * );
USHORT _APICALL MouSynch ( USHORT MouseHandle );
