/* ===========================================================================
    NAME:       vacuum
    TYPE:       c++ code
    PROJECT:    Studierstube Applications - vacuum
    CONTENT:    Defines the entry point for the DLL application.
   ===========================================================================
    AUTHORS:    ib,tf      Istvan Barakonyi, Tamer Fahmy
   ===========================================================================
    HISTORY:

    15-Aug-03 01:55:42  ib      created
=========================================================================== */

#ifdef WIN32
#include <windows.h>
#endif

#include "vacuum.h"

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
    End of vacuum.cxx
=========================================================================== */
