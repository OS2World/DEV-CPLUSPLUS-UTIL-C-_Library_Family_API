//
//	  *******************************************************************
//		JdeBP C++ Library Routines 	 	General Public Licence v1.00
//			Copyright (c) 1991,1992	 Jonathan de Boyne Pollard
//	  *******************************************************************
//

//
//	MS-DOS support for Family API DOS routines
//

struct DOSFind_t {
	unsigned char   drive;          // Undocumented DOS
	char            pattern[13];    // Undocumented DOS
	char            reserved[7];
	unsigned char   attrib;
	unsigned        wr_time;
	unsigned        wr_date;
	long            size;
	char            name[13];
};
struct psp {
	short int20;        // Old-style way of exiting program
	PARA memtop;        // Paragraph beyond the top of memory
	BYTE reserved1;
	BYTE cpmcall[5];    // FAR call to MS-DOS int dispatcher
	void far *int22;    // Program terminate address
	void far *int23;    // CTRL-BREAK address
	void far *int24;    // Critical error address
	PARA parentpsp;     // Paragraph of parent's psp
	BYTE handles[20];   // Indices into SFT
	PARA environment;   // Paragraph of environment
	void far *sss;      // Stack-Switch Storage
	short nhands;       // Number of handles
	BYTE far *htable;   // Address of handle table
	BYTE reserved2[24];
	BYTE dosint[3];     // INT21, then FAR RET
	BYTE reserved3[2];
	BYTE fcb1ext[7];    // FCB1 extension
	BYTE fbc1[16];      // FCB1
	BYTE fcb2[16];      // FCB2
	BYTE reserved4[4];
	BYTE cmdlen;        // Length of command line
	BYTE cmdline[127];  // Command line
} ;
struct mcb {
	BYTE type;   		// 4D for normal, 5A for end of block
	PARA owner;
	PARA size;
	BYTE unused[3];
	BYTE name[8];
} ;

extern "C" {

void _APICALL	DosDosSleep18	( unsigned long ) ;
void _APICALL	DosDosSetDTA 	(const void far *) ;
USHORT _APICALL	DosDosFindFirst (const char far *, unsigned short, struct DOSFind_t far *) ;
USHORT _APICALL	DosDosFindNext	(struct DOSFind_t far *) ;
USHORT _APICALL	DosDosDevIOCtl	(int, unsigned short, void far *, unsigned short, unsigned short far *) ;
USHORT _APICALL	DosDosOpen		(const char far *, unsigned short, unsigned short far *) ;
USHORT _APICALL	DosDosSelCurDrive (unsigned short, unsigned short far *) ;
USHORT _APICALL	DosDosQCurDrive (unsigned short far *) ;
USHORT _APICALL	DosDosQFileTime (int, unsigned short far *, unsigned short far *) ;
USHORT _APICALL	DosDosQPSPSeg	(unsigned short far *) ;
USHORT _APICALL	DosDosSetFileTime (int, unsigned short , unsigned short) ;
USHORT _APICALL	DosDosTmpnam	(char far *, unsigned short, char far * far *, unsigned short far *) ;
USHORT _APICALL	DosDosCreate	(const char far *, unsigned short, unsigned short far *) ;
USHORT _APICALL	DosDosWrite		(unsigned short, const void far *, unsigned short, unsigned short far *);
USHORT _APICALL	DosDosXOpen		(const char far *, unsigned short far *, unsigned int far *, unsigned int, unsigned int, unsigned int);
USHORT _APICALL	DosDosRestoreVectors	( void ) ;
USHORT _APICALL	DosDosSaveVectors	( void ) ;
void _APICALL	DosDosSetVect	( unsigned short, void interrupt (far *) () ) ;
void _APICALL	DosDosSleep		( unsigned short ) ;
void _APICALL	DosDosGetVect	( USHORT, void interrupt (far * far *PtrVecFN) () ) ;

}