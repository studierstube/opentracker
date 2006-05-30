/***********************************************************************
*
*       File Name:      isense.h
*       Description:    Header File for InterSense Tracker Driver
*       Created:        12/4/99
*       Author:         Yury Altshuler
*       Contact:        yurya@isense.com, 781-270-0090 ext. 124
*
*       Copyright:      InterSense 1999 - All rights Reserved.
*
*       Comments:       This program supports IS-300, IS-600, IS-900 and InterTrax.
*                       It can detect, configure, and get data from up to 4 trackers.
*                       InterTrax does not require any cofiguration changes, so most
*                       of the functions can be ignored.
*
*                       
************************************************************************/
#ifndef _ISD_isenseh
#define _ISD_isenseh

/* if not Win32 then it must be UNIX, no other OS is supported  */
#if !defined __WIN32__ && !defined WIN32 && !defined _Windows && !defined _WIN32
#define UNIX 

/* Following defines the flavor of UNIX the program should be compiled for.
   Only one of the 3 lines can be used, the others should be commented out. */
#ifdef _SGI_SOURCE
#define IRIX      /* use SGI-specific serial port configuration */
/* #define SOLARIS */   /* SUN Solaris OS */
#else
#define LINUX
#endif
#endif

/* Serial port initialization varies for diffent vesions of IRIX operating systems.
   If you have an older SGI, remove the comments from around the following line. I don't
   have the actual models and IRIX version numbers, so it may be necessary to try both
   with and without this flag. */

/* #define LEGACY_SGI */


/* handle difference between little and big endian systems */
#if defined IRIX || defined SOLARIS
#define REVERSE_BYTE_ORDER
#endif

#if (defined IRIX && (defined LINUX || defined SOLARIS)) || (defined LINUX && defined SOLARIS)
#error More than one UNIX version is defined
#endif

/* maximum baud rate allowed by the operating system. Older SGI computers did not
   support 115200 */

#if defined UNIX && defined LEGACY_SGI
#define MAX_BAUD_RATE  38400L
#else
#define MAX_BAUD_RATE  115200L
#endif

/* -------------------------------------- */
/* Do not change anything below this line */
/* -------------------------------------- */

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#ifdef UNIX

typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef long                LONG;
typedef int                 INT;

#define ISENSE_API

#else /* Win32 platform */

#include <windows.h>

#ifdef ISENSE_EXPORTS
#define ISENSE_API __declspec(dllexport)
#else
#define ISENSE_API __declspec(dllimport)
#endif

#endif

typedef int                 IS_BOOL;

/* tracking system type */
#define ISD_NONE               0    /* none found, or can't identify */
#define ISD_PRECISION_SERIES   1    /* IS-300, IS-600 and IS-900 */
#define ISD_INTERTRAX_SERIES   2    /* InterTrax */

/* tracking system hardware model */
#define ISD_UNKNOWN     0    
#define ISD_IS300       1    /* 3DOF system */
#define ISD_IS600       2    /* 6DOF system */
#define ISD_IS900       3    /* 6DOF system */
#define ISD_INTERTRAX   4    /* InterTrax */

#define ISD_MAX_STATIONS       8

/* assume no more than 4 rs232 ports */
#define ISD_MAX_TRACKERS       4 

/* orientation format */
#define ISD_EULER              1
#define ISD_QUATERNION         2

/* number of supported stylus buttons */
#define MAX_NUM_BUTTONS        8

typedef int ISD_TRACKER_HANDLE;


typedef struct
{
    /* Following item are for information only and should not be changed */

    WORD   TrackerType;    /* IS Precision series or InterTrax. 
                              TrackerType can be: 
                              ISD_PRECISION_SERIES for IS-300, IS-600 and IS-900 model tracker, 
                              ISD_INTERTRAX_SERIES for InterTrax, or 
                              ISD_NONE if tracker is not initialized */

    WORD   TrackerModel;   /* ISD_UNKNOWN, ISD_IS300, ISD_IS600, ISD_IS900, ISD_INTERTRAX */
    
    WORD   CommPort;       /* Number of the rs232 port. Starts with 1. */

    /* Communications statistics. For information only. */

    WORD   RecordsPerSec;
    float  KBitsPerSec;    

    /* Following items are used to configure the tracker and can be set in
       the isenseX.ini file */

    WORD   SyncEvent;   /* 4 events: 0 - OFF, system is in free run 
                                     1 - ON, hardware genlock frequency is automatically 
                                         determined (if supported by the firmware)
                                     2 - ON, hardware genlock frequency is specified by the user
                                     3 - ON, no hardware sygnal, lock to the user specified frequency */ 

    float  SyncRate;    /* Sync frequency - number of hardware sync signals per second, 
                           or, if SyncEvent is 3 - data record output frequency */

    WORD   SyncPhase;   /* 0 to 100 per cent */

} ISD_TRACKER_TYPE;



/* ISD_STATION_CONFIG_TYPE can only be used with IS Precision Series tracking devices.
   If passed to ISD_SetStationEvent or ISD_GetStationEvent with InterTrax, FALSE is returned. */

typedef struct
{
    WORD    ID;             /* unique number identifying a station. It is the same as that 
                               passed to the ISD_SetStationEvent and ISD_GetStationEvent   
                               functions and can be 1 to ISD_MAX_STATIONS */

    IS_BOOL    Event;          /* TRUE if ON, FALSE if OFF */

    IS_BOOL    Compass;        /* 0, 1 or 2 for OFF, PARTIAL and FULL. Older versions of tracker
                               firmware supported only 0 and 1, which stood for ON or OFF. 
                               Please use the new notation. This API will correctly interpret 
                               the settings. Compass setting is ignored if station is configured 
                               for Fusion Mode operation */

    LONG    InertiaCube;    /* InertiaCube associated with this station. If no InertiaCube is
                               assigned, this number is -1. Otherwise, it is a positive number
                               1 to 4 */

    WORD    Enhancement;    /* levels 0, 1, or 2 */
    WORD    Sensitivity;    /* levels 1 to 4 */
    WORD    Prediction;     /* 0 to 50 ms */
    WORD    AngleFormat;    /* ISD_EULER or ISD_QUATERNION */
    IS_BOOL    TimeStamped;    /* TRUE if time stamp is requested */
    IS_BOOL    GetButtons;     /* TRUE if joystick of stylus button event is requested */

} ISD_STATION_CONFIG_TYPE;


typedef struct
{
    float Orientation[4];   /* Supports both Euler and Quaternion formats */
    float Position[3];      /* Always in meters */
    float TimeStamp;        /* Only if requested */
    int   ButtonEvent[MAX_NUM_BUTTONS];  /* Only if requested */

} ISD_STATION_DATA_TYPE;


typedef struct
{
    ISD_STATION_DATA_TYPE Station[ISD_MAX_STATIONS];

} ISD_DATA_TYPE;


/* Returns -1 on failure. To detect tracker automatically specify 0 for commPort */
#ifdef __cplusplus__
extern "C" {
#endif

ISENSE_API ISD_TRACKER_HANDLE ISD_OpenTracker( DWORD commPort, IS_BOOL infoScreen, IS_BOOL verbose );

ISENSE_API IS_BOOL  ISD_CloseTracker( ISD_TRACKER_HANDLE );

ISENSE_API IS_BOOL  ISD_GetTrackerEvent( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE *, IS_BOOL verbose );
ISENSE_API IS_BOOL  ISD_SetTrackerEvent( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE *, IS_BOOL verbose );


/* Get RecordsPerSec and KBitsPerSec without requesting genlock settings from the tracker.
   Use this instead of ISD_GetTrackerEvent to prevent your program from stalling while
   waiting for the tracker response. */

ISENSE_API IS_BOOL  ISD_GetCommEvent( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE * );


/* stationNum is a number from 1 to ISD_MAX_STATIONS */
ISENSE_API IS_BOOL  ISD_SetStationEvent( ISD_TRACKER_HANDLE, ISD_STATION_CONFIG_TYPE *, 
                           WORD stationNum, IS_BOOL verbose );

ISENSE_API IS_BOOL  ISD_GetStationEvent( ISD_TRACKER_HANDLE, ISD_STATION_CONFIG_TYPE *, 
                           WORD stationNum, IS_BOOL verbose );

/* Get data from all configured stations. */
ISENSE_API IS_BOOL  ISD_GetTrackerData( ISD_TRACKER_HANDLE, ISD_DATA_TYPE * );


/* Send a configuration script to the tracker. Script must consist of valid commands as
   described in the interface protocol. Commands in the script should be terminated by
   the New Line character '\n'. Line Feed character '\r' is added by the function and is
   not required. */

ISENSE_API IS_BOOL  ISD_SendScript( ISD_TRACKER_HANDLE, char * );
 
/* Returns number of configured tracking devices */
ISENSE_API IS_BOOL  ISD_NumOpenTrackers( WORD * );

#ifdef __cplusplus__
}
#endif

#endif
