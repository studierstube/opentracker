/***********************************************************************
*
*    File Name:      isense
*    Description:    DLL access point
*    Created:        12/7/98
*    Author:         Yury Altshuler
*
*    Copyright:      InterSense 1999 - All rights Reserved.
*
*                    
************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "isense.h"

static void printErrorMessage( DWORD error, LPCSTR lpProcName );
static FARPROC getProcAddress( HMODULE hModule, LPCSTR lpProcName );

typedef ISD_TRACKER_HANDLE (*ISLIB_OPEN_FN)           ( HWND, DWORD, BOOL, BOOL );
typedef BOOL               (*ISLIB_COMMAND_FN)        ( ISD_TRACKER_HANDLE );
typedef BOOL               (*ISLIB_COMM_INFO_FN)      ( ISD_TRACKER_HANDLE, ISD_TRACKER_INFO_TYPE * );
typedef BOOL               (*ISLIB_SYSTEM_CONFIG_FN)  ( ISD_TRACKER_HANDLE, ISD_TRACKER_INFO_TYPE *, BOOL );
typedef BOOL               (*ISLIB_STATION_CONFIG_FN) ( ISD_TRACKER_HANDLE, ISD_STATION_INFO_TYPE *, WORD, BOOL );
typedef BOOL               (*ISLIB_DATA_FN)           ( ISD_TRACKER_HANDLE, ISD_DATA_TYPE * );
typedef BOOL               (*ISLIB_SCRIPT_FN)         ( ISD_TRACKER_HANDLE, char * );
typedef BOOL               (*ISLIB_COUNT_FN)          ( WORD * );
typedef BOOL               (*ISLIB_RESET_ANGLES_FN)   ( ISD_TRACKER_HANDLE, float, float, float );
typedef HWND               (*ISLIB_WINDOW_FN)         ( ISD_TRACKER_HANDLE );

/* dll function pointers */
ISLIB_OPEN_FN           _ISLIB_OpenTracker       = NULL;
ISLIB_COMMAND_FN        _ISLIB_CloseTracker      = NULL;
ISLIB_COMM_INFO_FN      _ISLIB_GetCommInfo       = NULL;
ISLIB_SYSTEM_CONFIG_FN  _ISLIB_GetTrackerConfig  = NULL;
ISLIB_SYSTEM_CONFIG_FN  _ISLIB_SetTrackerConfig  = NULL;
ISLIB_STATION_CONFIG_FN _ISLIB_GetStationConfig  = NULL;
ISLIB_STATION_CONFIG_FN _ISLIB_SetStationConfig  = NULL;
ISLIB_DATA_FN           _ISLIB_GetTrackerData    = NULL;      
ISLIB_SCRIPT_FN         _ISLIB_SendScript        = NULL;
ISLIB_COUNT_FN          _ISLIB_NumOpenTrackers   = NULL;
ISLIB_WINDOW_FN         _ISLIB_OpenConfigWindow  = NULL;
ISLIB_RESET_ANGLES_FN   _ISLIB_ResetAngles       = NULL; 
 
/* dll handle */  
HINSTANCE ISLIB_lib = NULL;


/*****************************************************************************
*
*   functionName:   getProcAddress
*   Description:    loads specified dll routine
*   Created:        7/25/99
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static FARPROC getProcAddress( HMODULE hModule, LPCSTR lpProcName )
{
    FARPROC proc;

    proc = GetProcAddress( hModule, lpProcName );
    if(proc == NULL)
    {
        printf("Failed to load %s. Error code %d\n", lpProcName, GetLastError());
    }
    return proc;
}


/*****************************************************************************
*
*   functionName:   load_ISLIB
*   Description:    loads isense.dll
*   Created:        12/7/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static HINSTANCE load_ISLIB( void )
{
    HINSTANCE hLib;

    if(hLib = LoadLibrary ( "isense.dll" ))
    {
        _ISLIB_OpenTracker      = ( ISLIB_OPEN_FN )           getProcAddress( hLib, "ISD_OpenTracker" );
        _ISLIB_CloseTracker     = ( ISLIB_COMMAND_FN )        getProcAddress( hLib, "ISD_CloseTracker" );
        _ISLIB_GetCommInfo      = ( ISLIB_COMM_INFO_FN )      getProcAddress( hLib, "ISD_GetCommInfo" );
        _ISLIB_GetTrackerConfig = ( ISLIB_SYSTEM_CONFIG_FN )  getProcAddress( hLib, "ISD_GetTrackerConfig" );
        _ISLIB_SetTrackerConfig = ( ISLIB_SYSTEM_CONFIG_FN )  getProcAddress( hLib, "ISD_SetTrackerConfig" );
        _ISLIB_GetStationConfig = ( ISLIB_STATION_CONFIG_FN ) getProcAddress( hLib, "ISD_GetStationConfig" );
        _ISLIB_SetStationConfig = ( ISLIB_STATION_CONFIG_FN ) getProcAddress( hLib, "ISD_SetStationConfig" );
        _ISLIB_GetTrackerData   = ( ISLIB_DATA_FN )           getProcAddress( hLib, "ISD_GetTrackerData" );
        _ISLIB_SendScript       = ( ISLIB_SCRIPT_FN )         getProcAddress( hLib, "ISD_SendScript" );
        _ISLIB_NumOpenTrackers  = ( ISLIB_COUNT_FN )          getProcAddress( hLib, "ISD_NumOpenTrackers" );
        _ISLIB_OpenConfigWindow = ( ISLIB_WINDOW_FN )         getProcAddress( hLib, "ISD_OpenConfigWindow" );
        _ISLIB_ResetAngles      = ( ISLIB_RESET_ANGLES_FN )   getProcAddress( hLib, "ISD_ResetAngles" );
    }

    if(hLib == NULL)
    {
        printf("Could not load isense.dll\n");
        printErrorMessage(GetLastError(), "LoadLibrary");
    }

    return hLib;
}


/*****************************************************************************
*
*   functionName:   printErrorMessage
*   Description:    
*   Created:        7/25/99
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static void printErrorMessage( DWORD error, LPCSTR lpProcName )
{
    switch(error)
    {
        case ERROR_DLL_INIT_FAILED:
            printf("A DLL initialization routine failed.\n");
            break;
        case ERROR_INVALID_FUNCTION:
            printf("The function is incorrect.\n");
            break;
        case ERROR_FILE_NOT_FOUND:
            printf("The system cannot find the file specified.\n");
            break;
        case ERROR_PATH_NOT_FOUND:
            printf("The system cannot find the specified path.\n");
            break;
        case ERROR_TOO_MANY_OPEN_FILES:
            printf("The system cannot open the file.\n");
            break;
        case ERROR_ACCESS_DENIED:
            printf("Access is denied.\n");
            break;
        default:
            printf("%s failed. Error code %d\n", lpProcName, error); 
            break;
    }
}


/*****************************************************************************
*
*   functionName:   free_ISLIB
*   Description:    frees isense.dll
*   Created:        12/8/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static void free_ISLIB( HINSTANCE hLib )
{
    _ISLIB_OpenTracker      = NULL;
    _ISLIB_CloseTracker     = NULL;
    _ISLIB_GetCommInfo      = NULL;
    _ISLIB_GetTrackerConfig = NULL;
    _ISLIB_SetTrackerConfig = NULL;
    _ISLIB_GetStationConfig = NULL;
    _ISLIB_SetStationConfig = NULL;
    _ISLIB_GetTrackerData   = NULL;      
    _ISLIB_SendScript       = NULL;
    _ISLIB_NumOpenTrackers  = NULL;
    _ISLIB_OpenConfigWindow = NULL;
    _ISLIB_ResetAngles      = NULL;

	FreeLibrary ( hLib );  /* free the dll */
}


/******************************************************************************/
ISD_TRACKER_HANDLE ISLIB_OpenTracker( HWND hParent, DWORD commPort, 
                                      BOOL infoScreen, BOOL verbose )
{
    if(!_ISLIB_OpenTracker) /* this will be NULL if dll not loaded */
    {
        ISLIB_lib = load_ISLIB();

        if(!ISLIB_lib)  /* failed to load dll */
        {
            return 0;
        }
    }
    return((*_ISLIB_OpenTracker)( hParent, commPort, infoScreen, verbose ));
}


/******************************************************************************/
BOOL  ISLIB_CloseTracker( ISD_TRACKER_HANDLE handle )
{
    BOOL ret;
    WORD num;

    if(_ISLIB_CloseTracker)
    {
        ret = (*_ISLIB_CloseTracker)( handle );

        /* if all trackers are closed the dll can be freed */
        if(ISLIB_NumOpenTrackers( &num ))
        {
            if(num == 0)
            {
                free_ISLIB( ISLIB_lib );
                ISLIB_lib = NULL;
            }
        }
        return ret;
    }
    return FALSE;
}


/******************************************************************************/
BOOL ISLIB_NumOpenTrackers( WORD *num )
{
    if(_ISLIB_NumOpenTrackers) 
    {
        return((*_ISLIB_NumOpenTrackers)( num ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_GetCommInfo( ISD_TRACKER_HANDLE handle, ISD_TRACKER_INFO_TYPE *Tracker )
{
    if(_ISLIB_GetCommInfo)
    {
        return((*_ISLIB_GetCommInfo)( handle, Tracker ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_GetTrackerConfig( ISD_TRACKER_HANDLE handle, 
                             ISD_TRACKER_INFO_TYPE *Tracker, BOOL verbose )
{
    if(_ISLIB_GetTrackerConfig)
    {
        return((*_ISLIB_GetTrackerConfig)( handle, Tracker, verbose ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_SetTrackerConfig( ISD_TRACKER_HANDLE handle, 
                              ISD_TRACKER_INFO_TYPE *Tracker, BOOL verbose )
{
    if(_ISLIB_SetTrackerConfig)
    {
        return((*_ISLIB_SetTrackerConfig)( handle, Tracker, verbose ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_SetStationConfig( ISD_TRACKER_HANDLE handle, 
                              ISD_STATION_INFO_TYPE *Station, WORD stationNum, BOOL verbose )
{
    if(_ISLIB_SetStationConfig)
    {
        return((*_ISLIB_SetStationConfig)( handle, Station, stationNum, verbose ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_GetStationConfig( ISD_TRACKER_HANDLE handle, 
                              ISD_STATION_INFO_TYPE *Station, WORD stationNum, BOOL verbose )
{
    if(_ISLIB_GetStationConfig)
    {
        return((*_ISLIB_GetStationConfig)( handle, Station, stationNum, verbose ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_GetTrackerData( ISD_TRACKER_HANDLE handle, ISD_DATA_TYPE *Data )
{
    if(_ISLIB_GetTrackerData)
    {
        return((*_ISLIB_GetTrackerData)( handle, Data ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_SendScript( ISD_TRACKER_HANDLE handle, char *script )
{
    if(_ISLIB_SendScript)
    {
        return((*_ISLIB_SendScript)( handle, script ));
    }
    return FALSE;
}


/******************************************************************************/
BOOL  ISLIB_ResetAngles( ISD_TRACKER_HANDLE handle, float yaw, float pitch, float roll )
{
    if(_ISLIB_ResetAngles) 
    {
        return((*_ISLIB_ResetAngles)( handle, yaw, pitch, roll ));
    }
    return FALSE;
}


/******************************************************************************/
HWND  ISLIB_OpenConfigWindow( ISD_TRACKER_HANDLE handle )
{
    if(_ISLIB_OpenConfigWindow)
    {
        return((*_ISLIB_OpenConfigWindow)( handle ));
    }
    return NULL;
}

