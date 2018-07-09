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
//	The function chain invoked by a vector handler starts at the latest
//	setting of DosSetVect() and *may* chain through to NoFunction() .
//
static int _APICALL NoFunction () { return 0 ; }

static int (_APICALL *Int0Function)() = NoFunction ;
static int (_APICALL *Int4Function)() = NoFunction ;
static int (_APICALL *Int5Function)() = NoFunction ;
static int (_APICALL *Int6Function)() = NoFunction ;
static int (_APICALL *Int7Function)() = NoFunction ;
static int (_APICALL *Int10Function)() = NoFunction ;

//
//	The actual hardware interrupt handlers invoke the vector call chains.
//
static void interrupt Int0Handler() { (*Int0Function)() ; }
static void interrupt Int4Handler() { (*Int4Function)() ; }
static void interrupt Int5Handler() { (*Int5Function)() ; }
static void interrupt Int6Handler() { (*Int6Function)() ; }
static void interrupt Int7Handler() { (*Int7Function)() ; }
static void interrupt Int10Handler() { (*Int10Function)() ; }

//
//	Set exception vectors
//
USHORT _APICALL
DosSetVec	( USHORT VectNum,
			 int (_APICALL *Function)(),
			 int (_APICALL * far *PrevFunction)() )
{
	if (VectNum != 0
	&&  VectNum != 4 && VectNum != 5 && VectNum != 6
	&&  VectNum != 7 && VectNum != 0x10 )
		return ERROR_NOT_SUPPORTED ;

	DosDosSaveVectors() ;
	switch (VectNum) {
		case 0:
			*PrevFunction = Int0Function;
			Int0Function = Function ;
			DosDosSetVect( VectNum, Int0Handler) ;
			break;
		case 4:
			*PrevFunction = Int4Function;
			Int4Function = Function ;
			DosDosSetVect( VectNum, Int4Handler) ;
			break;
		case 5:
			*PrevFunction = Int5Function;
			Int5Function = Function ;
			DosDosSetVect( VectNum, Int5Handler) ;
			break;
		case 6:
			*PrevFunction = Int6Function;
			Int6Function = Function ;
			DosDosSetVect( VectNum, Int6Handler) ;
			break;
		case 7:
			*PrevFunction = Int7Function;
			Int7Function = Function ;
			DosDosSetVect( VectNum, Int7Handler) ;
			break;
		case 0x10:
			*PrevFunction = Int10Function;
			Int10Function = Function ;
			DosDosSetVect( VectNum, Int10Handler) ;
			break;
	}
	return NO_ERROR ;
}
