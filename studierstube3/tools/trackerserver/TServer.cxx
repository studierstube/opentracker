// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       TServer
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    implementation of Tserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:09:43  rr      last modification
// ===========================================================================

#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <stdio.h>
#include <termios.h>
#include <termio.h>
#include <curses.h>
#include <ctype.h>
#include "GlobalDef.h"
#include <util/os.h>
#include <util/option.h>
#include "NetComm.h"
#include "ParComm.h"
#include "CfgOpts.h"
#include "Fob.h"
#include "Polhemus.h"
#include "NewKFilt.h"

//----------------------------------------------------------------------------

int reverse_qmap[3];
int world_x = 0;    // is track x per default
int world_y = 1;    // is track y per default
int world_z = 2;    // is track z per default
int world_qx = 0;   // is track x per default
int world_qy = 1;   // is track y per default
int world_qz = 2;   // is track z per default


char serialBaudRate[256]; // baud rate for fob - added ds2000-03-24
ParRecord parRec[8];
int ercId; // additional ERC bird number - added gr2000-12-05

//----------------------------------------------------------------------------


/*** function initCurses
 *
 * Purpose: Initializes the screen used for outputting tracker information
 */

void initCurses()
{
    initscr();
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr,FALSE);
    keypad(stdscr,TRUE);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);                     //switch off the cursor display
}


/*** function initScreen
 *
 * Purpose: Clears the screen and writes the header info
 * In:      toAddr:   Pointer to a string holding the address of the
 *                    multicastgroup
 *          toPort:   Pointer to a string holding the port of the
 *                    multicastgroup
 */

void initScreen(char* toAddr, char* toPort, StationRecord* station, int stNr)
{
    clear();
    move(0,0);
    printw(ABOUT);
    printw("Servername: %s\n",station[0].servName);
    printw("Sending to address '%s', port '%s'\n",toAddr, toPort); 

    for (int i=0; i<stNr; i++)
    {
        move(4+i,51);
        printw(station[i].name);
    }

    move(5+stNr,0);
    printw("<q> quit   <r> restart");
    refresh();
}


/*** function printBinary
 *
 * Purpose: Prints a byte in binary format
 * In:      b: byte value to be printed in binary format
 */

void printBinary(int b)
{
    char bin[10];
    int i;

    for (i=0; i<8; i++)
        bin[i+(i/4)]=(b&(128>>i)?'*':'.');
    bin[9]='\0';
    bin[4]=' ';
    printw(bin);
}


/*** function printRate
 *
 * Purpose: Prints the update rate of the stations in Hz.
 * In:      n: Number of stations being used
 * In/Out:  ticks:    Counts how often this function has been called and is
 *                    used to output information to the screen every 100th
 *                    call
 *          lastTime: Pointer to the time that was being measured in the last
 *                    call of this function
 *          station:  Pointer to the record of the station used to keep track
 *                    of its properties
 */

void printRate(int *ticks, double *lastTime, StationRecord *station, int n)
{
    if ((*ticks)==100)
    {
        (*ticks) = 0;
        double currentTime=osTimeOfDay()/1000.0;
        double dt=currentTime-(*lastTime);

        if (dt>=0.1) refresh();
        if (dt>=1)
        {
            for (int i=0; i<n; i++)
            {
                move(4+i,0);
                printw("#%d: %4.1fHz",i+1,
                       ((double)station[i].received)/dt);
                station[i].received = 0;

                if (station[i].error>0)
                {
                    station[i].error--;
                    if (station[i].error==0)
                    {
                        move(4+i,0);
                        printw("           ");
                    }
                }
            }
            (*lastTime)=currentTime;
        }
    }
    (*ticks)++;
}


/*** function chkInput
 *
 * Purpose: Checks for user input and quits the trackerserver with different
 *          exitcodes accordingly. 'q' quits with exit code 0. 'r' quits with
 *          exit code '1'.
 * In:      station: Pointer to the record of the stations holding the
 *                   filedescriptor of the station    
 */

void chkInput(StationRecord* station, int mode)
{
    char inputch;

    /* check for user input */
    if ((inputch=getch())!=ERR)
    { 
        switch (tolower(inputch))
        {
            case 'r':
            if (mode != parButton) birdSleep(station[0].fd);
            endwin();
            exit(1);

            case 'q':
            if (mode != parButton) birdSleep(station[0].fd);
            endwin();
            exit(0);
        }
    }
}


/*** function printPosButton
 *
 * Purpose: Prints the position and button state of a station
 * In:      i:        Number of the station
 *          position: Pointer to an array of floats containing the position
 *                    of the station
 *          buttons:  Byte holding the button bits
 */

void printPosButton(int i, float *position, char buttons)
{
    move(4+i,13);
    printw("X=%5.2f",position[0]);
    move(4+i,22);
    printw("Y=%5.2f",position[1]);
    move(4+i,31);
    printw("Z=%5.2f",position[2]);
    move(4+i,40);
    printBinary(buttons);
}


/*** function scalePosition
 *
 * Purpose: Convert position bytes from Ascension into actual position values
 * In:      station:  Pointer to the record of the station used for
 *                    scaling the position
 *          stNr:     Number of station of the position dealt with
 * In/Out:  position: Pointer to an array of floats for the position to
 *                    be put in
 * Globals: world_x, world_y, world_z
 */

void scalePosition(float *position, StationRecord* station, int stNr)
{
    float f[3];

    f[world_x]=position[0];
    f[world_y]=position[1];
    f[world_z]=position[2];
    position[0]=f[0];
    position[1]=f[1];
    position[2]=f[2];

    position[world_x] = position[world_x]*station[stNr].scaleX;
    position[world_y] = position[world_y]*station[stNr].scaleY;
    position[world_z] = position[world_z]*station[stNr].scaleZ;
}


/*** function revertQuaternion
 *
 * Purpose: Negates and rearranges the quaternion components if necessary
 * In/Out:  orientation: Pointer to an array of floats containing the
 *                       Quaternion
 * Globals: world_qx, world_qy, world_qz
 */

void revertQuaternion(float* orientation)
{
    float f[3];

    f[world_qx]=orientation[0+1];
    f[world_qy]=orientation[1+1];
    f[world_qz]=orientation[2+1];
    orientation[0+1]=f[0];
    orientation[1+1]=f[1];
    orientation[2+1]=f[2];
   
    if (reverse_qmap[0]) orientation[world_qx+1] = -orientation[world_qx+1];
    if (reverse_qmap[1]) orientation[world_qy+1] = -orientation[world_qy+1];
    if (reverse_qmap[2]) orientation[world_qz+1] = -orientation[world_qz+1];
}


/*** function initStationRecord
 *
 * Purpose: Initializes several vars and properties of the stations
 * In:      numOfStations: Number of stations being used
 * In/Out:  station:       Pointer to the record of the stations used to keep
 *                         track of its properties
 * Globals: world_x, world_y, world_z
 */

void initStationRecord(StationRecord* station, int numOfStations)
{
    float f[3];
    int i;

    for (i=0; i<numOfStations; i++)
    {
        station[i].pos = 0;
        station[i].received = 0;
        station[i].error = 0;
        station[i].rawButton = 0;
        station[i].sent = 0;

        f[world_x]=station[i].scaleX;
        f[world_y]=station[i].scaleY;
        f[world_z]=station[i].scaleZ;
        station[i].scaleX=f[0];
        station[i].scaleY=f[1];
        station[i].scaleZ=f[2];
    }
}


/*** function testFile
 *
 * Purpose: Tries to open the initfile and exits when if it does not succeed.
 *          This is necessary because the cfgopts do not perform such a check.
 * In:      initFilename: Pointer to a string containing the file to be
 *                        opened.
 */

void testFile(char* initFilename)
{
    FILE *testfile;

    testfile=fopen(initFilename,"r");
    if (testfile==NULL)
    {
        fprintf(stderr,"Error opening file '%s'.\n",initFilename);
        exit(1);
    }
    fclose(testfile);
}


/*** function readGeomSection
 *
 * Purpose: Parses the assignment of the tracker axis to the world axis from
 *          the initfile
 * In:      Pointer to a string containing the filename of the initfile
 * Globals: reverse_qmap[3], world_x, world_y, world_z, world_qx, world_qy,
 *          world_qz
 */

void readGeomSection(char* initFilename)
{
    int reverse_qx = 0;
    int reverse_qy = 0;
    int reverse_qz = 0;

    char world_xstr[256]="track_x";
    char world_ystr[256]="track_y";
    char world_zstr[256]="track_z";

    char world_qxstr[256]="track_qx";
    char world_qystr[256]="track_qy";
    char world_qzstr[256]="track_qz";
    char reverse_qxstr[256]="off";
    char reverse_qystr[256]="off";
    char reverse_qzstr[256]="off";

    struct Config_Tag configs[] =
    {
        { "world_x", String_Tag, world_xstr },
        { "world_y", String_Tag, world_ystr },
        { "world_z", String_Tag, world_zstr },
        { "world_qx", String_Tag, world_qxstr },
        { "world_qy", String_Tag, world_qystr },
        { "world_qz", String_Tag, world_qzstr },
        { "reverse_qx", String_Tag, reverse_qxstr },
        { "reverse_qy", String_Tag, reverse_qystr },
        { "reverse_qz", String_Tag, reverse_qzstr },
        { NULL , Error_Tag, NULL        } // Terminating record
    };


    (void) input_config(initFilename,configs,"[geometry]");

    if (strcasecmp(reverse_qxstr,"on") == 0) { reverse_qx = 1; }
    if (strcasecmp(reverse_qystr,"on") == 0) { reverse_qy = 1; }
    if (strcasecmp(reverse_qzstr,"on") == 0) { reverse_qz = 1; }


    if (strncasecmp(world_xstr,"track_",6) == 0)
    {
        if (world_xstr[6] == 'x') { world_x=0; }
        if (world_xstr[6] == 'y') { world_x=1; }
        if (world_xstr[6] == 'z') { world_x=2; }
    }
    else
    {
        fprintf(stderr,"track_[xyz] expected\n");
        exit(1);
    }

    if (strncasecmp(world_ystr,"track_",6) == 0)
    {
        if (world_ystr[6] == 'x') { world_y=0; }
        if (world_ystr[6] == 'y') { world_y=1; }
        if (world_ystr[6] == 'z') { world_y=2; }
    }
    else
    {
        fprintf(stderr,"track_[xyz] expected\n");
        exit(1);
    }

    if (strncasecmp(world_zstr,"track_",6) == 0)
    {
        if (world_zstr[6] == 'x') { world_z=0; }
        if (world_zstr[6] == 'y') { world_z=1; }
        if (world_zstr[6] == 'z') { world_z=2; }
    }
    else
    {
        fprintf(stderr,"track_[xyz] expected\n");
        exit(1);
    }


    if (strncasecmp(world_qxstr,"track_q",7) == 0)
    {
        if (world_qxstr[7] == 'x') { world_qx=0; }
        if (world_qxstr[7] == 'y') { world_qx=1; }
        if (world_qxstr[7] == 'z') { world_qx=2; }
    }
    else
    {
        fprintf(stderr,"track_q[xyz] expected\n");
        exit(1);
    }

    if (strncasecmp(world_qystr,"track_q",7) == 0)
    {
        if (world_qystr[7] == 'x') { world_qy=0; }
        if (world_qystr[7] == 'y') { world_qy=1; }
        if (world_qystr[7] == 'z') { world_qy=2; }
    }
    else
    {
        fprintf(stderr,"track_q[xyz] expected\n");
        exit(1);
    }

    if (strncasecmp(world_qzstr,"track_q",7) == 0)
    {
        if (world_qzstr[7] == 'x') { world_qz=0; }
        if (world_qzstr[7] == 'y') { world_qz=1; }
        if (world_qzstr[7] == 'z') { world_qz=2; }
    }
    else
    {
        fprintf(stderr,"track_q[xyz] expected\n");
        exit(1);
    }

    //--- init reverse map
    reverse_qmap[world_qx] = reverse_qx;
    reverse_qmap[world_qy] = reverse_qy;
    reverse_qmap[world_qz] = reverse_qz;
}


/*** function initParallelButtons
 *
 * Purpose: Reads the names of the parallel devices used for button input
 *          from the initfile and opens those ports that are specified
 * In:      initFilename: Pointer to a string holding the filename of the
 *                        initfile
 * Globals: parRec
 */

void initParallelButtons(char* initFilename)
{
    char parallel_devname[8][256];
    int i;

    struct Config_Tag configs[] =
    {
        { "parallel1_devname", String_Tag, parallel_devname[0] },
        { "parallel2_devname", String_Tag, parallel_devname[1] },
        { "parallel3_devname", String_Tag, parallel_devname[2] },
        { "parallel4_devname", String_Tag, parallel_devname[3] },
        { "parallel5_devname", String_Tag, parallel_devname[4] },
        { "parallel6_devname", String_Tag, parallel_devname[5] },
        { "parallel7_devname", String_Tag, parallel_devname[6] },
        { "parallel8_devname", String_Tag, parallel_devname[7] },
        { "baseport1", HexWord_Tag, &parRec[0].basePort },
        { "baseport2", HexWord_Tag, &parRec[1].basePort },
        { "baseport3", HexWord_Tag, &parRec[2].basePort },
        { "baseport4", HexWord_Tag, &parRec[3].basePort },
        { "baseport5", HexWord_Tag, &parRec[4].basePort },
        { "baseport6", HexWord_Tag, &parRec[5].basePort },
        { "baseport7", HexWord_Tag, &parRec[6].basePort },
        { "baseport8", HexWord_Tag, &parRec[7].basePort },
        { NULL , Error_Tag, NULL        }   // Terminating record
    };

    for (i=0; i<8; i++)
    {
        parRec[i].basePort=0;
        strcpy(parallel_devname[i],"\0");
    }
   
    (void) input_config(initFilename,configs,"[common]");

    for (i=0; i<8; i++)
    {
        if (strlen(parallel_devname[i]) != 0)
        {
            printf("Initialising parallel interface %s at port 0x%x\n",
                   parallel_devname[i],parRec[i].basePort);

            parRec[i].fdParallel=openParallelPort(parallel_devname[i],
                                                   parRec[i].basePort);
        }
        else
        {
            parRec[i].fdParallel=0;
        }
    }
}


/*** function checkButton
 *
 * Purpose: Parses the 'button' option from the configfile
 * In:      buttons: Pointer to string of the option
 *          stNr:    Number of station dealt with
 *          buttNr:  Number of the button of the station dealt with
 *          fdCount: Number of available stations
 * In/Out:  station: Pointer to the record of the station used to keep track
 *                   of its properties
 */

void checkButton(char* buttons,StationRecord* station,int stNr, int buttNr,
                 int fdCount)
{
    char c;
    char d[2]="X";
    int i,j;

    if (strcasecmp(buttons,"None") != 0)
    {
        if (strlen(buttons) < 4)
        {
            fprintf(stderr,"Insufficient arguments at 'buttons' option.\n");
            exit(1);
        }
        if (buttons[2] != ';')
        {
            fprintf(stderr,"';' expected at 'buttons' option.\n");
            exit(1);
        }

        c=buttons[0];
        d[0]=buttons[1];
        i=atoi(d);
        d[0]=buttons[3];
        j=atoi(d);

        d[0]='X';
        if (strlen(buttons) > 4) { d[0]=buttons[4]; }

#ifdef DEBUG_DATA
        fprintf(stderr,"[station %d] button%d=%c %d %d %c\n",
                stNr+1,buttNr+1,c,i,j,d[0]);
#endif

        if ((c=='S') || (c=='s'))
        {
            if ( (i<=0) || (i>fdCount) )
            {
                fprintf(stderr,"Invalid station number supplied at 'buttons' "
                               "option in section [station %d].\n",stNr+1);
                exit(1);
            }
            station[i-1].buttonMode=buttonTracker;

            if ( (j>0) && (j<=8) )
            {
                station[stNr].buttons[buttNr]=j+100*i;
            }
            else
            {
                fprintf(stderr,"Invalid bitnumber supplied at 'buttons' "
                               "option in section [station %d].\n",stNr+1);
                exit(1);
            }
        }

        if ((c=='P') || (c=='p'))
        {
            if ( (i<=0) || (i>8) )
            {
                fprintf(stderr,"Invalid parallel number supplied at 'buttons' "
                               "option in section [station %d].\n",stNr+1);
                exit(1);
            }

            if ( (j>0) && (j<=8) )
            {
                station[stNr].buttons[buttNr]=j+10000*i;
            }
            else
            {
                fprintf(stderr,"Invalid bitnumber supplied at 'buttons' "
                               "option in section [station %d].\n",stNr+1);
                exit(1);
            }
        }

        if ( (d[0]=='i') || (d[0]=='I') )           // invert
        {
            station[stNr].buttons[buttNr] = -(station[stNr].buttons[buttNr]);
        }
    }
}


/*** function checkPosFilter
 *
 * Purpose: Parses the 'position_filter' option from the configfile
 * In:      filter:     Pointer to string of the option 'position_filter'
 *          stationNum: Number of the station dealt with
 * In/Out:  station:    Pointer to the record of the station used to keep
 *                      track of its properties
 */

void checkPosFilter(char* filter, StationRecord* station, int stationNum)
{
    station[stationNum].positionFilter = noFilter;
    int i=station[stationNum].positionFilterParam;

    if (strcasecmp(filter,"Box_Filter") == 0)
    {
        if ( (i > 1) && (i <= filterMax) )
        { station[stationNum].positionFilter = boxFilter; }
        else
        {
            fprintf(stderr,"Position_Filter_Param (%d) is out of range at "
                           "[station %d].\n",i,stationNum+1);
            exit(1);
        }
    }   

    if (strcasecmp(filter,"Aniso_Filter") == 0)
    {
        if ( station[stationNum].mode != positionQuaternion )
        {
            fprintf(stderr,"Aniso_Filter requires 'format' option to be ");
            fprintf(stderr,"Position_Quaternion at [station %d].\n",
                    stationNum+1);
            exit(1);
        }

        if ( (i > 1) && (i <= filterMax) )
        {
            station[stationNum].positionFilter = anisoFilter;
        }
        else
        {
            fprintf(stderr,"Position_Filter_Param (%d) is out of range at "
                           "[station %d].\n",i,stationNum+1);
            exit(1);
        }
    }   
}


/*** function checkOrientFilter
 *
 * Purpose: Parses the 'orientation_filter' option from the configfile
 * In:      filter:     Pointer to string of the option 'orientation_filter'
 *          stationNum: Number of the station dealt with
 * In/Out:  station:    Pointer to the record of the station used to keep track
 *                      of its properties
 */

void checkOrientFilter(char* filter, StationRecord* station, int stationNum)
{
    station[stationNum].orientationFilter = noFilter;

    if (strcasecmp(filter,"Box_Filter") == 0)
    {
        int i=station[stationNum].orientationFilterParam;
        if ( (i > 1) && (i <= filterMax) )
        {
            station[stationNum].orientationFilter = boxFilter;
        }
        else
        {
            fprintf(stderr,"Orientation_Filter_Param (%d) is out of "
                           "range.\n",i);
            exit(1);
        }
    }

    if (strcasecmp(filter,"Kalman_Filter") == 0)
    {
        station[stationNum].orientationFilter = kalmanFilter;
    }
}


/*** function printStationFilter
 *
 * Purpose: Writes information on software filters being enable for each
 *          individual station
 * In:      num:     Number of stations
 *          station: Pointer to the record of the station used to keep
 *                   track of its properties
 */

void printStationFilter(StationRecord* station, int num)
{
    for (int i=0; i<num; i++)
    {
        printf("Station %d position software filter: ",i+1);

        switch (station[i].positionFilter)
        {
            case noFilter:     printf("No filter\n"); break;
            case boxFilter:    printf("Box filter\n"); break;
            case anisoFilter:  printf("Aniso filter\n"); break;
        }
    }
    printf("\n");

    for (int i=0; i<num; i++)
    {
        printf("Station %d orientation software filter: ",i+1);
        switch (station[i].orientationFilter)
        {
            case noFilter:     printf("No filter\n"); break;
            case boxFilter:    printf("Box filter\n"); break;
            case kalmanFilter: printf("Kalman filter\n"); break;
        }
    }
    printf("\n");
}


/*** function getNumOfTrackers
 *
 * Purpose: Reads the number of tracking devices from the initfile
 * In:      initFilename: Pointer to a string containing the name of the
 *                        initfile
 * Returns: the number of tracking devices
 */

int getNumOfTrackers(char* initFilename, int mode)
{
    int numOfStations=0;

    struct Config_Tag configs[] =
    {
        { "tracker", Word_Tag, &numOfStations },
        { NULL , Error_Tag, NULL        }   // Terminating record
    };

    (void) input_config(initFilename,configs,"[common]");    

    if ( ( mode == fobSingle ) || ( mode == fobMulti) )
    {
        if ( numOfStations<1 || numOfStations>maxBirds)
        {
            fprintf(stderr,"Invalid amount of Birds specified in [common].\n"
                           "Max. 14 Birds are supported, not %d.\n",
                            numOfStations);
            exit(1);
        }
    }
    else
    {
        if ( numOfStations<1 )
        {
            fprintf(stderr,"Invalid amount of trackers specified in "
                           "[common].\n");
            exit(1);
        }
    }

    return(numOfStations);
}


/*** function checkFormat
 *
 * Purpose: Parses the 'format' option from the configfile
 * In:      format:     Pointer to string of the option
 *          stationNum: Number of the station dealt with
 * In/Out:  station: Pointer to the record of the station used to keep track
 *                   of its properties
 */

void checkFormat(char* format, StationRecord* station, int stationNum)
{
    if (strcasecmp(format,"Position_Quaternion") == 0)
    {
        station[stationNum].mode = positionQuaternion;
    }
    else
    {
        if (strcasecmp(format,"Position_Angles") == 0)
        {
            station[stationNum].mode = positionAngles; }
        else
        {
            if (strcasecmp(format,"Position_Matrix") == 0)
            {
                station[stationNum].mode = positionMatrix;
            }
            else
            {
                fprintf(stderr,"Format can only be Position_Quaternion, "
                               "Position_Angles or Position_Matrix, not "
                               "'%s'.\n",format);
                exit(1);
            }
        }
    }   
}


/*** function readInitfile
 *
 * Purpose: Reads options from the configfile which are independent from the
 *          tracker hardware
 * In:      initFilename: Pointer to a string containing the file to be
 *                        opened
 *          n:            Number of stations
 *          mode:         Mode of operation of the trackerserver
 * In/Out:  station: Pointer to the record of the station used to keep track
 *                   of its properties
 */

void readInitfile(char* initFilename, int n, StationRecord* station,
                  int mode)
{
    char buttons[8][256];
    char statstr[20]="[station 1]";
    char stationname[256]="\0";
    char servername[256]="\0";
    char positionFilter[256]="No_Filter";
    int positionFilterParam=40;
    char orientationFilter[256]="No_Filter";
    int orientationFilterParam=40;
    float scaleX=1.0,scaleY=1.0,scaleZ=1.0;
    char format[256]="Position_Quaternion";


    struct Config_Tag configs[] =
    {     // Valid options
        { "format", String_Tag, format },
        { "button1", String_Tag, buttons[0] },
        { "button2", String_Tag, buttons[1] },
        { "button3", String_Tag, buttons[2] },
        { "button4", String_Tag, buttons[3] },
        { "button5", String_Tag, buttons[4] },
        { "button6", String_Tag, buttons[5] },
        { "button7", String_Tag, buttons[6] },
        { "button8", String_Tag, buttons[7] },
        { "stationname", String_Tag, stationname },
        { "servername", String_Tag, servername },
        { "position_filter", String_Tag, positionFilter },
        { "position_filter_param", Word_Tag, &positionFilterParam },
        { "orientation_filter", String_Tag, orientationFilter },
        { "orientation_filter_param", Word_Tag, &orientationFilterParam },
        { "scale_x", Float_Tag, &scaleX },
        { "scale_y", Float_Tag, &scaleY },
        { "scale_z", Float_Tag, &scaleZ },
        { NULL , Error_Tag, NULL        }   // Terminating record
    };

    readGeomSection(initFilename);
    initParallelButtons(initFilename);  
    printf("\n");

    //--- Initialize Buttons to be 'None'
    for (int i=0; i<n; i++)
    {
        station[i].buttonMode=buttonNone;
        for (int j=0; j<8; j++) { station[i].buttons[j]=0; }
    }

    for (int i=0; i<n; i++)
    {
        sprintf(statstr,"[station %d]",i+1);
        for (int j=0; j<8; j++) { strcpy(buttons[j],"None"); }

        strcpy(stationname,"\0");
        strcpy(positionFilter,"No_Filter");
        positionFilterParam=40;
        strcpy(orientationFilter,"No_Filter");
        orientationFilterParam=40;
        scaleX=1.0; scaleY=1.0; scaleZ=1.0;
        strcpy(format,"Position_Quaternion");

        (void) input_config(initFilename,configs,"[common]");
        (void) input_config(initFilename,configs,statstr);

    	if (mode==parButton) station[i].mode=positionQuaternion;
        strcpy(station[i].name,stationname);
        strcpy(station[i].servName,servername);    

        station[i].positionFilterParam=positionFilterParam;
        checkPosFilter(positionFilter,station,i);
        station[i].orientationFilterParam=orientationFilterParam;
        checkOrientFilter(orientationFilter,station,i);

        checkFormat(format,station,i);
        if ( mode == polhemus )
            if ( station[i].mode == positionMatrix)
            {
                fprintf(stderr,"Polhemus does not support "
                               "Position_Matrix.\n"); exit(1);
            }
                               

        station[i].scaleX=scaleX;
        station[i].scaleY=scaleY;
        station[i].scaleZ=scaleZ;

        for (int j=0; j<8; j++)                       // check button 1 to 8
        {
            checkButton(buttons[j],station,i,j,n);
        }
    }

    printStationFilter(station,n);       // Write info about filtering
}


/*** function getParButtons
 *
 * Purpose: Gets the buttonBytes from the parallel interfaces 
 * Globals: parRec
 */

void getParButtons()
{
    int i;

    for (i=0; i<8; i++)
    {
        if (parRec[i].fdParallel != 0)
        {
            parRec[i].parButtons=~inPort(parRec[i].fdParallel,
                                          parRec[i].basePort);
        }
    }
}


/*** function getParBit
 *
 * Purpose: Uses the buttonByte from the parallel interface and shifts the
 *          relevant bit to its position as specified in the initfile
 * In:      stNr:    Number of the station for which the button bit
 *                   will be composed
 *          station: Pointer to the record of the station holding
 *                   the assignments of the stations' raw bits to the
 *                   bitpositions of the buttonByte to be composed
 * Returns: Shifted button bit from the parallel interface for the specified
 *          station
 */

char getParBit(int stNr, StationRecord* station, int j)
{
    int bval,b2val;
    char c;

    c=0;
    bval=station[stNr].buttons[j];

    if ( (bval > 10000) || ( bval < -10000) ) // Button from Parallel device
    {
        if (bval < 0) { bval=-bval; }
        // Get button bit from specified parallel and specified bitposition
        b2val=(parRec[(bval/10000)-1].parButtons) & (1<<((bval%10)-1));
        b2val=b2val>>((bval%10)-1);         // Shift button bit to lsb
        if (station[stNr].buttons[j] < 0)    // Invert the button bit?
        { if (b2val == 0) { b2val=1; } else { b2val=0; } }
        b2val=b2val<<j;                     // Shift bit to Button bit j
        c=c|b2val;                            // Collect bits in this byte
    }
    return c;
}


/*** function composeButtons
 *
 * Purpose: Composes the buttonByte for a station which will be sent via the
 *          network according to the assignments from the initfile. The
 *          physical bits from the various stations' raw buttonByte
 *          will be shifted and put into the one buttonByte for the network
 *          as defined in the initfile.
 * In:      stNr:    Number of the station for which the button byte
 *                   will be composed
 * In/Out:  station: Pointer to the record of the station holding
 *                   the assignments of the stations' raw bits to the
 *                   bitpositions of the buttonByte to be composed
 * Returns: Composed button byte for the specified station
 */

char composeButtons(int stNr, StationRecord* station)
{
    int j;
    int bval,b2val;
    char c;

    c=0;

#ifndef BUTTON_HACK
    for (j=0; j<8; j++)                         // Button bit 1 to 8
    {
        c=c|getParBit(stNr,station,j);      // Buttonbit from Parallel dev

        bval=station[stNr].buttons[j];
        if ( (bval < 10000) && (bval > -10000) && (bval != 0))
        {                                       // Button from Station
            if (bval < 0) { bval=-bval; }
            // Get button bit from specified station and specified bitposition
            b2val=(station[(bval/100)-1].rawButton) & (1<<((bval%10)-1));
            b2val=b2val>>((bval%10)-1);         // Shift button bit to lsb
            if (station[stNr].buttons[j] < 0)    // Invert the button bit?
            { if (b2val == 0) { b2val=1; } else { b2val=0; } }
            b2val=b2val<<j;                     // Shift bit to Button bit j
            c=c|b2val;                            // Collect bits in this byte
        }
    }
#endif

#ifdef BUTTON_HACK
    for (j=0; j<8; j++)
    {
        c=c|getParBit(stNr,station,j);      // Buttonbit from Parallel dev

        bval=station[stNr].buttons[j];            // Get button data from
        if ( (bval < 10000) && (bval > -10000) && (bval != 0))
        {
            if (bval < 0) bval=-bval;
            b2val=station[(bval/100)-1].rawButton;

            if ((bval%10) == 6)
            {
                switch (b2val)
                {
                    case 0x30: c=c|(1<<j); break;   // Only one button pressed                                         
                    case 0x70: c=c|(1<<j); break;   // Both buttons pressed
                }                                
            }

            if ((bval%10) == 5)
            {
                switch (b2val)
                {
                    case 0x10: c=c|(1<<j); break;   // Only one button pressed                        
                    case 0x70: c=c|(1<<j); break;   // Both buttons pressed
                }
            }
        }
    }
#endif
    return c;
}


/*** function shiftPos
 *
 * Purpose: Shifts the position array (which has the size of
 *          position_filter_param from the initfile) used for the software
 *          position filter to make room for the latest position data.
 * In:      stNr: Number of the station which position array will be shifted
 * In/Out:  station: Pointer to the record of the station holding
 *                   the position array
 */

void shiftPos(int stNr, StationRecord* station)
{
    int max=station[stNr].positionFilterParam;

    for (int i=1; i<max; i++)
    {
        station[stNr].fposition[0][max-i]=station[stNr].fposition[0][max-i-1];
        station[stNr].fposition[1][max-i]=station[stNr].fposition[1][max-i-1];
        station[stNr].fposition[2][max-i]=station[stNr].fposition[2][max-i-1];
    }

    station[stNr].fposition[0][0]=station[stNr].position[0];
    station[stNr].fposition[1][0]=station[stNr].position[1];
    station[stNr].fposition[2][0]=station[stNr].position[2];
}


/*** function shiftOrient
 *
 * Purpose: Shifts the orientation array (which has the size of
 *          orientation_filter_param from the initfile) used for the software
 *          orientation filter to make room for the latest orientation data.
 * In:      stNr: Number of the station which orientation array will be
 *                shifted
 * In/Out:  station: Pointer to the record of the station holding
 *                   the orientation array
 */

void shiftOrient(int stNr, StationRecord* station)
{
    int max=station[stNr].orientationFilterParam;

    int j=4;
    if (station[stNr].mode == positionAngles) j=3;
    if (station[stNr].mode == positionMatrix) j=9;

    for (int i=1; i<max; i++)
    {
        for (int k=0; k<j; k++)
        {
            station[stNr].forientation[k][max-i]=
            station[stNr].forientation[k][max-i-1];
        }
    }

    for (int k=0; k<j; k++)
    {
        station[stNr].forientation[k][0]=station[stNr].orientation[k];
    }
}


/*** function qdq
 *
 * Purpose: Returns the dot product of quaternions q1 and q2
 * In:      q1: Pointer at quaternion
 *          q2: Pointer at quaternion
 * Returns: dot product of q1 and q2
 */

float qdq(float* q1, float* q2)
{
    return((q1[0]*q2[0])+(q1[1]*q2[1])+(q1[2]*q2[2])+(q1[3]*q2[3]));
}


/*** function qinv
 *
 * Purpose: Sets qi to the inverse of quaternion q
 * In:      q: Pointer at quaternion
 * In/Out:  qi: Pointer at quaternion
 */

void qinv(float* q, float* qi)    
{
    float qn;

    qn = qdq(q, q);
    qi[0] =  q[0]/qn;
    qi[1] = -q[1]/qn;
    qi[2] = -q[2]/qn;
    qi[3] = -q[3]/qn;
}


/*** function qxq
 *
 * Purpose: q3 is the product of quaternions q1 and q2
 * In:      q1: Pointer at quaternion
 *          q2: Pointer at quaternion
 * In/Out:  q3: Pointer at quaternion
 */

void qxq(float* q1, float* q2, float* q3)
{
    q3[0] = q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2]-q1[3]*q2[3];
    q3[1] = q1[0]*q2[1]+q1[1]*q2[0]+q1[2]*q2[3]-q1[3]*q2[2];
    q3[2] = q1[0]*q2[2]+q1[2]*q2[0]+q1[3]*q2[1]-q1[1]*q2[3];
    q3[3] = q1[0]*q2[3]+q1[3]*q2[0]+q1[1]*q2[2]-q1[2]*q2[1];
}


/*** function vrotq
 *
 * Purpose: Rotate vector v1 by quaternion q and store in vector v2
 * In:      q:  Pointer at quaternion
 *          v1: Pointer at vector
 * In/Out:  v2: Pointer at vector
 */

void vrotq(float* q, float* v1, float* v2)
{
    float q1[4], q2[4], qi[4], qt[4];

    q1[0] = 0.0;                        // w
    q1[1] = v1[0];                      // x
    q1[2] = v1[1];                      // y
    q1[3] = v1[2];                      // z

    qinv(q, qi);
    qxq(q, q1, qt);
    qxq(qt, qi, q2);

    v2[0] = q2[1];
    v2[1] = q2[2];
    v2[2] = q2[3];
}


/*** function anisoFilterPos
 *
 * Purpose: Applies an anisotropic filter on the position data. It is
 *          supposed to be used with those stations that have HMDs connected.
 * In:      stNr: Number of the station whose position data will be filtered.
 * In/Out:  station: Pointer to the record of the station holding the
 *                   position array
 */

void anisoFilterPos(int stNr, StationRecord* station)
{
    shiftPos(stNr,station);

    float v1[3],v2[3],v3[3];
    float qt[4];
    int length=station[stNr].positionFilterParam;

    qt[0] = -station[stNr].orientation[0];
    qt[1] = station[stNr].orientation[1];
    qt[2] = station[stNr].orientation[2];
    qt[3] = station[stNr].orientation[3];

    v3[0] = v3[1] = v3[2] = 0.0;

    int length_3 = length / 3;
    if (!length_3) length_3 = 1;

    for (int i=0; i<length; i++)
    {
        v1[0] = station[stNr].position[0];
        v1[1] = station[stNr].position[1];
        v1[2] = station[stNr].position[2];
        vrotq (qt, v1, v2);
        if ( i < length_3 ) v3[0] += v2[0];
        v3[1] += v2[1];
        v3[2] += v2[2];
    }
    v3[0] /= (float)length_3;
    v3[1] /= (float)length;
    v3[2] /= (float)length;
    qt[0] = -qt[0];
    vrotq(qt, v3, v2);
    station[stNr].position[0] = v2[0];
    station[stNr].position[1] = v2[1];
    station[stNr].position[2] = v2[2];
}


/*** function boxFilterPos
 *
 * Purpose: Applies a box filter on the position data.
 * In:      stNr: Number of the station whose position data will be filtered.
 * In/Out:  station: Pointer to the record of the station holding the
 *                   position array
 */

void boxFilterPos(int stNr, StationRecord* station)
{
    shiftPos(stNr,station);

    float pos[3];
    int max=station[stNr].positionFilterParam;

    pos[0]=0.0;
    pos[1]=0.0;
    pos[2]=0.0;

    for (int i=0; i<max; i++)
    {
        pos[0] += station[stNr].fposition[0][i];
        pos[1] += station[stNr].fposition[1][i];
        pos[2] += station[stNr].fposition[2][i];
    }

    station[stNr].position[0] = pos[0] / (float)max;
    station[stNr].position[1] = pos[1] / (float)max;
    station[stNr].position[2] = pos[2] / (float)max;
}


/*** function boxFilterOrient
 *
 * Purpose: Applies a box filter on the orientation data (angles and
 *          quaternion only)
 * In:      stNr: Number of the station whose orientation data will be
 *                filtered.
 * In/Out:  station: Pointer to the record of the station holding the
 *                   orientation array
 */

void boxFilterOrient(int stNr, StationRecord* station)
{
    shiftOrient(stNr,station);

    float orient[4];
    int max=station[stNr].orientationFilterParam;

    int j=4;
    if (station[stNr].mode == positionAngles) j=3;
    if (station[stNr].mode == positionMatrix) return;

    for (int i=0; i<j; i++) orient[i]=0.0;

    for (int k=0; k<max; k++)
    {
        for (int i=0; i<j; i++) orient[i] += station[stNr].forientation[i][k];
    }

    for (int i=0; i<j; i++) orient[i] /= (float) max;

    if (station[stNr].mode == positionQuaternion)
    {
        float norm = sqrt((orient[0]*orient[0])+(orient[1]*orient[1])+
                          (orient[2]*orient[2])+(orient[3]*orient[3]));

        if (fabs(norm-1.0) > 0.0001)
        {
            for (int i=0; i<j; i++) orient[i] /= norm;
        }
    }

    for (int i=0; i<j; i++) station[stNr].orientation[i] = orient[i];
}


/*** function kalmanFilterOrient
 *
 * Purpose: Applies a Kalman filter on the orientation data (quaternion only)
 * In:      stNr: Number of the station whose orientation data will be
 *                filtered.
 * In/Out:  station: Pointer to the record of the station holding the
 *                   orientation array
 */

void kalmanFilterOrient(int stNr, StationRecord* station)
{
    float orient[4];

    if (station[stNr].mode == positionQuaternion)
    {
        station[stNr].kfilter.New_K_filt(station[stNr].orientation, orient);

        for (int i=0; i<4; i++) station[stNr].orientation[i]=orient[i];
    }   
}


/*** function filterRecord
 *
 * Purpose: Calls the filtering functions for the position and orientation
 *          data of a station accordingly
 * In:      stNr: Number of the station to be filtered
 * In/Out:  station: Pointer to the record of the station holding the
 *                   position and orientation array
 */

void filterRecord(int stNr, StationRecord* station)
{
    if (station[stNr].positionFilter == boxFilter)
        boxFilterPos(stNr,station);

    if (station[stNr].positionFilter == anisoFilter)
        anisoFilterPos(stNr,station);

    if (station[stNr].orientationFilter == boxFilter)
        boxFilterOrient(stNr,station);

    if (station[stNr].orientationFilter == kalmanFilter)
        kalmanFilterOrient(stNr,station);
}


/*** function processRecord
 *
 * Purpose: Builds buttondata from the record read from the station, prints
 *          the information and converts it to the byte order suitable for
 *          network transmission
 * In:      stNr:    Number of station to be dealt with
 * In/Out:  nR:      Pointer to the data part of the record which is
 *                   supposed to be sent via the network
 *          station: Pointer to the record of the stations used to keep
 *                   track of its properties
 */

void processRecord(char* nR, StationRecord* station, int stNr)
{
    getParButtons();

    // Compose the buttonByte sent via network from the raw buttons' data
    char buttonByte=composeButtons(stNr,station);

    scalePosition(station[stNr].position,station,stNr);

    if (station[stNr].mode == positionQuaternion)
    {
        revertQuaternion(station[stNr].orientation);
    }

    filterRecord(stNr,station);

    printPosButton(stNr,station[stNr].position,buttonByte);

    initNetRecordData(nR,&station[stNr],buttonByte,stNr+1);

    station[stNr].pos=0;              // Reset bytecounter of the station
    station[stNr].received++;         // Another record received and processed
}


/*** function processFobRecord
 *
 * Purpose: Builds the position, quaternion and buttondata from the record
 *          read from the station, prints the information and converts it
 *          to the byte order suitable for network transmission
 * In:      stNr:    Number of station to be dealt with
 *          mode:    Mode of operation (fobMulti or fobSingle)
 * In/Out:  nR:      Pointer to the data part of the record which is
 *                   supposed to be sent via the network
 *          station: Pointer to the record of the stations used to keep
 *                   track of its properties
 */

void processFobRecord(char* nR, StationRecord* station, int stNr, int mode)
{
    char *inputBuffer;

    // Buffer holding the complete record of the station
    inputBuffer = station[stNr].buffer; 

    // Build the values from the raw bytes of the record of the station
    buildPosition(inputBuffer,station[stNr].position);
    switch (station[stNr].mode)
    {    
        case positionQuaternion:
            buildQuaternion(inputBuffer,station[stNr].orientation); break;
        case positionMatrix:        
            buildMatrix(inputBuffer,station[stNr].orientation); break;
        case positionAngles:
            buildAngles(inputBuffer,station[stNr].orientation); break;
    }

    // Get the raw buttonByte
    if (station[stNr].buttonMode == buttonTracker)
    {
        if ( mode == fobSingle)
            station[stNr].rawButton=inputBuffer[(station[stNr].numOfBytes)-2];
        else
            station[stNr].rawButton=inputBuffer[(station[stNr].numOfBytes)-1];
    }

    processRecord(nR,station,stNr);
}


/*** main
 *
 */

int main(int argc, char **argv)
{
    char initFilename[256];
    NetPropRecord npr;

    int mode;
    int numOfStations;
    NetworkTrackerDataRecord netRecord;

    int ticks = 0;
    double lastTime;

    int i,b,stNr;
    short int si;

    printf("\n");
    //--- comand line parameters
    OptionString optToaddr("address","a", "224.100.200.101");
    OptionString optToport("port","p", "12346");
    OptionString optInitFilename("initfilename","i","tracker.init");
    OptionString optMode("mode","m","fobMulti");
    OptionString optSerialBaudRate("serialbaudrate","s","57600");
    OptionLong optERC("erc","e",0);
    if(!Option::parse(argc,argv)) { Option::printUsage(); exit(-1); }

    strcpy(npr.toaddr,optToaddr.getValue());
    strcpy(npr.toport,optToport.getValue());
    strcpy(initFilename,optInitFilename.getValue());
    strcpy(serialBaudRate,optSerialBaudRate.getValue());
    ercId = optERC.getValue();
 
    if (strcasecmp(optMode.getValue(),"fobMulti") == 0) mode=fobMulti;
    else
        if (strcasecmp(optMode.getValue(),"fobSingle") == 0) mode=fobSingle;
        else
            if (strcasecmp(optMode.getValue(),"polhemus") == 0) mode=polhemus;
            else
                if (strcasecmp(optMode.getValue(),"parButton") == 0)
                    mode=parButton;
                else
                {
                    fprintf(stderr,"Unsupported mode. Valid modes: "
                            "fobMulti, fobSingle, polhemus or parButton\n");
                    exit(1);
                }

    npr.sock=initMulticastGroup(npr.toaddr, npr.toport);

    testFile(initFilename);                   // Tries to open the config file

    numOfStations=getNumOfTrackers(initFilename,mode);
    StationRecord *station = new StationRecord[numOfStations];
    readInitfile(initFilename, numOfStations, station, mode);
    initStationRecord(station,numOfStations);

    initNetRecord(&netRecord, numOfStations);
    initNetRecordHeader(&netRecord,station[0].servName);
    netRecord.numOfStations=0;
    char* nR=netRecord.data+ntohs(netRecord.commentLength);

    if ( ( mode == fobSingle) || ( mode == fobMulti) )
        initTrackersFob(initFilename,mode,station,numOfStations);

    if (mode == polhemus)
        (void) runFastrak(initFilename, numOfStations, netRecord, station,
                          npr);

    lastTime=osTimeOfDay()/1000.0;

    stNr = 0;                                      // Birdnumber for fobSingle
    startFlock(numOfStations,mode,station);      // Get Flock of Birds running

    initCurses();
    initScreen(npr.toaddr,npr.toport,station,numOfStations);

    while (1)
    {
        if (mode==fobSingle)
        {
            if (station[stNr].erc == fobErc)
            {
                stNr = (stNr + 1) % numOfStations;      // Skip Bird with ERC
            }
            
            i=readFobRecordSingle(station,numOfStations,stNr);
        
            if (i>=0)
            {
                stNr = (stNr + 1) % numOfStations;

                if (station[i].sent == 1)
                {
                    netRecord.numOfStations=htons(netRecord.numOfStations);
                    if ((b=sendData(npr.sock,(char *)&netRecord,
                                    getNetRecSize(&netRecord)))<0)
                    {
                        // ignor conn. refused, output other errors
                        if (errno!=ECONNREFUSED) perror("send");
                        beep();
                    }

                    for (int j=0; j<numOfStations; j++) station[j].sent=0;
                    nR=netRecord.data+ntohs(netRecord.commentLength);
                    netRecord.numOfStations=0;
                }

                processFobRecord(nR,station,i,mode);
                station[i].sent=1;

                netRecord.numOfStations++;
                memcpy(&si,nR+sizeof(short int)*3,sizeof(short int));
                nR=nR+ntohs(si);

                printRate(&ticks,&lastTime,station,numOfStations);
            }
        }

        if (mode==fobMulti)
        {        
            i=readFobRecordMulti(station,numOfStations);

            if (i==-1) recoverTimeout(numOfStations,station);
            if (i>=0)
            {
                if (station[i].sent == 1)
                {
                    netRecord.numOfStations=htons(netRecord.numOfStations);
                    if ((b=sendData(npr.sock,(char *)&netRecord,
                                    getNetRecSize(&netRecord)))<0)
                    {
                        // ignor conn. refused, output other errors
                        if (errno!=ECONNREFUSED) perror("send");
                        beep();
                    }

                    for (int j=0; j<numOfStations; j++) station[j].sent=0;
                    nR=netRecord.data+ntohs(netRecord.commentLength);
                    netRecord.numOfStations=0;
                }

                processFobRecord(nR,station,i,mode);
                station[i].sent=1;

                netRecord.numOfStations++;
                memcpy(&si,nR+sizeof(short int)*3,sizeof(short int));
                nR=nR+ntohs(si);

                printRate(&ticks,&lastTime,station,numOfStations);
            }
        }

        if (mode==parButton)
        {
            for (i=0; i<numOfStations; i++)
            {
                getParButtons();
                char buttonByte=composeButtons(i,station);

                printPosButton(i,station[i].position,buttonByte);

                initNetRecordData(nR,&station[i],buttonByte,i+1);

                netRecord.numOfStations++;
                memcpy(&si,nR+sizeof(short int)*3,sizeof(short int));
                nR=nR+ntohs(si);
            }

            netRecord.numOfStations=htons(netRecord.numOfStations);
            if ((b=sendData(npr.sock,(char *)&netRecord,
                            getNetRecSize(&netRecord)))<0)
            {
                /* ignor conn. refused, output other errors */
                if (errno!=ECONNREFUSED) perror("send");
                beep();
            }
            nR=netRecord.data+ntohs(netRecord.commentLength);
            netRecord.numOfStations=0;
        }

        chkInput(station,mode);
    }
    delete [] station;
}
