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
//	Scan environment segment for VAR=VALUE pair
//
USHORT _APICALL
DosScanEnv	( const char far *PtrVarName,
			  const char far * far *PtrPtrResult )
{
	USHORT PSPSeg ;
	USHORT err = DosDosQPSPSeg(&PSPSeg) ;

	if (err) return err ;

	struct psp far *psp = (struct psp far *)MK_FP(PSPSeg, 0) ;

	char far *env = (char far *)MK_FP(psp->environment, 0) ;

	while (*env) {
		const char far *VarPtr = PtrVarName ;

		while (*VarPtr && *VarPtr == *env)
			VarPtr++, env++ ;

		if (!*VarPtr && *env == '=') {
			*PtrPtrResult = ++env ;	// Skip the '='
			return NO_ERROR ;
		} else
			while (*env++) ;		// Skip to start of next string
	}

	return ERROR_INVALID_NAME ;
}
