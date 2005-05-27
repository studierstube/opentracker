/* ===========================================================================
    NAME:       trainer
    TYPE:       c++ code
    PROJECT:    Studierstube Applications - trainer
    CONTENT:    Defines the entry point for the DLL application.
   ===========================================================================
    AUTHORS:    bs      Benjamin A. Schmit
=========================================================================== */

#include "Trainer.h"
#include "trainer_win32.h"

bool APIENTRY DllMain( HANDLE hModule, 
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
    End of trainer_win32.cxx
=========================================================================== */
