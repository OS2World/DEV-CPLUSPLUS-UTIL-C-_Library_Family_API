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
//	Query handle's type
//
USHORT _APICALL
DosQHandType	( unsigned short FileHandle,
				  unsigned short far * PtrHandType,
				  unsigned short far * PtrDeviceAttr )
{
	USHORT err = DosDosDevIOCtl	(FileHandle, 0, NULL, 0, PtrDeviceAttr) ;
	if (!err) *PtrHandType = (*PtrDeviceAttr & 0x0080) ? 1 : 0 ;
	return err ;
}
