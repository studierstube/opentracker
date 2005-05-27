/* ===========================================================================
    NAME:       projectionCalibration
    TYPE:       c++ code
    PROJECT:    Studierstube Applications - projectionCalibration
    CONTENT:    Defines the entry point for the DLL application.
   ===========================================================================
    AUTHORS:    jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    05-Feb-01 11:28:42  jp      last modification
    05-Feb-01 11:24:01  jp      created
=========================================================================== */

#include <windows.h>
#include "projectionCalibration.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

/* ===========================================================================
    End of projectionCalibration.cpp
=========================================================================== */
