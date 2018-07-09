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
//	A segment set up for sub-allocation has the pointer to the first free
//	block (offset only) in the first USHORT of the segment, and the segment
//	size in the second.
//
//	The free blocks are NOT sorted.
//

struct SubAllocSeg {
	USHORT NextOffset ;			// Next free block
	USHORT Size ;				// Size (rounded up)
} ;

//
//	The following was defined for testing the suballocation routines in
//	protected mode.  All allocated and freed blocks of memory, and all
//	segments after resizing, were cleared with zeros.  This caught several
//	pointer errors.
//

#define	BE_A_REAL_BASTARD	0

//
//	Allocate memory
//
USHORT _APICALL
DosSubAlloc	( USHORT Selector,
			  USHORT far *PtrOffset,
			  USHORT Size )
{
	struct SubAllocSeg far *sptr =
		(struct SubAllocSeg far *)MK_FP(Selector, 0) ;

	// Size is rounded up to next multiple of a long word

	ULONG LSize = (Size + 3UL) & ~3UL ;

	while (sptr->NextOffset != 0) {

		struct SubAllocSeg far *lastptr = sptr ;

		FP_OFF(sptr) = sptr->NextOffset ;

		if (sptr->Size > LSize) {

			// Allocate off the tail of the block

			sptr->Size -= LSize ;
			*PtrOffset = FP_OFF(sptr) + sptr->Size ;

#if BE_A_REAL_BASTARD
			//
			//	Bastard debugging code to provoke errors
			//
			UCHAR far *p = (UCHAR far *)MK_FP(Selector, *PtrOffset) ;
			while (LSize-- > 0)
				*p++ = 0 ;
#endif

			return NO_ERROR ;

		} else if (sptr->Size == LSize) {

			// Pull the heap record off the free list

			lastptr->NextOffset = sptr->NextOffset ;
			*PtrOffset = FP_OFF(sptr) ;

#if BE_A_REAL_BASTARD
			//
			//	Bastard debugging code to provoke errors
			//
			UCHAR far *p = (UCHAR far *)MK_FP(Selector, *PtrOffset) ;
			while (LSize-- > 0)
				*p++ = 0 ;
#endif

			return NO_ERROR ;

		}
	}

	return ERROR_NOT_ENOUGH_MEMORY ;
}

//
//	Free memory
//
USHORT _APICALL
DosSubFree	( USHORT Selector,
			  USHORT Offset,
			  USHORT Size )
{
	struct SubAllocSeg far *headptr =
		(struct SubAllocSeg far *)MK_FP(Selector, 0) ;
	struct SubAllocSeg far *freeptr =
		(struct SubAllocSeg far *)MK_FP(Selector, Offset) ;

	// Size is rounded up to next multiple of a long word

	ULONG LSize = (Size + 3UL) & ~3UL ;

	freeptr->Size = LSize ;
	freeptr->NextOffset = headptr->NextOffset ;
	headptr->NextOffset = Offset ;

#if BE_A_REAL_BASTARD
	//
	//	Bastard debugging code to provoke errors
	//
	UCHAR far *p = (UCHAR far *)MK_FP(Selector, Offset + 4) ;
	LSize -= 4 ;
	while (LSize-- > 0)
		*p++ = 0 ;
#endif

	return NO_ERROR ;
}

//
//	Initialise/Reinitialise a sub-allocated segment
//
USHORT _APICALL
DosSubSet	( USHORT Selector,
			  USHORT Flags,
			  USHORT Size )
{
	//
	//	Because the SubAlloc and SubFree functions round UP to the next
	//	multiple of a long word, this must round the overall size DOWN,
	//	lest the three or less bytes at the end are allocated as a
	//	whole long word, and fencepost errors result.
	//

	ULONG LSize = (Size - 1UL) & ~3UL ;

	struct SubAllocSeg far *headptr =
		(struct SubAllocSeg far *)MK_FP(Selector, 0) ;

	if (Flags == 1) {
		headptr->Size = (USHORT) LSize ;

		if (LSize > 4) {
			headptr->NextOffset = 4 ;
			headptr++ ;
			headptr->Size = (USHORT) (LSize - 4) ;
			headptr->NextOffset = (USHORT) 0 ;		// Signal last free block

#if BE_A_REAL_BASTARD
			//
			//	Bastard debugging code to provoke errors
			//
			UCHAR far *p = (UCHAR far *)(headptr + 1) ;
			LSize -= 8 ;
			while (LSize-- > 0)
				*p++ = 0 ;
#endif

		} else
			headptr->NextOffset = 0 ;

	} else {

		if (headptr->Size < LSize) {

			USHORT EndOffset = headptr->Size ;

			headptr->Size = (USHORT) LSize ;

			return DosSubFree(Selector, EndOffset, (USHORT) (LSize - EndOffset)) ;

		} else
			headptr->Size = (USHORT) LSize ;
	}

	return NO_ERROR ;
}