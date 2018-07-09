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

static int Saved = 0 ;

static void interrupt (far *Int0Vect) () = NULL ;
static void interrupt (far *Int4Vect) () = NULL ;
static void interrupt (far *Int5Vect) () = NULL ;
static void interrupt (far *Int6Vect) () = NULL ;
static void interrupt (far *Int7Vect) () = NULL ;
static void interrupt (far *Int10Vect) () = NULL ;

//
//	Set a vector
//
void _APICALL
DosDosSetVect ( USHORT VectNum,
				void interrupt (far *VecFN) () )
{
	asm push ds ;
	_DX = FP_OFF(VecFN) ;
	_DS = FP_SEG(VecFN) ;
	_AX = VectNum ;
	_AH = 0x25 ;
	Dos3Call() ;
	asm pop ds ;
}

//
//	Set a vector
//
void _APICALL
DosDosGetVect ( USHORT VectNum,
				void interrupt (far * far *PtrVecFN) () )
{
	asm push ds ;
	_BX = FP_OFF(PtrVecFN) ;
	_ES = FP_SEG(PtrVecFN) ;
	_AL = VectNum ;
	_AH = 0x35 ;
	Dos3Call() ;
	asm pop ds ;
}

//
//	Save vectors
//
USHORT _APICALL
DosDosSaveVectors	( void )
{
	if (!Saved) {
		asm push ds ;
		_BX = FP_OFF(&Int0Vect) ;
		_ES = FP_SEG(&Int0Vect) ;
		_AX = 0x3500 ;
		Dos3Call() ;
		_BX = FP_OFF(&Int4Vect) ;
		_ES = FP_SEG(&Int4Vect) ;
		_AX = 0x3504 ;
		Dos3Call() ;
		_BX = FP_OFF(&Int5Vect) ;
		_ES = FP_SEG(&Int5Vect) ;
		_AX = 0x3505 ;
		Dos3Call() ;
		_BX = FP_OFF(&Int6Vect) ;
		_ES = FP_SEG(&Int6Vect) ;
		_AX = 0x3506 ;
		Dos3Call() ;
		_BX = FP_OFF(&Int7Vect) ;
		_ES = FP_SEG(&Int7Vect) ;
		_AX = 0x3507 ;
		Dos3Call() ;
		_BX = FP_OFF(&Int10Vect) ;
		_ES = FP_SEG(&Int10Vect) ;
		_AX = 0x3510 ;
		Dos3Call() ;
		asm pop ds ;
		Saved = 1 ;
	}
	return NO_ERROR ;
}

//
//	Restore vectors
//
USHORT _APICALL
DosDosRestoreVectors	( void )
{
	if (Saved) {
		DosDosSetVect (0x00, Int0Vect) ;
		DosDosSetVect (0x04, Int4Vect) ;
		DosDosSetVect (0x05, Int5Vect) ;
		DosDosSetVect (0x06, Int6Vect) ;
		DosDosSetVect (0x07, Int7Vect) ;
		DosDosSetVect (0x10, Int10Vect) ;
		Saved = 0 ;
	}
	return NO_ERROR ;
}
