//
//	  *******************************************************************
//		JdeBP C++ Library Routines 	 	General Public Licence v1.00
//			Copyright (c) 1991,1992	 Jonathan de Boyne Pollard
//	  *******************************************************************
//

//
//	MS-DOS support for Family API VIO routines
//

extern "C" {

USHORT	_APICALL	VioDosScreenMode	( void ) ;
USHORT	_APICALL	VioDosGetCurPos		( void ) ;
void	_APICALL	VioDosSetCurPos		( unsigned short pos ) ;
USHORT	far * _APICALL	VioDosPointer	( unsigned short, unsigned short ) ;
USHORT	_APICALL	VioDosGetEGASettings( USHORT far *, USHORT far * ) ;
USHORT	_APICALL	VioDosGetDCC		( USHORT far * ) ;

}

//
//	Determine whether the current mode is a graphics mode
//
#define IS_GRAPH(m) ( ((m)&0xFF) > 0x03 && ((m)&0xFF) != 0x07)
