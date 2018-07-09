//
//	  *******************************************************************
//		JdeBP C++ Library Routines 	 	General Public Licence v1.00
//			Copyright (c) 1991,1992	 Jonathan de Boyne Pollard
//	  *******************************************************************
//
// Place in library : STDCm.LIB
//

#define	_MSDOS_SOURCE	1
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <os2.h>
#include "_malloc.h"
#include "_c0.h"


union HeapRecord {
	size_t size ;							// Size of block (inc header)
	union  HeapRecord near *next ;			// Next heap record
	struct FreeHeapRecord {
		struct FreeHeapRecord near *next ;	// Next free record
		struct FreeHeapRecord near *prev ;	// Next free record
	} f ;
} ;

struct HeapSegment {
	struct HeapSegment far *next ;		// Next segment in far heap
	size_t segsize ;					// Size of segment in bytes
	size_t maxsize ;					// Maximum size in bytes
	union HeapRecord near *first ;		// First in free list
	union HeapRecord near *last ;		// Last in free list
	union HeapRecord near *rover ;		// Roving pointer
} ;


//
//	Sub-Allocation out of a single segment
//

static
union HeapRecord far *
AllocateFromSegment ( struct HeapSegment far *hseg, size_t needed )
{
	if (needed > (hseg->segsize - sizeof(union HeapRecord))
		return 0 ;

	needed += sizeof(union HeapRecord) ;

	if (hseg->first == 0)
		return CreateSegment(hseg, needed) ;

	if (hseg->rover != 0) {

		union HeapRecord far *start = hseg->rover ;

		do {

			register union HeapRecord far *rover = hseg->rover ;

			if (rover->size > needed) {

				// Allocate the tail part of the heap record

				union HeapRecord far *old = rover ;

				rover->size -= needed ;

				rover = (union HeapRecord far *)
							((unsigned char far *)rover + rover->size) ;

				rover->size = needed ;
				rover->next = old->next ;
				old->next = rover ;

				return rover ;

			} else if (rover->size == needed) {

				// Pull the heap record off the free list

				if (rover->f.next != rover) {
					rover->f.next->f.prev = rover->f.prev ;
					rover->f.prev->f.next = rover->f.next ;
				} else
					hseg->rover = 0 ;

				return rover ;

			} else
				hseg->rover = rover->f.next ;

		} while (hseg->rover != start) ;
	}

	if (hseg->segsize > (hseg->maxsize - needed)
		return 0 ;

	union HeapRecord far *append =
		(union HeapRecord far *)(MK_FP(FP_SEG(hseg), hseg->segsize)) ;

	USHORT err = DosReallocSeg (FP_SEG(hseg), hseg->segsize += needed) ;

	if (err)
		return 0 ;

	hseg->last->next = append ;
	append->size = needed ;
	append->next = 0 ;
	hseg->last = append ;

	return append ;
}