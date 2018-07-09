//
//	  *******************************************************************
//		JdeBP C++ Library Routines 	 	General Public Licence v1.00
//			Copyright (c) 1991,1992	 Jonathan de Boyne Pollard
//	  *******************************************************************
//
// Part of FamAPI.LIB
//

#include "famapi.h"

//
//	This structure must _not_ be word-aligned
//
struct MsgSeg {
	UCHAR ffbyte ;			// Byte, value 0xFF
	UCHAR MkMsg[5] ;		// The letters "MKMSG"
	UCHAR Unknwn1[2] ;		// Letter plus NUL
	UCHAR Prefix[3] ;		// Three letter prefix for message number
	USHORT NMessages ;		// Total number of messages
	USHORT Unknwn2 ;		// Unknown
	UCHAR Flag ;			// 0 = ULONG pointers, 1 = USHORT pointers
	USHORT PtrTable ;		// 0 = No table offsets, 2 = Table offsets follow
	USHORT FirstPtr ;		// Offset of start of pointer table
	USHORT LastPtr ;		// Offset ot end of pointer table
	ULONG FileLen ;			// Total length of file
	UCHAR Unknwn4[5] ;		// Unknown
} ;

//
//	Retrive a message from a message file, expanding substitutions
//
USHORT _APICALL
DosGetMessage	( char far * far *PtrVTable,
				  USHORT VCount,
				  char far *PtrBuffer,
				  USHORT BufferLength,
				  USHORT MsgNumber,
				  char far *PtrFileName,
				  USHORT far *PtrMsgLength )
{
	USHORT Fh, NBytes, MsgLength = 0 ;
	UINT Action ;
	USHORT err = DosOpen(PtrFileName, &Fh, &Action, 0UL, 0L, 0x0001, 0x0040, 0UL) ;

	if (err) return err ;

	struct MsgSeg MsgSeg ;

	err = DosRead(Fh, (void far *)&MsgSeg, sizeof(MsgSeg), &NBytes) ;
	if (!err) {
		if (MsgNumber < MsgSeg.NMessages) {
			ULONG PtrOffset, StartOffset, EndOffset ;
			if (MsgSeg.PtrTable == 0x0002) {
				PtrOffset = MsgSeg.FirstPtr ;
				EndOffset = MsgSeg.FileLen ;
			} else {
				PtrOffset = sizeof(MsgSeg) ;
				DosChgFilePtr(Fh, 0, 2, (LONG far *)&EndOffset) ;
			}

			if (MsgSeg.Flag == 1) {
				PtrOffset += MsgNumber * 2 ;
				DosChgFilePtr(Fh, PtrOffset, 0, (LONG far *)&PtrOffset) ;
				DosRead(Fh, (void far *)&StartOffset, 2, &NBytes) ;
				if ((MsgNumber + 1) < MsgSeg.NMessages)
					DosRead(Fh, (void far *)&StartOffset, 2, &NBytes) ;
			} else {
				PtrOffset += MsgNumber * 4 ;
				DosChgFilePtr(Fh, PtrOffset, 0, (LONG far *)&PtrOffset) ;
				DosRead(Fh, (void far *)&StartOffset, 4, &NBytes) ;
				if ((MsgNumber + 1) < MsgSeg.NMessages)
					DosRead(Fh, (void far *)&EndOffset, 4, &NBytes) ;
			}

			//
			//	We have the start and edd offsets, read the message in
			//
			UCHAR MsgByte ;
			DosChgFilePtr(Fh, StartOffset, 0, (LONG far *)&StartOffset) ;
			DosRead(Fh, (void far *)&MsgByte, sizeof(MsgByte), &NBytes) ;
			if (MsgByte == 'E' || MsgByte == 'W') {
				if (MsgLength < BufferLength) PtrBuffer[MsgLength++] = MsgSeg.Prefix[0] ;
				if (MsgLength < BufferLength) PtrBuffer[MsgLength++] = MsgSeg.Prefix[1] ;
				if (MsgLength < BufferLength) PtrBuffer[MsgLength++] = MsgSeg.Prefix[2] ;
				USHORT Divisor = 1000U, ANumber = MsgNumber ;
				while (Divisor > 0) {
					UCHAR c = '0' ;
					while (ANumber >= Divisor) {
						c++ ;
						ANumber -= Divisor ;
					}
					if (MsgLength < BufferLength)
						PtrBuffer[MsgLength++] = c ;
					Divisor /= 10U ;
				}
				if (MsgLength < BufferLength) PtrBuffer[MsgLength++] = ':' ;
				if (MsgLength < BufferLength) PtrBuffer[MsgLength++] = ' ' ;
			}
			if (MsgByte == 'H'
			||  MsgByte == 'I'
			||  MsgByte == 'E'
			||  MsgByte == 'W') {
				USHORT ToGo = BufferLength - MsgLength ;
				if (ToGo >= (EndOffset - StartOffset))
					ToGo = EndOffset - StartOffset - 1 ;
				else if (ToGo < (EndOffset - StartOffset))
					err = ERROR_MR_MSG_TOO_LONG ;
				DosRead(Fh, (void far *)&PtrBuffer[MsgLength], ToGo, &NBytes) ;
				MsgLength += NBytes ;
				*PtrMsgLength = MsgLength ;
			} else
				err = ERROR_MR_MID_NOT_FOUND ;
		} else
			err = ERROR_MR_MID_NOT_FOUND ;
	}

	DosClose(Fh) ;

	return err ;
}

//
//	Copy a message, expanding substitutions
//
USHORT _APICALL
DosInsMessage ( char far * far *PtrVTable,
				USHORT VCount,
				char far *PtrMessage,
				USHORT MessageLength,
				char far *PtrBuffer,
				USHORT BufferLength,
				USHORT far *PtrMsgLength )
{
	return ERROR_MR_UN_PERFORM ;
}

//
//	Put a message to a previously opened message file
//
USHORT _APICALL
DosPutMessage ( int FileHandle,
				USHORT MessageLength,
				char far *PtrMessage )
{
	return ERROR_MR_UN_PERFORM ;
}
