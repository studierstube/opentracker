/**********************************************************************************
*
*    File Name:      main.c
*    Description:    Illustrates use of the InterSense driver API
*    Created:        12/7/98
*    Author:         Yury Altshuler
*    Contact:        yurya@isense.com, 781-270-0090 ext. 124
*
*    Copyright:      InterSense 1999 - All rights Reserved.
*
*    Comments:       This program illustrates the use of the functions
*                    defined in isense.h. It can be conditionally compiled
*                    for Windows95/98/NT or UNIX. It has beed tested on
*                    Linux, Solaris and IRIX. To compile for a specific
*                    flavor of UNIX, uncomment appropriate line in the
*                    isense.h file.
*                    
*                    The only files that you need to look at are isense.h,  
*                    which contains function prototypes and data types, and  
*                    this  file (main.c), which illustrates the use of the  
*                    functions. This API is almost identical to that provided  
*                    by the InterSense DLL. Please do not modify any files other  
*                    than main.c. The only exception is serial.c, which may need 
*                    to be modified if your version of UNIX is not supported.  
*                    If you do so, please let me know at yurya@isense.com, so  
*                    that I may add support for your system to this program.
*
*                    Program works with all models of InterSense trackers,
*                    including the InterTrax. InterTrax initialization relies on 
*                    setting the RTS line, which, as far as I know, can't be  
*                    directly done in UNIX. Because of that, UNIX version sometimes  
*                    has trouble detecting the tracker. I recommend that you cycle  
*                    power on the InterTrax the first time you start the program.
*                    
**********************************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
 
#include "isense.h"
#include "isdriver.h"
#include "timer.h"

#if !defined UNIX
#include <conio.h>
#endif

#define ESC 0X1B


/*****************************************************************************
*
*   functionName:   showTrackerStats
*   Description:    
*   Created:        12/7/98
*   Author:         Yury Altshuler
*
*   Comments:       Get and display tracker information
*
******************************************************************************/
void showTrackerStats( ISD_TRACKER_HANDLE handle )
{
    ISD_TRACKER_TYPE Tracker;
    ISD_STATION_CONFIG_TYPE Station;
    WORD i, numStations = 4;
    char buf[20];

    char   *systemType[3] = {"Unknown", "IS Precision Series", "InterTrax Series"};
    char   *modelName[5]  = {"Unknown", "IS-300 Series", "IS-600 Series", "IS-900 Series", "InterTrax 30"};


    if(ISD_GetTrackerState( handle, &Tracker, TRUE ))
    {
        printf("\n********** InterSense Tracker Information ***********\n\n");

        printf("Type:     %s device on port %d\n", 
            systemType[Tracker.TrackerType], Tracker.CommPort);
        printf("Model:    %s\n", modelName[Tracker.TrackerModel]);

        if(Tracker.TrackerType == ISD_PRECISION_SERIES)
        {
            if( Tracker.TrackerModel == ISD_IS600 || Tracker.TrackerModel == ISD_IS900)
            {
                numStations = ISD_MAX_STATIONS;
            }
            printf("\nStation\tTime\tState\tCube  Enhancement  Sensitivity  Prediction\n");

            for(i = 1; i <= numStations; i++)
            {
                printf("%d\t", i);

                if(ISD_GetStationState( handle, &Station, i, FALSE ))
                {
                    sprintf(buf, "%d", Station.InertiaCube);
                    printf("%s\t%s\t%s\t   %u\t\t%u\t   %u\n", 
                        Station.TimeStamped ? "ON" : "OFF", 
                        Station.State ? "ON" : "OFF", 
                        Station.InertiaCube == -1 ? "None" : buf, 
                        Station.Enhancement, 
                        Station.Sensitivity, 
                        Station.Prediction);
                }
                else
                {
                    printf("ISLIB_GetStationState failed\n");
                    break;
                }
            }
            printf("\n");
        }
    }
    else
    {
        printf("ISLIB_GetTrackerState failed\n");
    }
}



/*****************************************************************************
*
*   functionName:   main
*   Description:    a simple main
*   Created:        12/7/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
int main()
{
    ISD_TRACKER_HANDLE       handle;
    ISD_DATA_TYPE            data;
    ISD_STATION_CONFIG_TYPE  Station;
    ISD_TRACKER_TYPE         Tracker;

    
    WORD done = FALSE, station = 1;
    IS_BOOL verbose = FALSE;
    float lastTime = 0.0f;
 

    /* Detect first tracker. If you have more than one InterSense device 
       and would like to have a specific tracker, connected to a known port, 
       initialized first, then enter the port number instead of 0. Otherwise, 
       tracker connected to the port with lower number is found first */
    
    handle = ISD_OpenTracker( 2, FALSE, verbose );  

    while(!done)
    {
#if !defined UNIX
        if(kbhit())
        {
            switch(getch())
            {
                case 'E':
                    ISD_GetStationState( handle, &Station, station, verbose );
                    Station.Enhancement++;
                    if(Station.Enhancement > 2) Station.Enhancement = 0;
                    ISD_SetStationState( handle, &Station, station, verbose );
                    showTrackerStats( handle );
                    break;
                
                case 'S':
                    ISD_GetStationState( handle, &Station, station, verbose );
                    Station.Sensitivity++;
                    if(Station.Sensitivity > 4) Station.Sensitivity = 1;
                    ISD_SetStationState( handle, &Station, station, verbose );
                    showTrackerStats( handle );
                    break;
                
                case 'I':
                    ISD_GetStationState( handle, &Station, station, verbose );
                
                    if(Station.InertiaCube == -1) Station.InertiaCube = 1;
                    else                          Station.InertiaCube++;
                
                    if(Station.InertiaCube > 4) Station.InertiaCube = -1;
                    ISD_SetStationState( handle, &Station, station, verbose );
                    showTrackerStats( handle );
                    break;
                
                case 'P':
                    ISD_GetStationState( handle, &Station, station, verbose );
                    Station.Prediction += 10;
                    if(Station.Prediction > 50) Station.Prediction = 0;
                    ISD_SetStationState( handle, &Station, station, verbose );
                    showTrackerStats( handle );
                    break;
                
                case 'X':
                    ISD_GetStationState( handle, &Station, station, verbose );
                    Station.State = !Station.State;
                    ISD_SetStationState( handle, &Station, station, verbose );
                    showTrackerStats( handle );
                    break;
                
                case 'T':
                    ISD_GetStationState( handle, &Station, station, verbose );
                    Station.TimeStamped = !Station.TimeStamped;
                    ISD_SetStationState( handle, &Station, station, verbose );
                    showTrackerStats( handle );
                    break;
                
                case 'V':
                    verbose = !verbose;
                    break;
                
                case '1':
                    station = 1;
                    break;
                
                case '2':
                    station = 2;
                    break;
                
                case '3':
                    station = 3;
                    break;
                
                case '4':
                    station = 4;
                    break;
                
                case 'D':
                    /* Get comm port statistics for display with tracker data */
                    if(ISD_GetCommState( handle, &Tracker ))
                    {
                        printf("%5.2fKbps %3d Updates/s %7.3f %7.3f %7.3f %7.2f %7.2f %7.2f\n",
                            Tracker.KBitsPerSec, Tracker.RecordsPerSec, 
                            data.Station[station-1].Position[0], 
                            data.Station[station-1].Position[1], 
                            data.Station[station-1].Position[2], 
                            data.Station[station-1].Orientation[0], 
                            data.Station[station-1].Orientation[1], 
                            data.Station[station-1].Orientation[2]);
                    }
                    break;
                
                case ESC:
                case 'q':
                case 'Q':
                    done = TRUE;
                    break;
                
                default:
                    printf("q -- quit\n");
                    printf("1 -- make station 1 current\n");
                    printf("2 -- make station 2 current\n");
                    printf("3 -- make station 3 current\n");
                    printf("4 -- make station 4 current\n");
                    printf("D -- display data\n");
                    printf("X -- toggle station state\n");
                    printf("I -- change InertiaCube assignement\n");
                    printf("E -- change enhancement mode\n");
                    printf("S -- change sensitivity\n");
                    printf("P -- change predition\n");
                    printf("V -- toggle verbose mode\n");
                    break;
            }
        }
#else

station = 3;

        if(handle > 0)
        {
            if(timeNow() - lastTime > 0.1f)
            {
                lastTime = timeNow();
                if(ISD_GetCommState( handle, &Tracker ))
                {
                    printf("%5.2fKbps %3d Updates/s %7.3f %7.3f %7.3f %7.2f %7.2f %7.2f\n",
                        Tracker.KBitsPerSec, Tracker.RecordsPerSec,
                        data.Station[station-1].Position[0], 
                        data.Station[station-1].Position[1], 
                        data.Station[station-1].Position[2], 
                        data.Station[station-1].Orientation[0], 
                        data.Station[station-1].Orientation[1],
                        data.Station[station-1].Orientation[2]);
                    fflush( stdout );
                }
            }
        }
        else return 0;
#endif
        ISD_GetTrackerData( handle, &data );
    }
    ISD_CloseTracker( handle );
    return 0;
}


