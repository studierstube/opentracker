/* ======================================================================= */
/** source file defines the entry point for the application dll.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: ARlibrary.cpp 3427 2004-07-14 12:20:49Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "ARlibrary.h"
#include "windows.h"

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

