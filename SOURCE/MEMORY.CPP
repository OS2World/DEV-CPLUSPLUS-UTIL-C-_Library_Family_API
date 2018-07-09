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
//	Allocate memory
//
USHORT _APICALL
DosAllocSeg	( unsigned short Size,
			  unsigned short far *PtrSelector,
			  unsigned short AllocFlags )
{
	if (Size & 0xF)
		_BX = (Size >> 4) + 1 ;		// Round up to next paragraph
	else
		_BX = Size >> 4 ;
	_AH = 0x48 ;
	Dos3Call() ;
	if (_FLAGS & 0x0001) return _AX;
	*PtrSelector = _AX ;
	return NO_ERROR ;
}

//
//	Free memory
//
USHORT _APICALL
DosFreeSeg	( unsigned short Selector )
{
	_ES = Selector ;
	_AH = 0x49 ;
	Dos3Call() ;
	return (_FLAGS & 0x0001) ? _AX : NO_ERROR;
}

//
//	Obtain available memory
//
USHORT _APICALL
DosMemAvail	( unsigned long far *PtrBlockSize )
{
	_BX = -1U ;				// Attempt to allocate 1Mb of memory
	_AH = 0x48 ;
	Dos3Call() ;			// Ignore error : This should always fail
	asm {
		xor dx,dx;
		mov ax,bx;			// Take copy of BX
		mov cx,4 ;
		shl ax,cl;			// And left shift it for low word
		push ax ;
		mov ax,bx;			// Take copy of BX
		mov cx,-4;
		shl ax,cl;			// And right shift it for high word
		mov dx,ax;
		pop ax;
		les bx,PtrBlockSize;
		mov [es:bx], ax ;
		mov [es:bx + 2], dx ;
	}
//	*PtrBlockSize = ((ULONG)_DX << 16) + _AX ;
	return NO_ERROR;
}

//
//	Modify memory allocation
//
USHORT _APICALL
DosReallocSeg ( unsigned short NewSize, unsigned short Selector )
{
	if (NewSize & 0xF)
		_BX = (NewSize >> 4) + 1 ;		// Round up to next paragraph
	else
		_BX = NewSize >> 4 ;
	_ES = Selector ;
	_AH = 0x4A ;
	Dos3Call() ;
	return (_FLAGS & 0x0001) ? _AX : NO_ERROR;
}
