// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Polhemus
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of Polhemus Fastrak specific functions used in
//              the trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:18:12  rr      last modification
// ===========================================================================

#include "Polhemus.h"

const int maxJunk=1000;   // maximum number of junk chars. tolerated til reset

const int tsStart=0;                                       // states of parser
const int tsStationNumber=1;
const int tsSpace=2;
const int tsSyncBit=3;
const int tsGetRec=4;


void initCurses();
void printBinary(int b);
void getSerialDevNames(char* initFilename, StationRecord* station,
                       int fdCount);
void processRecord(char* nR, StationRecord* station, int stNr);
void initScreen(char* toAddr, char* toPort, StationRecord* station, int stNr);
void printRate(int *ticks, double *lastTime, StationRecord *station, int n);

char buttons[4];              // button states
int restart;
int stNr;


/*** function writelog
 *
 * Purpose: Writes a message and does a carriage return using curses and
 *          updates the output.
 * In:      message: String to be output
 */

void writelog(char *message)
{
    printw(message);
    printw("\n");
    refresh();
}


/*** function chkInputFastrak
 *
 * Purpose: Checks for user input and either quits the trackerserver 
 *          or restarts it. 'q' quits with exit code 0. 'r' restarts it.
 * Globals: restart
 */

void chkInputFastrak(void)
{
    char inputch;

    if ((inputch=getch())!=ERR)                        // check for user input
    { 
        switch (tolower(inputch))
        {
            case 'r': restart=TRUE; break;                  // restart tracker
            case 'q': endwin(); exit(0);                       // exit program
        }
    }
}


/*** function getSerialDevNames
 *
 * Purpose: Reads the names of the serial devices for the tracker and the mouse
 *  	    from the initfile
 * In:	    initFilename: Pointer to the string holding the name of the
 *  	    	    	  initfile
 *          n:            Number of stations
 * In/Out:  station:   Pointer to the record of the stations used to keep
 *                     track of its properties
 *          sio_mouse: Pointer to the string holding the name of the serial
 *                     mouse interface
 */

void getSerialNames(char* initFilename, StationRecord* station, int n,
                    char* sio_mouse)
{
    struct Config_Tag configs[] = {
    	{ "serial_mouse", String_Tag, sio_mouse },
        { NULL , Error_Tag, NULL        }   /* Terminating record   */
    };

    getSerialDevNames(initFilename,station,n);

    (void) input_config(initFilename,configs,"[common]");    
}


/*** function writeInitStr
 *
 * Purpose: Sends multi 'init' options of the given section of the
 *          initfile to the Fastrak
 * In:	    fdTracker:    Filedescriptor of the Fastrak
 *  	    initFilename: Pointer to a string holding the name of the initfile
 *          section:      The [section] from the initfile to be focused on
 */

void writeInitStr(int fdTracker, char *initFilename, char *section)
{
    FILE *initFile;
    char buffer[256]; 
    char buffer2[256]; 

    if ((initFile = fopen(initFilename, "r"))==NULL)
    {
    	fprintf(stderr,"Error opening '%s'.\n",initFilename);
	exit(1);
    }

    while (fgets(buffer, 256, initFile) != NULL)
    {
    	if (strncasecmp(buffer,section,strlen(section))==0) break;
    }

    while (fgets(buffer, 256, initFile) != NULL)
    {
                                                          // change CR to NL
    	for (int i=0; i<strlen(buffer); i++) if (buffer[i]==10) buffer[i]=13;

        int j=0;
        if ( buffer[0] == '[' )
            for (int i=0; i<strlen(buffer); i++)
                if ( buffer[i] == ']' ) j=1;
        if ( j != 0 ) break;

    	if (strncasecmp(buffer,"init=",5)==0)
     	{
    	    strcpy(buffer2,buffer+5);
    	    writeStrToSerial(fdTracker, buffer2);
    	}

    }  
    fclose(initFile);
}


/*** function readCfgFile
 *
 * Purpose: Reads options from the initfile, translates them to Fastrak
 *          commands and writes them using the serial interface.
 * In:	    fd:           Filedescriptor of the Fastrak
 *  	    initFilename: Pointer to a string holding the name of the initfile
 *          stNr:         Number of trackers of the Fastrak
 */

void readCfgFile(int fd, char *initFilename, int stNr)
{
    char hemisphere[256]="1,0,0";
    char reference_frame[256]="0,0,0,200,0,0,0,200,0";
    char statstr[20]="[station 1]";
    char buffer[256];

    struct Config_Tag configs[] =
    {   // Valid options
        { "hemisphere", String_Tag, hemisphere },
        { "reference_frame", String_Tag, reference_frame },
        { NULL , Error_Tag, NULL        }                // Terminating record
    };

    writeStrToSerial(fd,"\rc\r");                        // Continous mode off

    for (int i=0; i<stNr; i++)
    {
        sprintf(statstr,"[station %d]",i+1);
        strcpy(hemisphere,"1,0,0");
        strcpy(reference_frame,"0,0,0,200,0,0,0,200,0");

        (void) input_config(initFilename,configs,"[common]");
        (void) input_config(initFilename,configs,statstr);

        sprintf(buffer,"H%d,%s\r",i+1,hemisphere);
        writeStrToSerial(fd,buffer);

        sprintf(buffer,"R%d\rA%d,%s\r",i+1,i+1,reference_frame);
        writeStrToSerial(fd,buffer);

    }    
}


/*** function initTracker
 *
 * Purpose: Sends the initfile to the Fastrak, sets it to 16bit xyz and
 *  	    quaternion output and starts the continous mode
 * In:	    fdTracker:    Filedescriptor of the Fastrak
 *  	    initFilename: Pointer to a string holding the name of the
 *  	    	    	  file which is sent to the Fastrak
 *          stNr:         Number of trackers of the Fastrak
 */

void initTracker(StationRecord* station, char *initFilename, int stNr)
{
    int i;
    fd_set result;
    char statstr[20]="[common]";
    char str[256];
    int fdTracker=station[0].fd;

    readCfgFile(fdTracker, initFilename, stNr);    

    writelog("Sending init file");
    writeInitStr(fdTracker, initFilename, statstr);
    for (i=0; i<stNr; i++)
    {
        sprintf(statstr,"[station %d]",i+1);
        writeInitStr(fdTracker, initFilename, statstr);
    }


    for (i=0; i<stNr; i++)
    {
        if (station[i].mode == positionQuaternion)
        {
            sprintf(str,"O%d,18,20\r",i+1);
            writeStrToSerial(fdTracker,str);
            station[i].numOfBytes=14;
        }
        if (station[i].mode == positionAngles)
        {
            sprintf(str,"O%d,18,19\r",i+1);
            writeStrToSerial(fdTracker,str);
            station[i].numOfBytes=12;
        }
    }

    writeStrToSerial(fdTracker,"u\rC\r");

    iFlush(fdTracker);                        // draining input buffer of junk
}


/*** function pingTracker
 *
 * Purpose: Pings the Fastrak by requesting an output record until it receives
 *  	    a byte or the tracker gets quitted by pressing 'q'.
 * In:	    fdTracker:	Filedescriptor of the serial interface
 */

void pingTracker(int fdTracker)
{
    static char *PingString = "\rP";
    char c;

    do
    {
    	write(fdTracker, PingString, strlen(PingString));
	tcdrain(fdTracker);
	fprintf(stderr, "."); 
    	chkInputFastrak();
    	restart=FALSE;	    	       // Ignore 'r' being pressed for restart
    }
    while (nonBlockGetChar(fdTracker, 0, 1, &c, buttons)<0); 
}
  

/*** function getBinary
 *
 * Purpose: Returns the two bytes from the buffer as a Fastrak 16 bit integer
 * In:	    buffer: 	Pointer to array of bytes to be converted
 * Returns: Fastrak 16 bit integer
 */

short int getBinary(const char *buffer)
{
    int d;

    d=(buffer[0]&0x7f)+(buffer[1]<<7); 
    if (d&0x2000) { d=(d&0x1ffff)-16384; }

    return d;
}


/*** function buildPosition
 *
 * Purpose: Convert position bytes from Fastrak into actual position int
 *  	    values
 * In:	    buffer: 	Buffer containing bytes received from Fastrak
 * In/Out:  position:	Pointer to an array of floats for the position to
 *  	    	    	be put in
 */

void buildPositionFT(char* buffer, float* position)
{
    for (int i=0; i<3; i++)
    {
    	position[i]=(float)(getBinary(buffer+i*2));
    }
}


/*** function buildQuaternionFT
 *
 * Purpose: Convert quaternion bytes from Fastrak into actual quaternion
 *  	    float values
 * In:	    buffer: 	    Buffer containing bytes received from Fastrak
 * In/Out:  quaternion:     Pointer to an array of floats for the quaternion
 *  	    	    	    to be put in
 */

void buildQuaternionFT(char* buffer, float* quaternion)
{
    for (int i=0; i<4; i++)
    {
    	quaternion[i]=((float)(getBinary(buffer+i*2+6)))/8192;
    }
}


/*** function buildAnglesFT
 *
 * Purpose: Convert angle bytes from Fastrak into actual angle float values
 * In:	    buffer: 	    Buffer containing bytes received from Fastrak
 * In/Out:  quaternion:     Pointer to an array of floats for the quaternion
 *  	    	    	    to be put in
 */

void buildAnglesFT(char* buffer, float* angles)
{
    for (int i=0; i<3; i++)
    {
    	angles[i]=((float)(getBinary(buffer+i*2+6)))/32768*180;
    }
}


/*** function processFastrakRecord
 *
 * Purpose: Builds the position, quaternion and buttondata from the record
 *          read from the station, prints the information and converts it
 *          to the byte order suitable for network transmission
 * In:      stNr:    Number of station to be dealt with
 * In/Out:  nR:      Pointer to the data part of the record which is
 *                   supposed to be sent via the network
 *          station: Pointer to the record of the stations used to keep
 *                   track of its properties
 */

void processFastrakRecord(char* nR, StationRecord* station, int stNr)
{
    buildPositionFT(station[0].buffer,station[stNr].position);

    switch (station[stNr].mode)
    {    
        case positionQuaternion:
            buildQuaternionFT(station[0].buffer,station[stNr].orientation);
            break;
        case positionAngles:
            buildAnglesFT(station[0].buffer,station[stNr].orientation); break;
    }

    // Get the raw buttonByte
    if (station[stNr].buttonMode == buttonTracker)
    {
        station[stNr].rawButton=buttons[stNr];
    }

    processRecord(nR,station,stNr);
}


/*** function parseRecord
 *
 * Purpose: Parses the data being read from the Fastrak using the Recordtype
 *          and the sync bit.
 * In:      n: Number of stations being used
 *          c: Byte that was read from the Fastrak
 * In/Out:  state:   Pointer to the state of the parser
 *          station: Pointer to the record of the station used to keep track
 *                   of its properties
 * Returns:  0 if a whole data record has been read
 *          -1 else
 */

int parseRecord(StationRecord* station, int *state, char c, int n)
{
    int rc=-1;

    switch (*state)
    {
        case tsStart:
            if (c=='0')
            {              // Record Type "0", possible start of output record
                (*state)=tsStationNumber;
            }
            break;

        case tsStationNumber:
            stNr=c-'1';
            if ((stNr>=0) && (stNr<n))
            {                                                // station number
                (*state)=tsSpace;
            } else (*state)=tsStart;
            break;

        case tsSpace:
            if ( c==' ') (*state)=tsSyncBit; else (*state)=tsStart;
            break;

        case tsSyncBit:
            station[0].pos=0;
            if ((c&0x80)!=0)        // expecting to find a byte with hibit set
            {
                (*state)=tsGetRec;
                station[0].buffer[station[0].pos++]=(c&0x7f);
            } else { (*state)=tsStart; }
            break;

        case tsGetRec:
            if ((c&0x80)!=0) { (*state)=tsStart; } else
            {
                station[0].buffer[station[0].pos++]=c;
                if (station[0].pos==station[stNr].numOfBytes)
                {                       // got one whole data record in buffer
                    (*state)=tsStart;
                    rc=0;
                }
            }
    }

    return(rc);
}


/*** Fastrak mainloop
 *
 */

int runFastrak(char* initFilename, int numOfStations,
               NetworkTrackerDataRecord netRecord, StationRecord *station,
               NetPropRecord npr)
{
    char c;
    int fdMouse, b;
    int state, junkRead;
    char sio_mouse[256]="/dev/cua1";    	// device for mouse buttons

    short int si;

    int ticks = 0;
    double lastTime;

    char* nR=netRecord.data+ntohs(netRecord.commentLength);

    getSerialNames(initFilename,station,numOfStations,sio_mouse);
    station[0].fd=openSerialToFastrak(station[0].serDevName,
                                      "Tracker","115200");
    fdMouse=openSerialToFastrak(sio_mouse,"3d-Mouse","9600");

    initCurses();

    while(1)                           // loop if tracker dies -> reinitialise
    {
        initScreen(npr.toaddr,npr.toport,station,numOfStations);
      	move(11+stNr,0); writelog("Waiting for tracker to respond...");

        pingTracker(station[0].fd);
    	initTracker(station,initFilename,numOfStations);

        writelog("Starting to poll data");

        state=tsStart;                            // state variable of parser
        junkRead=0;                                // no junk read so far

        lastTime=osTimeOfDay()/1000.0;
        ticks=0;

        initScreen(npr.toaddr,npr.toport,station,numOfStations);
      	move(11+stNr,0); printw("Starting to poll data\n"); refresh();

        while (1)               // leave loop only if timeout or junk detected
        {
            if (nonBlockGetChar(station[0].fd, fdMouse, 1, &c, buttons)<0)
            {
                writelog("\nTracker stopped responding\n");
                break;                   // leave loop and try to reinitialise
            }

            restart=FALSE;
            chkInputFastrak();                        // check for user input
            if (restart)
            {
                restart=FALSE;
                break;
            }

            junkRead++;
            if (junkRead>maxJunk)
            {
                writelog("\nTracker keeps sending junk\n");
                break;                               // too much junk, restart
            }

            b=parseRecord(station,&state,c, numOfStations);

            if (b == 0)
            {
                junkRead=0;

                if (station[stNr].sent == 1)
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

                processFastrakRecord(nR,station,stNr);
                station[stNr].sent=1;

                netRecord.numOfStations++;
                memcpy(&si,nR+sizeof(short int)*3,sizeof(short int));
                nR=nR+ntohs(si);

                printRate(&ticks,&lastTime,station,numOfStations);
            }
        }
    }
    return 0;
}
