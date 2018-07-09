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

#define MAXFINDBUF	128

//
//	Each directory search buffer is allocated a segment using DosAllocSeg.
//	Since we *know* that this is DOS real mode, this will not overload
//	an LDT.  If the segment is 0, then we consider it unallocated.
//

static unsigned short FindBufSeg[MAXFINDBUF] = { 0 } ;

//
//	Search a directory using DOS buffer and copy to OS/2 style buffer
//
// We ignore a "No (more) files" error.
//
static
USHORT _APICALL
DoTheSearch	( struct DOSFind_t far *PtrFindBuf,
			  FILEFINDBUF far *PtrFindBuffer,
			  unsigned int BufferLength,
			  unsigned int far * PtrSearchCount )
{
	USHORT err = NO_ERROR;
	unsigned int SearchCount = *PtrSearchCount ;

	if (SearchCount < 1 || BufferLength < sizeof(FILEFINDBUF))
		return ERROR_BUFFER_OVERFLOW ;

	//
	//	Assume that the first one has been done for us
	//
	while (!err && SearchCount) {
		unsigned char cchName ;
		--SearchCount;

		//
		//	Copy the DOS find buffer to the OS/2 find buffer
		//
		PtrFindBuffer->fdateCreation =
		PtrFindBuffer->fdateLastAccess =
		PtrFindBuffer->fdateLastWrite = *(FDATE far *)(&PtrFindBuf->wr_date) ;
		PtrFindBuffer->ftimeCreation =
		PtrFindBuffer->ftimeLastAccess =
		PtrFindBuffer->ftimeLastWrite = *(FTIME far *)(&PtrFindBuf->wr_time) ;
		PtrFindBuffer->cbFile = PtrFindBuf->size ;
		// This is a kludge: we round up to next 4K boundary
		PtrFindBuffer->cbFileAlloc = (PtrFindBuf->size + 0x0FFF) & ~0x0FFF ;
		PtrFindBuffer->attrFile = PtrFindBuf->attrib ;
		for (cchName = 0 ; PtrFindBuf->name[cchName] && cchName < 13; cchName++) {
			PtrFindBuffer->achName[cchName] = PtrFindBuf->name[cchName] ;
		}
		PtrFindBuffer->cchName = cchName ;
		PtrFindBuffer->achName[cchName] = '\000';

		//
		// Go to the next buffer.  Because the caller may be an HPFS program
		// expecting a longer structure length, we increment by the caller's
		// EXPECTED structure length rather than the ACTUAL structure length.
		//
		PtrFindBuffer = (FILEFINDBUF far *)((unsigned char far *)PtrFindBuffer + BufferLength) ;

		// Look for another only if there are more buffers to fill up
		if (SearchCount) err = DosDosFindNext (PtrFindBuf) ;
	}

	*PtrSearchCount -= SearchCount ;

	return (err == ERROR_NO_MORE_FILES) ? NO_ERROR : err ;
}

//
//	Close a search directory
//
USHORT _APICALL
DosFindClose ( unsigned short DirHandle )
{
	if (DirHandle >= MAXFINDBUF) return ERROR_INVALID_TARGET_HANDLE ;

	USHORT err = DosFreeSeg(FindBufSeg[DirHandle]) ;

	FindBufSeg[DirHandle] = 0 ;

	return err ;
}

//
//	Find a set of files in a directory
//
USHORT _APICALL
DosFindFirst	( char far *PtrFileSpec,
				  unsigned short far *PtrDir,
				  unsigned int Attribute,
				  FILEFINDBUF far *PtrFindBuffer,
				  unsigned int BufferLength,
				  unsigned int far *PtrSearchCount,
				  unsigned long Reserved )
{
	if (*PtrDir == -1U) {
		*PtrDir = 0 ;

		for (;;) {
			if (FindBufSeg[*PtrDir] == 0)
				break ;
			else if (*PtrDir >= MAXFINDBUF)
				return ERROR_NO_MORE_SEARCH_HANDLES ;
			else
				(*PtrDir)++ ;
		}
	} else if (*PtrDir >= MAXFINDBUF)
		return ERROR_INVALID_TARGET_HANDLE ;

	if (FindBufSeg[*PtrDir] == 0) {
		USHORT err = DosAllocSeg((unsigned short) sizeof(struct DOSFind_t),
								&FindBufSeg[*PtrDir], 0) ;
		if (err) return err ;
	}

	struct DOSFind_t far *PtrFindBuf = (struct DOSFind_t far *)MK_FP(FindBufSeg[*PtrDir], 0) ;

	//
	// We will succeed if at least one file can be found
	//
	USHORT err = DosDosFindFirst (PtrFileSpec, Attribute, PtrFindBuf) ;
	if (!err) err = DoTheSearch	(PtrFindBuf, PtrFindBuffer, BufferLength, PtrSearchCount ) ;
	return err ;
}

//
//	Find the next set of files in a directory
//
USHORT _APICALL
DosFindNext	( unsigned short Dir,
			  FILEFINDBUF far *PtrFindBuffer,
			  unsigned int BufferLength,
			  unsigned int far * PtrSearchCount )
{
	if (Dir >= MAXFINDBUF || FindBufSeg[Dir] == 0)
		return ERROR_INVALID_TARGET_HANDLE ;

	struct DOSFind_t far *PtrFindBuf = (struct DOSFind_t far *)MK_FP(FindBufSeg[Dir], 0) ;

	//
	// We will succeed if at least one more file can be found
	//
	USHORT err = DosDosFindNext (PtrFindBuf) ;
	if (!err) err = DoTheSearch	(PtrFindBuf, PtrFindBuffer, BufferLength, PtrSearchCount ) ;
	return err ;
}
