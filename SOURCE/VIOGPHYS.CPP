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
//	Return addresses of physical buffer
//
USHORT _APICALL
VioGetPhysBuf	( VIOPHYSBUF far *PtrBuffer,
				  unsigned short Reserved )
{
	PtrBuffer->pBuf = (unsigned char far *)VioDosPointer(0, 0) ;
	PtrBuffer->asel[0] = FP_SEG(PtrBuffer->pBuf) ;
	return NO_ERROR ;
}