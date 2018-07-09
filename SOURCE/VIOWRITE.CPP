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

#pragma argsused
//
//	Write a string of characters in the existing display attributes
//
USHORT _APICALL
VioWrtCharStr ( const char far *PtrString,
				unsigned short StringLen,
				unsigned short Row,
				unsigned short Col,
				unsigned short VioHandle )
{
	unsigned char far *screen = (unsigned char far *)VioDosPointer(Row, Col) ;

	if (!screen) {
		unsigned short savepos = VioDosGetCurPos() ;
		unsigned short pos = (Row << 8) + (Col & 0xFF) ;

		while (StringLen--) {
			VioDosSetCurPos (pos++) ;
			asm {
				mov AH,0x08 ;
				mov BH,0x00 ;
				push SI ;
				push DI ;
				int 0x10 ;				// Read cell at cursor
				pop DI ;
				pop SI ;
				push AX ;				// Save it
			}
			_AL = *(PtrString)++ ;		// Rewrite with new character
			asm {
				mov AH,0x0A ;
				pop BX ;				// Restore cell
				mov BL,BH ;				// Keep attribute part
				mov BH,0x00 ;
				mov CX,0x01 ;
				push SI ;
				push DI ;
				int 0x10 ;				// Write a cell at cursor
				pop DI ;
				pop SI ;
			}
		}
		VioDosSetCurPos (savepos) ;
	} else {
		while (StringLen--) {
			*screen = (unsigned char)*PtrString++ ;
			screen += 2;
		}
	}

	return NO_ERROR ;
}

#pragma argsused
//
//	Write a string of characters in a single display attribute
//
USHORT _APICALL
VioWrtCharStrAtt (	const char far *PtrString,
					unsigned short StringLen,
					unsigned short Row,
					unsigned short Col,
					const unsigned char far *PtrAttr,
					unsigned short VioHandle )
{
	unsigned short far *screen = VioDosPointer(Row, Col) ;

	if (!screen) {
		unsigned short savepos = VioDosGetCurPos() ;
		unsigned short pos = (Row << 8) + (Col & 0xFF) ;
		unsigned char attr = *PtrAttr & 0x0F;

		while (StringLen--) {
			VioDosSetCurPos (pos++) ;
			_AH = 0x09 ;
			_AL = *(PtrString)++ ;		// Modifies ES:BX
			_BH = 0x00 ;
			_BL = attr ;
			_CX = 0x01 ;
			asm {
				push SI ;
				push DI ;
				int 0x10 ;				// Write a cell at cursor
				pop DI ;
				pop SI ;
			}
		}
		VioDosSetCurPos (savepos) ;
	} else {
		unsigned short attr = ((unsigned short)*PtrAttr) << 8 ;
		while (StringLen--) {
			*(screen)++ = (unsigned char)*PtrString++ | attr ;
		}
	}

	return NO_ERROR ;
}

#pragma argsused
//
//	Write a string of character/attribute pairs
//
USHORT _APICALL
VioWrtCellStr ( const char far *PtrCellStr,
				unsigned short StringLen,
				unsigned short Row,
				unsigned short Col,
				unsigned short VioHandle )
{
	unsigned short far *screen = VioDosPointer(Row, Col) ;

	++StringLen;		// Round upwards
	StringLen >>= 1 ;	// Convert byte count to word count
	if (!screen) {
		unsigned short savepos = VioDosGetCurPos() ;
		unsigned short pos = (Row << 8) + (Col & 0xFF) ;

		while (StringLen--) {
			VioDosSetCurPos (pos++) ;
			_AH = 0x09 ;
			_AL = *(PtrCellStr)++ ;		// Character (modifies ES:BX)
			_BL = *(PtrCellStr)++ ;		// Then attribute
			_BH = 0x00 ;
			_CX = 0x01 ;
			asm {
				push SI ;
				push DI ;
				int 0x10 ;				// Write a cell at cursor
				pop DI ;
				pop SI ;
			}
		}
		VioDosSetCurPos (savepos) ;
	} else {
		while (StringLen--) {
			*(screen)++ = *((unsigned short far *)PtrCellStr)++ ;
		}
	}

	return NO_ERROR ;
}

#pragma argsused
//
//	Write an attribute, repeatedly
//
USHORT _APICALL
VioWrtNAttr   ( const unsigned char far *PtrAttr,
				unsigned short Count,
				unsigned short Row,
				unsigned short Col,
				unsigned short VioHandle )
{
	unsigned char far *screen = (unsigned char far *)VioDosPointer(Row, Col) ;

	if (!screen) {
		unsigned short savepos = VioDosGetCurPos() ;
		unsigned short pos = (Row << 8) + (Col & 0xFF) ;

		while (Count--) {
			VioDosSetCurPos (pos++) ;
			asm {
				mov AH,0x08 ;
				mov BH,0x00 ;
				push SI ;
				push DI ;
				int 0x10 ;				// Read cell at cursor
				pop DI ;
				pop SI ;
				push AX ;
			}
			_BL = *(PtrAttr) ;			// Rewrite with new attribute
			asm {
				pop AX ;
				mov	AH,0x09 ;
				mov BH,0x00 ;
				mov CX,0x01 ;
				push SI ;
				push DI ;
				int 0x10 ;				// Write a cell at cursor
				pop DI ;
				pop SI ;
			}
		}
		VioDosSetCurPos (savepos) ;
	} else {
		while (Count--) {
			screen++ ;
			*screen++ = *(PtrAttr) ;
		}
	}

	return NO_ERROR ;
}

#pragma argsused
//
//	Write a character/attribute pair, repeatedly
//
USHORT _APICALL
VioWrtNCell   ( const unsigned char far *PtrCell,
				unsigned short Count,
				unsigned short Row,
				unsigned short Col,
				unsigned short VioHandle )
{
	unsigned short far *screen = VioDosPointer(Row, Col) ;

	if (!screen) {
		unsigned short savepos = VioDosGetCurPos() ;
		unsigned short pos = (Row << 8) + (Col & 0xFF) ;

		VioDosSetCurPos (pos++) ;
		_AH = 0x09 ;
		_AL = PtrCell[1] ;			// Attribute (modifies ES:BX)
		asm push AX ;
		_AL = PtrCell[0] ;			// Character (modifies ES:BX)
		asm pop BX ;
		_BH = 0x00 ;
		_CX = Count ;
		asm {
			push SI ;
			push DI ;
			int 0x10 ;				// Write CX cells at cursor
			pop DI ;
			pop SI ;
		}
		VioDosSetCurPos (savepos) ;
	} else {
		while (Count--) {
			*screen++ = *((unsigned short far *)PtrCell) ;
		}
	}

	return NO_ERROR ;
}

#pragma argsused
//
//	Write a character, repeatedly
//
USHORT _APICALL
VioWrtNChar   ( const char far *PtrChar,
				unsigned short Count,
				unsigned short Row,
				unsigned short Col,
				unsigned short VioHandle )
{
	unsigned char far *screen = (unsigned char far *)VioDosPointer(Row, Col) ;

	if (!screen) {
		unsigned short savepos = VioDosGetCurPos() ;
		unsigned short pos = (Row << 8) + (Col & 0xFF) ;

		while (Count--) {
			VioDosSetCurPos (pos++) ;
			_AH = 0x08 ;
			_BH = 0x00 ;
			asm {
				push SI ;
				push DI ;
				int 0x10 ;				// Read cell at cursor
				pop DI ;
				pop SI ;
			}
			_BL = _AH ;					// Keep the attribute
			_AH = 0x0A ;
			asm	push BX ;				//
			_AL = *(PtrChar) ;			// Rewrite with new character
			asm pop BX ;
			_BH = 0x00 ;
			_CX = 0x01 ;
			asm {
				push SI ;
				push DI ;
				int 0x10 ;				// Write a cell at cursor
				pop DI ;
				pop SI ;
			}
		}
		VioDosSetCurPos (savepos) ;
	} else {
		while (Count--) {
			*screen = *(PtrChar) ;
			screen += 2 ;
		}
	}

	return NO_ERROR ;
}