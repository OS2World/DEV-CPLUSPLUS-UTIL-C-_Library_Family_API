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

#define NO_CLEAR_FLAG 0x80

#pragma argsused
//
//	Get display mode
//
//	Known Bugs : 	Little (& incorrect) processing of graphics modes.
//
USHORT _APICALL
VioGetMode ( VIOMODEINFO far *PtrMode,
			 unsigned short VioHandle )
{
	volatile unsigned char far *BIOSrows = (volatile unsigned char far *)MK_FP(0x0040, 0x0084) ;
	VIOMODEINFO minfo ;
	unsigned short mode;

	mode = VioDosScreenMode() ;
	minfo.col = mode >> 8 ;
	mode &= 0x7F ;
	if (*BIOSrows)
		minfo.row = *(BIOSrows) + 1 ;
	else
		minfo.row = 25 ;			// Bodge for AMSTRAD PCs

	if (mode == 7) {
		minfo.fbType = VGMT_MONOCHROME ;
		minfo.color = 1 ;
	} else if (mode == 0 || mode == 2) {
		minfo.fbType = VGMT_OTHER | VGMT_DISABLEBURST ;
		minfo.color = 4 ;
	} else if (mode < 4) {
		minfo.fbType = VGMT_OTHER ;
		minfo.color = 4 ;
	} else {
		minfo.fbType = VGMT_OTHER | VGMT_GRAPHICS ;
		minfo.color = 4 ;
	}

	minfo.hres = minfo.vres = 0 ;

	if (PtrMode->cb > 2) {
		PtrMode->fbType = minfo.fbType ;
		if (PtrMode->cb > 3) {
			PtrMode->color = minfo.color ;
			if (PtrMode->cb > 4) {
				PtrMode->col = minfo.col ;
				if (PtrMode->cb > 6) {
					PtrMode->row = minfo.row ;
					if (PtrMode->cb > 8) {
						PtrMode->hres = minfo.hres ;
						if (PtrMode->cb > 10) {
							PtrMode->vres = minfo.vres ;
							if (PtrMode->cb > 12) {
								PtrMode->fmt_ID = minfo.fmt_ID ;
								if (PtrMode->cb > 13) {
									PtrMode->attrib = minfo.attrib ;
								}
							}
						}
					}
				}
			}
		}
	}

	return NO_ERROR;
}

#pragma argsused
//
//	Set display mode
//
//	Known Bugs : 	Little (& incorrect) processing of graphics modes.
//
USHORT _APICALL
VioSetMode ( const VIOMODEINFO far *pminfo,
			 unsigned short VioHandle )
{
//	UCHAR oldmode = VioDosScreenMode() & 0xFF ;
	USHORT DCC, dipswitch, memsize ;
	int HasEGAVGA = !VioDosGetEGASettings(&dipswitch, &memsize) ;
	int HasVGA = !VioDosGetDCC(&DCC) ;

	volatile unsigned char far *BIOSmode =
		(volatile unsigned char far *)MK_FP(0x0040, 0x0049) ;
	volatile unsigned short far *BIOSflags =
		(volatile unsigned short far *)MK_FP(0x0040, 0x0087) ;

	//
	//	Don't support graphics or "non-compatible" modes
	//
	if (pminfo->fbType & (VGMT_GRAPHICS|0x80)) return ERROR_VIO_MODE ;

	//
	//	The resolution requested determines which adapter we require.
	//
	//		720x400, 720x480 imply VGA (DOS doesen't support 480)
	//		640x200, 320x200 imply EGA/CGA
	//		720x350 implies MDA
	//
	if (pminfo->hres == 720) {
		if (pminfo->vres == 350) {
			//
			//	MDA adapter requested.  We can only set 80x50 mono mode.
			//
			if (pminfo->fbType != VGMT_MONOCHROME ||
				pminfo->row != 25 || pminfo->col != 80 )
				return ERROR_VIO_MODE ;

			// Set the mode
			_AL = 0x07 ;
			_AH = 0x00 ;
			geninterrupt(0x10) ;

		} else if (pminfo->vres == 400) {
			//
			//	VGA adapter requested.  First issue a mode change according
			//	to whether the colour burst is enabled and the screen width,
			//	the select the character size according to the screen depth.
			//
			if (!HasVGA) return ERROR_VIO_MODE ;
			UCHAR newmode = NO_CLEAR_FLAG ;
			if (pminfo->fbType == VGMT_MONOCHROME)
				newmode += 7 ;
			else {
				if (pminfo->col == 80) {
					newmode += 2 ;
				} else if (pminfo->col != 40) {
					return ERROR_VIO_MODE ;
				}
				if (!(pminfo->fbType & VGMT_DISABLEBURST)) ++newmode ;
			}

			if (pminfo->row != 50 && pminfo->row != 28 && pminfo->row != 25)
				return ERROR_VIO_MODE ;

			//	Enable default pallette loading
			_AH = 0x12 ;
			_BL = 0x31 ;
			_AL = 0 ;
			geninterrupt(0x10) ;

			//	Enable/Disable grey-scaling on next mode switch
			_AL = (pminfo->fbType & VGMT_DISABLEBURST) ? 0 : 1 ;
			_AH = 0x12 ;
			_BL = 0x33 ;
			geninterrupt(0x10) ;

			// Set the mode
			_AL = newmode ;
			_AH = 0x00 ;
			geninterrupt(0x10) ;

			switch (pminfo->row) {
				case 50:
				_AX = 0x1112 ;				// Load & Display 8x8 characters
				_BX = 0 ;
				geninterrupt(0x10) ;
				break;
			case 28:
				_AX = 0x1111 ;				// Load & Display 8x14 characters
				_BX = 0 ;
				geninterrupt(0x10) ;
				break;
			case 25:
				_AX = 0x1114 ;				// Load & Display 8x16 characters
				_BX = 0 ;
				geninterrupt(0x10) ;
				break;
			}

			// Select alternate print screen
			_AX = 0x1200 ;
			_BL = 0x20 ;
			geninterrupt(0x10) ;

			*BIOSmode &= ~NO_CLEAR_FLAG ;
			*BIOSflags &= ~NO_CLEAR_FLAG ;

		} else
			return ERROR_VIO_MODE ;
	} else if (pminfo->vres == 200) {
		//
		//	EGA/CGA adapter requested.  First issue a mode change according
		//	to whether the colour burst is enabled and the screen width,
		//	the select the character size according to the screen depth.
		//
		if (!HasEGAVGA) return ERROR_VIO_MODE ;
		UCHAR newmode = NO_CLEAR_FLAG ;
		if (pminfo->col == 80) {
			newmode += 2 ;
		} else if (pminfo->col != 40) {
			return ERROR_VIO_MODE ;
		}
		if (!(pminfo->fbType & VGMT_DISABLEBURST)) ++newmode ;

		if (pminfo->row != 43 && pminfo->row != 25 )
			return ERROR_VIO_MODE ;

		switch (pminfo->row) {
			case 43:
			*BIOSflags |= 1 ;
			break;
		case 25:
			*BIOSflags &= ~1 ;
			break;
		}

		// Set the mode
		_AL = newmode ;
		_AH = 0x00 ;
		geninterrupt(0x10) ;

		switch (pminfo->row) {
			case 43:
			_AX = 0x1112 ;				// Load & Display 8x8 characters
			_BX = 0 ;
			geninterrupt(0x10) ;
			break;
		case 25:
			_AX = 0x1111 ;				// Load & Display 8x14 characters
			_BX = 0 ;
			geninterrupt(0x10) ;
			break;
		}

		*BIOSmode &= ~NO_CLEAR_FLAG ;
		*BIOSflags &= ~NO_CLEAR_FLAG ;

	} else
		return ERROR_VIO_MODE ;

	return NO_ERROR ;
}

