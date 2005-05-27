// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Fob
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of Ascension FOB specific functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:18:12  rr      last modification
// ===========================================================================


#include "Fob.h"

extern int ercId;    // additional erc bird number - added gr2000-12-05

/*** function errorCodeToText
 *
 * Purpose: Translates the given errorcode into the corresponding string and
 *          returns its errorlevel according to the Ascension manual.
 * In:      errorcode: Ascension FOB Errornumber which you want to translate
 *                     into the errormessage
 * In/Out:  str: Pointer to a string where the errormessage will be put
 * Returns: Errorlevel according to Ascension manual.
 *          -1: Fatal
 *           0: No Error
 *           1: Warning1
 *           2: Warning2 (the flock continues to run)
 */

int errorCodeToText(int errorcode, char *str)
{
    switch(errorcode)
    {
        case 0: strcpy(str,"0: No Error\n"); return 0;
        case 1: strcpy(str,"1: Ram Failure\n"); return -1;
        case 2: strcpy(str,"2: Non-Volatile Storage Write Failure\n");
                return -1;
        case 3: strcpy(str,"3: PCB Configuration Data Corrupt\n"); return 1;
        case 4: strcpy(str,"4: Transmitter Configuration Data Corrupt\n");
                return 1;
        case 5: strcpy(str,"5: Receiver Configuration Data Corrupt\n");
                return 1;
        case 6: strcpy(str,"6: Invalid RS232 Command\n"); return 2;
        case 7: strcpy(str,"7: Not an FBB master\n"); return 2;
        case 8: strcpy(str,"8: No Birds accessible in Device List\n");
                return 2;
        case 9: strcpy(str,"9: Bird is Not Initialized\n"); return 2;
        case 10: strcpy(str,"10: FBB Receive Error - Intra Bird Bus\n");
                 return 1;
        case 11: strcpy(str,"11: RS232 Receive Overrun or Framing Error\n");
                 return 1;
        case 12: strcpy(str,"12: FBB Receive Error - FBB Host Bus\n");
                 return 1;
        case 13: strcpy(str,"13: No FBB Command Response"); return 1;
        case 14: strcpy(str,"14: Invalid FBB Host Command\n"); return 1;
        case 15: strcpy(str,"15: FBB Run Time Error\n"); return -1;
        case 16: strcpy(str,"16: Invalid CPU Speed\n"); return -1;
        case 17: strcpy(str,"17: No Data Error\n"); return 1;
        case 18: strcpy(str,"18: Illegal Baud Rate Error\n"); return 1;
        case 19: strcpy(str,"19: Slave Acknowledge Error\n"); return 1;
        case 20: strcpy(str,"20: Unused_INT4\n"); return -1;
        case 21: strcpy(str,"21: Unused_INT5\n"); return -1;
        case 22: strcpy(str,"22: Unused_INT6\n"); return -1;
        case 23: strcpy(str,"23: Unused_INT7\n"); return -1;
        case 24: strcpy(str,"24: Unused_INT9\n"); return -1;
        case 25: strcpy(str,"25: Unused_INT10\n"); return -1;
        case 26: strcpy(str,"26: Unused_INT11\n"); return -1;
        case 27: strcpy(str,"27: Unused_INT16\n"); return -1;
        case 28: strcpy(str,"28: CRT Synchronization Error\n"); return 1;
        case 29: strcpy(str,"29: Transmitter Not accessible Error\n");
                 return 1;
        case 30: strcpy(str,"30: Extended Range Transmitter Not "
                            "Attached Error\n"); return 1;
        case 31: strcpy(str,"31: CPU Time Overflow Error\n"); return 2;
        case 32: strcpy(str,"32: Receiver Saturated Error\n"); return 1;
        case 33: strcpy(str,"33: Slave Configuration Error\n"); return 1;
        case 34: strcpy(str,"34: Watch Dog Error\n"); return 1;
        case 35: strcpy(str,"35: Over Temperature Error\n"); return 1;
        default: //strcpy(str,"Unkown Errorcode!\n"); return -1;
                 sprintf(str,"Unknown Errorcode %i\n",errorcode);return -1;
    }
}


/*** function getSerialDevNames
 *
 * Purpose: Reads the name(s) of the serial device(s) from the initfile
 * In:      initFilename: Pointer to a string containing the name of the
 *                        initfile
 *          fdCount:      Number of stations used
 * In/Out:  station: Pointer to the record of the stations holding the
 *                   filedescriptor of the serial interconnection, the
 *                   buffer where the record being read from the bird
 *                   is stored, as well as the length of that record.
 */

void getSerialDevNames(char* initFilename, StationRecord* station,
                       int fdCount)
{
    char serial_devname[256];
    char statstr[20]="[station 1]";
    int i;

    struct Config_Tag configs[] =
    {
        { "serial_devname", String_Tag, serial_devname },
        { NULL , Error_Tag, NULL        }   // Terminating record
    };


    for (i=0; i<fdCount; i++)
    {
        sprintf(statstr,"[station %d]",i+1);

        strcpy(serial_devname,"\0");
        (void) input_config(initFilename,configs,"[common]");
        (void) input_config(initFilename,configs,statstr);

        strcpy(station[i].serDevName,serial_devname);
    }
}


/*** function readFobRecordMulti
 *
 * Purpose: Reads data from the specified number of birds, checks the sync
 *          bit if it's the beginning of a record and returns the number of
 *          a station if a record has been completely read into the buffer
 * In:      numOfStations: Number of stations to be read from
 * In/Out:  station: Pointer to the record of the stations holding the
 *                   filedescriptor of the serial interconnection, the
 *                   buffer where the record being read from the bird
 *                   is stored, as well as the length of that record.
 * Returns: -1 if a timeout occured
 *          Number of station whose record has been completed
 *          -2 else
 */
        
int readFobRecordMulti(StationRecord* station, int numOfStations)
{
    int count,i;
    fd_set result;

    // Waits for a character to arrive from any station
    result = waitForChar(station,numOfStations,1);

    count = 0;   // Counts from how many stations data has been read from

    for (i=0; i<numOfStations; i++)
    {
        if (FD_ISSET(station[i].fd, &result))    // Char waiting to be read?
        {
            if (read(station[i].fd,station[i].buffer+station[i].pos,1)==-1)
            {
                perror("read tracker");
                endwin();
                exit(1);
            }

            //Phasing bit of the first byte has to be 1
            if (station[i].pos==0 && (station[i].buffer[0] & 0x80)==0)
            {
                move(4+i,13);
                printw("!sync                        ");
                refresh();
                station[i].error = 2;
            }
            else
            {
                station[i].pos++;
                if (station[i].pos==station[i].numOfBytes) // Record complete?
                {
                    return i;                  // Record of station nr. i done
                }
            }
            count++;
        }
    }

    if (count==0)
    {
        return -1;                              // Timeout occured
    }
    return -2;
}


/*** function readFobRecordSingle
 *
 * Purpose: Reads data from a station via the master bird, checks the sync bit
 *          if it's the beginning of a record and returns the number of a
 *          station if a record has been completely read into the buffer
 * In:      numOfStations: Number of stations to be read from
 *          stNr:          Number of station which is expected to belong the
 *                         data to
 * In/Out:  station: Pointer to the record of the stations holding the
 *                   filedescriptor of the serial interconnection, the
 *                   buffer where the record being read from the bird
 *                   is stored, as well as the length of that record.
 * Returns: Number of station whose record has been completed
 *          -1 else
 */

int readFobRecordSingle(StationRecord* station, int numOfStations, int stNr)
{
    int i,j;
    fd_set result;
    char* inputBuffer;

    result = waitForCharSingle(station[0].fd,1);
    i=stNr;                           // Station number expected to read from

    if (FD_ISSET(station[0].fd, &result))
    {
        if (read(station[0].fd,station[i].buffer+station[i].pos,1)==-1)
        {
            perror("read tracker");
            endwin();
            exit(1);
        }

        //Phasing bit of the first byte has to be 1
        if (station[i].pos==0 && (station[i].buffer[0] & 0x80)==0)
        {
            move(4+i,20);
            printw("!sync                        ");
            refresh();
            station[i].error = 2;
        }
        else
        {
            station[i].pos++;
            if (station[i].pos==station[i].numOfBytes)    // Record complete?
            {
                inputBuffer = station[i].buffer;     // Get number of station
                j = (inputBuffer[(station[i].numOfBytes)-1]-1);

                if (stNr == j)                  // Is it the expected station
                {
                    return i;                 // Record of station nr. i done
                }
                else station[i].pos=0;
            }
        }
    }
    return -1;
}


/*** function buildPosition
 *
 * Purpose: Convert position bytes from Ascension into actual position float
 *          values
 * In:      inputBuffer: Buffer containing bytes received from Ascension FOB
 * In/Out:  position: Pointer to an array of floats for the position to
 *                    be put in
 */

void buildPosition(char *inputBuffer, float *position)
{
    int d,i;

    for (i=0; i<3; i++)                         // x,y,z
    {
        d=(inputBuffer[i*2]&0x7f)+(inputBuffer[i*2+1]<<7);
        if (d&0x2000) d|=0xffffc000;        // Extend signbit
        position[i] = ((float)d);
    }
}


/*** function buildQuaternion
 *
 * Purpose: Convert quaternion bytes from Ascension into actual quaternion
 *          float values
 * In:      inputBuffer: Buffer containing bytes received from Ascension
 * In/Out:  orientation: Pointer to an array of floats for the quaternion to
 *                       be put in
 */

void buildQuaternion(char *inputBuffer, float *orientation)
{
    int d,i;

    for (i=0; i<4; i++)                         // qa,qx,qy,qz
    {
        d=(inputBuffer[6+2*i]&0x7f)+(inputBuffer[7+2*i]<<7);
        if (d&0x2000) d|=0xffffc000;        // Extend signbit
        orientation[i] = ((float)d)/8192;
    }
}


/*** function buildAngles
 *
 * Purpose: Convert Euler angle bytes from Ascension into actual Euler angle
 *          (azimuth, elevation, roll) float values
 * In:      inputBuffer: Buffer containing bytes received from Ascension
 * In/Out:  angles:      Pointer to an array of floats for the angles to be
 *                       put in.
 */

void buildAngles(char *inputBuffer, float *angles)
{
    int d,i;

    for (i=0; i<3; i++)
    {
        d=(inputBuffer[6+2*i]&0x7f)+(inputBuffer[7+2*i]<<7);
        if (d&0x2000) d|=0xffffc000;        // Extend signbit
        angles[i]=((float)d)/32768*180;
    }
}


/*** function buildMatrix
 *
 * Purpose: Convert matrix bytes from Ascension into actual matrix float values
 * In:      inputBuffer: Buffer containing bytes received from Ascension
 * In/Out:  matrix:      Pointer to an array of floats for the matrix to be
 *                       put in.
 */

void buildMatrix(char *inputBuffer, float *matrix)
{
    int d,i;

    for (i=0; i<9; i++)
    {
        d=(inputBuffer[6+2*i]&0x7f)+(inputBuffer[7+2*i]<<7);
        if (d&0x2000) d|=0xffffc000;        // Extend signbit
        matrix[i]=((float)d)/32768;
    }        
}


/*** function testErrorSingleFob
 *
 * Purpose: Tests a station for an error using the master bird.
 * In:      fd:  Filedescriptor of the master bird
 *          num: Number of the bird to be tested (0 being the master)
 * Returns: -1:  Timeout while reading the master bird
 *          0:   No error
 *          n>0: Errornumber of the bird
 */

int testErrorSingleFob(int fd, int num)
{
    char str[10];                              // IObuffer
    fd_set result;
    
    oFlush(fd);  
    osSleep(100);
    iFlush(fd);
    sprintf(str,"%c%c%c",0xf1+num,0x4f,0x00);   // Request Bird Status of
    writeToSerial(fd,str,3);                // Bird 'num' from Master
    result = waitForCharSingle(fd,3);
    if (!FD_ISSET(fd, &result))
    {
#ifdef DEBUG_DATA
        fprintf(stderr,"Timeout requesting Bird Status of Bird %d from "
                       "master.\n",num);
#endif
        return(-1);                               // Timeout
    }
    read(fd,str,2);

#ifdef DEBUG_DATA
    // Print the 2 error bytes as hex
    fprintf(stderr,"Bird %d: ",num+1);
    fprintf(stderr,"TestErrorSingle: LSB: %x - MSB: %x\n",str[0],str[1]);
#endif
    
    if ( (str[1]&0x20) == 0x20)                 // Bit 13: Error if set
    {
        iFlush(fd);
        sprintf(str,"%c%c%c",0xf1+num,0x4f,0x10); // Request Errorvalue of
        writeToSerial(fd,str,3);              // Bird 'num' from Master
        result = waitForCharSingle(fd,3);
        if (!FD_ISSET(fd, &result))
        {
#ifdef DEBUG_DATA
            fprintf(stderr,"Timeout reading error value of Bird %d from "
                           "master.\n",num);
#endif
            return(-1);
        }
        read(fd,str,2);
      
        return(str[0]);                            // Return Errorvalue
    }
    return(0);                                    // No Error
}


/*** function testErrorFob
 *
 * Purpose: Tests station for an error
 * In:      fd: Filedescriptor of the station to be tested
 * Returns: -1:  Timeout
 *          0:   No Error
 *          n>0: Errornumber
 */

int testErrorFob(int fd)
{
    char str[10];                           // IObuffer
    fd_set result;

    oFlush(fd);
    osSleep(100);
    iFlush(fd);
    sprintf(str,"%c%c",0x4f,0x00);          // Request Bird Status
    writeToSerial(fd,str,2);
    result = waitForCharSingle(fd,3);
    if (!FD_ISSET(fd, &result))
    {
#ifdef DEBUG_DATA
        fprintf(stderr,"Timeout requesting Bird Status.\n");
#endif
        return(-1);                           // Timeout
    }
    read(fd,str,2);

#ifdef DEBUG_DATA
    fprintf(stderr,"TestError: LSB: %x - MSB: %x\n",str[0],str[1]);
#endif

    if ( (str[1]&0x20) == 0x20)             // Bit 13: Error if set
    {
        iFlush(fd);
        sprintf(str,"%c%c",0x4f,0x10);        // Request Errorvalue
        writeToSerial(fd,str,2);
        result = waitForCharSingle(fd,3);
        if (!FD_ISSET(fd, &result))
        {
#ifdef DEBUG_DATA
            fprintf(stderr,"Timeout reading error value\n");
#endif
            return(-1);                         // Timeout
        }
        read(fd,str,2);
        return(str[0]);                        // Return Errorvalue
    }
    return(0);                                // No Error
}


/*** function testStationsFob
 *
 * Purpose: Tests stations for an error and prints an errormessage if a timeout
 *          occured or the errorlevel is 'fatal' or 'warning1'. 'warning2' will
 *          be omitted, since the flock continues to run. 
 * In:      fd:  Pointer to an array of filedescriptors of the stations
 *          num: Number of stations to be tested for an error
 */

void testStationsFob(int *fd, int num)
{
    int i,j,k;
    char str[256];                          // String to hold the errormessage

    for (i=0; i<num; i++)
    {
        k=testErrorSingleFob(fd[0],i);    // Test station 'i' for error using
                                           // the master bird

        if (k == -1) fprintf(stderr,"Station %d: Timeout.\n",i+1);
        else
        {   
            if (k != 0)
            {
                j=errorCodeToText(k,str);          // Translate errorcode to
                                                   // errormessage
                if ( (j==-1) || (j==1) )           // Fatal or Warning1?
                {
                    fprintf(stderr,"Station %d: Error %s\n",i+1,str);
                }
            }
        }
    }
} 


/*** function birdSleep
 *
 * Purpose: Puts Flock Of Birds to Sleep
 * In:      fdMaster: Filedescriptor of the Masterbird
 */

void birdSleep(int fdMaster)
{
    writeToSerial(fdMaster,"GGG",3);        // Put FOB to Sleep
}

/*** function recoverTimeout
 *
 * Purpose: Tries to recover the stations from a timeout.
 * In:      numOfStations: Number of stations being used in the FOB
 *          station:       Pointer to the record of the stations holding the
 *                         filedescriptors of serial interconnections to the
 *                         birds
 */

void recoverTimeout(int numOfStations, StationRecord* station)
{
    int count,i;
    char str[100];

    //--- stop stream mode
    for (i=0; i<numOfStations; i++)
    {
        sprintf(str,"%c",0x42);                 // POINT cmd stops stream mode
        writeToSerial(station[i].fd,str,1);
        oFlush(station[i].fd);
        osSleep(100);
        iFlush(station[i].fd);
    }

    do
    {
        count=0;
        for (i=0; i<numOfStations; i++)
        {
            if (testErrorFob(station[i].fd) == -1)
            {
                if (i==0)
                {
                    endwin();
                    printf("Master Bird not responding. Timeout.\n\n");
                    exit(1);
                }

                move(4+i,51);
                printw("Timeout!");
                count++;
            }
            else
            {
                move(4+i,51);
                printw("        ");
            }
            refresh();
        }
    }
    while (count !=0);

    sprintf(str,"%c%c%c",0x46,0x46,0x46);              // RUN
    writeToSerial(station[0].fd,str,3);

    //--- start stream mode
    for (i=0; i<numOfStations; i++)
    {
        sprintf(str,"%c",0x40);
        writeToSerial(station[i].fd,str,1);
        oFlush(station[i].fd);
    }                                   

    osSleep(100);
}


/*** function writeFob
 *
 * Purpose: Writes a string to a station using the master bird
 * In:      fd:      Pointer to array of filedescriptors of the stations
 *          station: Number of the station to be written
 *          str:     Pointer to string to be written
 *          length:  Length of the string
 */

void writeFob(int *fd, int station, char* str, int length)
{
    char c[4];                               // Outputbuffer

    sprintf(c,"%c",0xf1+station);            // Preface number of station
    writeToSerial(fd[0],c,1);                // Write to master
    writeToSerial(fd[0],str,length);         // Write the string
    oFlush(fd[0]);
}


/*** function writeAllFob
 *
 * Purpose: Writes a string to a number of stations using the master bird
 * In:      fd:      Pointer to array of filedescriptors of the stations
 *          fdCount: Number of stations to be written
 *          str:     Pointer to string to be written
 *          length:  Length of the string
 */

void writeAllFob(int *fd, int fdCount, char* str, int length)
{
    int i; 

    for (i=0; i<fdCount; i++)
    {
        writeFob(fd,i,str,length);         // Write string to station 'i'
    }                                       // using the master bird
}


/*** function checkPositionScaling
 *
 * Purpose: Checks if the position scaling has a valid value (36 or 72 inch)
 *          and prepares the commandstring to be sent to the tracker
 * In:      position_scaling: value of the 'position_scaling' option in the
 *                            configfile
 * In/Out:  str: Pointer to string which is supposed to be sent to the
 *               tracker in order to set the position scaling
 */

void checkPositionScaling(int position_scaling, char* str)
{
    if (position_scaling == 36)
    {
        sprintf(str,"%c%c%c&c",0x50,0x03,0x00,0x00);
    }
    else 
    {
        if (position_scaling == 72)
        {
            sprintf(str,"%c%c%c%c",0x50,0x03,0x10,0x00);
        }
        else
        {
            fprintf(stderr,"Position scaling can only be 36 or 72, not %d.\n",
                    position_scaling);
            exit(1);
        }
    }
}


/*** function checkXyzReferenceFrame
 *
 * Purpose: Parses 'xyz_reference_frame' option from the config file and
 *          prepares the commandstring which is supposed to be sent to the
 *          tracker in order to set the XYZ reference frame
 * In:      xyz_reference_frame: Pointer to string of the option
 * In/Out:  str:    Pointer to string which is supposed to be sent to the
 *                  tracker in order to set the XYZ reference frame
 */

void checkXyzReferenceFrame(char* xyz_reference_frame, char* str)
{
    if (strcasecmp(xyz_reference_frame,"off") == 0)
    {
        sprintf(str,"%c%c%c",0x50,0x11,0x00);
    }  
    else
    {
        if (strcasecmp(xyz_reference_frame,"on") == 0)
        {
            sprintf(str,"%c%c%c",0x50,0x11,0x01);
        }  
        else
        {
            fprintf(stderr,"XYZ Reference Frame can only be on or off, "
                           "not '%s'.\n",xyz_reference_frame);
            exit(1);
        }
    }
}


/*** function checkHemisphere
 *
 * Purpose: Parses 'hemisphere' option from the config file and prepares the
 *          commandstring which is supposed to be sent to the tracker in order
 *          to set the hemisphere
 * In:      hemisphere: Pointer to string of the option
 * In/Out:  str:        Pointer to string which is supposed to be sent to the
 *                      tracker in order to set the hemisphere 
 */

void checkHemisphere(char* hemisphere, char* str)
{
    int i;

    i=0;

    if (strcasecmp(hemisphere,"Forward") == 0)
    { sprintf(str,"%c%c%c",0x4c,0x00,0x00); i=1; }

    if (strcasecmp(hemisphere,"Aft") == 0)
    { sprintf(str,"%c%c%c",0x4c,0x00,0x01); i=1; }  

    if (strcasecmp(hemisphere,"Upper") == 0)
    { sprintf(str,"%c%c%c",0x4c,0x0c,0x01); i=1; }  

    if (strcasecmp(hemisphere,"Lower") == 0)
    { sprintf(str,"%c%c%c",0x4c,0x0c,0x00); i=1; }  

    if (strcasecmp(hemisphere,"Left") == 0)
    { sprintf(str,"%c%c%c",0x4c,0x06,0x01); i=1; }  

    if (strcasecmp(hemisphere,"Right") == 0)
    { sprintf(str,"%c%c%c",0x4c,0x06,0x00); i=1; }  

    if (i == 0)
    {
        fprintf(stderr,"Hemisphere can only be Forward, Aft, Upper, Lower, "
                       "Left or Right, not '%s'.\n",hemisphere);
        exit(1);
    }
}


/*** function checkInit
 *
 * Purpose: Parses 'init' option from the config file and prepares the
 *          commandstring which is supposed to be sent to the tracker
 * In:      str:    Pointer to string of the option
 * In/Out:  cmdstr: Pointer to string which is supposed to be sent to the
 *                  tracker
 * Returns: number of bytes of the cmdstr
 */

int checkInit(char* str, char* cmdstr)
{
    int i,length,c;
    char num[10];
    char *end;

    if (strlen(str) != 0)
    {
        i=0;
        length=0;
        while (i<strlen(str))
        {
            if (str[i]=='\\')            // Hex sequence starting with '/0x'
            {
                i++;
                if (i+4>strlen(str) || strncmp(str+i,"0x",2)!= 0)
                {
                    fprintf(stderr,"Invalid escape sequence\n");
                    exit(1);
                }
                num[0]=str[i+2];        // Convert hex sequence to int
                num[1]=str[i+3];
                num[2]=0;
                c = strtol(num,&end,16);
                if (*end!='\0')
                {
                    fprintf(stderr,"Invalid hex number %s\n",num);
                    exit(1);
                }

                cmdstr[length]=c;
                i+=4;
                length++;                     
            }
            else
            {
                cmdstr[length]=str[i];
                i++;
                length++;
            }
        }
    return length;
    }
    return 0;
}


/*** function checkReferenceFrame
 *
 * Purpose: Checks azimuth, elevation and roll of the reference frame.
 * In:      rf_azimuth:   Azimuth (Z angle in degrees, range -180 to 180)
 *          rf_elevation: Elevation (Y angle in degrees, range -90 to 90)
 *          rf_roll:      Roll (X angle in degrees, range -180 to 180)
 * In/Out:  str: Pointer to string which is supposed to be sent to the tracker
 */

void checkReferenceFrame(float rf_azimuth, float rf_elevation, float rf_roll,
                         char* str)
{
    int rf_a,rf_e,rf_r;

    if ((rf_azimuth > 180.0) || (rf_azimuth < -180.0))
    {
        fprintf(stderr,"'rf_azimuth' must be in range -180 to 180, not "
                       "%f.\n",rf_azimuth);
        exit(1);
    }

    if ((rf_elevation > 90.0) || (rf_elevation < -90.0))
    {
        fprintf(stderr,"'rf_elevation' must be in range -90 to 90, not "
                       "%f.\n",rf_elevation);
        exit(1);
    }

    if ((rf_roll > 180.0) || (rf_roll < -180.0))
    {
        fprintf(stderr,"'rf_roll' must be in range -180 to 180, not "
                       "%f.\n",rf_roll);
        exit(1);
    }

    rf_a=(int)(rf_azimuth/180*32767);
    rf_e=(int)(rf_elevation/180*32767);
    rf_r=(int)(rf_roll/180*32767);

    sprintf(str,"%c%c%c%c%c%c%c",0x72,rf_a,rf_a>>8,rf_e,rf_e>>8,rf_r,rf_r>>8);
}


/*** function readCfgFile
 *
 * Purpose: Reads the config file and writes the options to the stations of
 *          the tracker accordingly
 * In:      fd:           Pointer to array of filedescriptors of the stations
 *          fdCount:      Number of stations
 *          initFilename: Pointer to the name of the config file
 * In/Out:  station: Pointer to the record of the station used to keep
 *                   track of its properties
 */

void readCfgFile(int *fd, int fdCount, char* initFilename,
                 StationRecord* station)
{
    char str[1000];
    int i,j,k;
    char statstr[20]="[station 1]";
    int rf_a,rf_e,rf_r;

                     //--- Vars with their default values used for the options
    char hemisphere[256]="Forward";
    char xyz_reference_frame[256]="off";
    int position_scaling=36;
    char init[1000]="\0";
    float scaleX=1.0,scaleY=1.0,scaleZ=1.0;
    float rf_azimuth=0.0, rf_elevation=0.0, rf_roll=0.0;
    char serial_devname[256]="\0";
#ifdef DEBUG_DATA
    char format[256]="Position_Quaternion";
    char positionFilter[256]="No_Filter";
    int positionFilterParam=40;
    char orientationFilter[256]="No_Filter";
    int orientationFilterParam=40;
    char parallel_devname[8][256];
    short int basePort[8];
    char buttons[8][256];
    char stationname[256]="\0";
    char servername[256]="\0";
#endif

    struct Config_Tag configs[] =
    {   // Valid options
        { "hemisphere", String_Tag, hemisphere },
        { "position_scaling", Word_Tag, &position_scaling },
        { "xyz_reference_frame", String_Tag, xyz_reference_frame },
        { "init", String_Tag, init },
        { "rf_azimuth", Float_Tag, &rf_azimuth },
        { "rf_elevation", Float_Tag, &rf_elevation },
        { "rf_roll", Float_Tag, &rf_roll },
        { "serial_devname", String_Tag, serial_devname },
#ifdef DEBUG_DATA
        { "format", String_Tag, format },
        { "scale_x", Float_Tag, &scaleX },
        { "scale_y", Float_Tag, &scaleY },
        { "scale_z", Float_Tag, &scaleZ },
        { "position_filter", String_Tag, positionFilter },
        { "position_filter_param", Word_Tag, &positionFilterParam },
        { "orientation_filter", String_Tag, orientationFilter },
        { "orientation_filter_param", Word_Tag, &orientationFilterParam },
        { "stationname", String_Tag, stationname },
        { "servername", String_Tag, servername },
        { "button1", String_Tag, buttons[0] },
        { "button2", String_Tag, buttons[1] },
        { "button3", String_Tag, buttons[2] },
        { "button4", String_Tag, buttons[3] },
        { "button5", String_Tag, buttons[4] },
        { "button6", String_Tag, buttons[5] },
        { "button7", String_Tag, buttons[6] },
        { "button8", String_Tag, buttons[7] },
        { "parallel1_devname", String_Tag, parallel_devname[0] },
        { "parallel2_devname", String_Tag, parallel_devname[1] },
        { "parallel3_devname", String_Tag, parallel_devname[2] },
        { "parallel4_devname", String_Tag, parallel_devname[3] },
        { "parallel5_devname", String_Tag, parallel_devname[4] },
        { "parallel6_devname", String_Tag, parallel_devname[5] },
        { "parallel7_devname", String_Tag, parallel_devname[6] },
        { "parallel8_devname", String_Tag, parallel_devname[7] },
        { "baseport1", HexWord_Tag, &basePort[0] },
        { "baseport2", HexWord_Tag, &basePort[1] },
        { "baseport3", HexWord_Tag, &basePort[2] },
        { "baseport4", HexWord_Tag, &basePort[3] },
        { "baseport5", HexWord_Tag, &basePort[4] },
        { "baseport6", HexWord_Tag, &basePort[5] },
        { "baseport7", HexWord_Tag, &basePort[6] },
        { "baseport8", HexWord_Tag, &basePort[7] },
#endif
        { NULL , Error_Tag, NULL        }                // Terminating record
    };


#ifdef DEBUG_DATA
    for (i=0; i<8; i++)
    {
        basePort[i]=0;
        strcpy(parallel_devname[i],"\0");
    }
#endif

    (void) input_config(initFilename,configs,"[common]");      // Read options

#ifdef DEBUG_DATA
    fprintf(stderr,"\n[common]\n");
    fprintf(stderr,"hemisphere=%s\n",hemisphere);
    fprintf(stderr,"position_scaling=%d\n",position_scaling);
    fprintf(stderr,"xyz_reference_frame=%s\n",xyz_reference_frame);
    fprintf(stderr,"init=%s\n",init);
    fprintf(stderr,"format=%s\n",format);
    for (i=0; i<8; i++) fprintf(stderr,"button%d=%s\n",i+1,buttons[i]);
    fprintf(stderr,"scale_x=%f\n",scaleX);
    fprintf(stderr,"scale_y=%f\n",scaleY);
    fprintf(stderr,"scale_z=%f\n",scaleZ);
    fprintf(stderr,"rf_azimuth=%f\n",rf_azimuth);
    fprintf(stderr,"rf_elevation=%f\n",rf_elevation);
    fprintf(stderr,"rf_roll=%f\n",rf_roll);
    fprintf(stderr,"parallel1_devname=%s\n",parallel_devname[0]);
    fprintf(stderr,"baseport1=%x\n",basePort[0]);
    fprintf(stderr,"parallel2_devname=%s\n",parallel_devname[1]);
    fprintf(stderr,"baseport2=%x\n",basePort[1]);
    fprintf(stderr,"parallel3_devname=%s\n",parallel_devname[2]);
    fprintf(stderr,"baseport3=%x\n",basePort[2]);
    fprintf(stderr,"parallel4_devname=%s\n",parallel_devname[3]);
    fprintf(stderr,"baseport4=%x\n",basePort[3]);
    fprintf(stderr,"parallel5_devname=%s\n",parallel_devname[4]);
    fprintf(stderr,"baseport5=%x\n",basePort[4]);
    fprintf(stderr,"parallel6_devname=%s\n",parallel_devname[5]);
    fprintf(stderr,"baseport6=%x\n",basePort[5]);
    fprintf(stderr,"parallel7_devname=%s\n",parallel_devname[6]);
    fprintf(stderr,"baseport7=%x\n",basePort[6]);
    fprintf(stderr,"parallel8_devname=%s\n",parallel_devname[7]);
    fprintf(stderr,"baseport8=%x\n",basePort[7]);
    fprintf(stderr,"servername=%s\n",servername);
    fprintf(stderr,"position_filter=%s\n",positionFilter);
    fprintf(stderr,"position_filter_param=%d\n",positionFilterParam);
    fprintf(stderr,"orientation_filter=%s\n",orientationFilter);
    fprintf(stderr,"orientation_filter_param=%d\n\n",orientationFilterParam);
#endif

    j=checkInit(init, str);
    if (j>0) writeAllFob(fd,fdCount,str,j);

    for (i=0; i<fdCount; i++)
    {
        sprintf(statstr,"[station %d]",i+1);
        strcpy(hemisphere,"Forward");
        strcpy(xyz_reference_frame,"off");
        position_scaling=36;
        strcpy(init,"\0");
        rf_azimuth=0.0;
        rf_elevation=0.0;
        rf_roll=0.0;
        strcpy(serial_devname,"\0");
#ifdef DEBUG_DATA
        strcpy(format,"Position_Quaternion");
        scaleX=1.0;
        scaleY=1.0;
        scaleZ=1.0;
        strcpy(positionFilter,"No_Filter");
        positionFilterParam=40;
        strcpy(orientationFilter,"No_Filter");
        orientationFilterParam=40;

        for (j=0; j<8; j++) strcpy(buttons[j],"None");
        strcpy(stationname,"\0");
#endif

        (void) input_config(initFilename,configs,"[common]");
        (void) input_config(initFilename,configs,statstr);

#ifdef DEBUG_DATA
        fprintf(stderr,"[station %d]\n",i+1);
        fprintf(stderr,"hemisphere=%s\n",hemisphere);
        fprintf(stderr,"position_scaling=%d\n",position_scaling);
        fprintf(stderr,"xyz_reference_frame=%s\n",xyz_reference_frame);
        fprintf(stderr,"init=%s\n",init);
        fprintf(stderr,"format=%s\n",format);
        for (j=0; j<8; j++) fprintf(stderr,"button%d=%s\n",j+1,buttons[j]);
        fprintf(stderr,"scale_x=%f\n",scaleX);
        fprintf(stderr,"scale_y=%f\n",scaleY);
        fprintf(stderr,"scale_z=%f\n",scaleZ);
        fprintf(stderr,"rf_azimuth=%f\n",rf_azimuth);
        fprintf(stderr,"rf_elevation=%f\n",rf_elevation);
        fprintf(stderr,"rf_roll=%f\n",rf_roll);
        fprintf(stderr,"serial_devname=%s\n",serial_devname);
        fprintf(stderr,"stationname=%s\n",stationname);
        fprintf(stderr,"position_filter=%s\n",positionFilter);
        fprintf(stderr,"position_filter_param=%d\n",positionFilterParam);
        fprintf(stderr,"orientation_filter=%s\n",orientationFilter);
        fprintf(stderr,"orientation_filter_param=%d\n\n",
                orientationFilterParam);
#endif

        k=0;
        for (j=0; j<fdCount; j++) { if (station[j].erc == fobErc) k=1; }
     
        if (k == 0 && ercId <= 0 )  // Do not use position scaling if ERC is present
        {
            checkPositionScaling(position_scaling,str);
            writeFob(fd,i,str,4);
        }

        if (station[i].erc != fobErc)     // Do not send these commands to ERC
        {
            checkHemisphere(hemisphere, str);
              writeFob(fd,i,str,3);

            checkXyzReferenceFrame(xyz_reference_frame,str);
            writeFob(fd,i,str,3);
        }

        if ((rf_azimuth != 0) || (rf_elevation != 0) || (rf_roll !=0))
        {
            checkReferenceFrame(rf_azimuth,rf_elevation,rf_roll,str);
            writeFob(fd,i,str,7);
        }

        j=checkInit(init, str);
        if (j>0) writeFob(fd,i,str,j);
    }
}


/*** function readScales
 *
 * Purpose: Reads the scalefactor of all stations and sets the properties
 *          accordingly
 * In:      fd:      Pointer to array of filedescriptors of the stations
 *          fdCount: Number of stations
 * In/Out:  station: Pointer to the record of the station used to keep
 *                   track of its properties
 */

void readScales(int* fd, int fdCount, StationRecord* station)
{
    fd_set result;
    char str[100];
    int i,count;
    
    // if ERC present, scale is always 144 inches, otherwise read it from bird
    // gr2000-12-05
    if( ercId > 0 )
    {
        for(i=0; i<fdCount; i++)
        {
            station[i].scaleX = (144*inchesToMeters/8192)*station[i].scaleX;
            station[i].scaleY = (144*inchesToMeters/8192)*station[i].scaleY;
            station[i].scaleZ = (144*inchesToMeters/8192)*station[i].scaleZ;
            printf("Station %d: scale_x=%f; scale_y=%f; scale_z=%f\n",
               i+1,station[i].scaleX,station[i].scaleY,station[i].scaleZ);
        }
    } else {
        for (i=0; i<fdCount; i++)
        {
            iFlush(fd[0]);
            sprintf(str,"%c%c%c",0xf1+i,0x4f,0x03);  // Examine Value Pos. scaling
            writeToSerial(fd[0],str,3);

            count = 0;
            do
            {
                result = waitForCharSingle(fd[0],3);
                if (!FD_ISSET(fd[0], &result))
                {
                    fprintf(stderr,"Timeout of station: %d\n",i+1);
                    exit(1);
                }
                read(fd[0],str+count,1);
#ifdef DEBUG_DATA
                fprintf(stderr,"byte %d is %d\n",count,str[count]);
#endif
                count++;
            }
            while (count<2);

            if (str[0]!=0)
            {   //72 inch
                station[i].scaleX = (72*inchesToMeters/8192)*station[i].scaleX;
                station[i].scaleY = (72*inchesToMeters/8192)*station[i].scaleY;
                station[i].scaleZ = (72*inchesToMeters/8192)*station[i].scaleZ;
            }
            else
            {   //36 inch
                station[i].scaleX = (36*inchesToMeters/8192)*station[i].scaleX;
                station[i].scaleY = (36*inchesToMeters/8192)*station[i].scaleY;
                station[i].scaleZ = (36*inchesToMeters/8192)*station[i].scaleZ;
            }

            printf("Station %d: scale_x=%f; scale_y=%f; scale_z=%f\n",
                   i+1,station[i].scaleX,station[i].scaleY,station[i].scaleZ);
        }
    }
}

/*** function readStatus
 *
 * Purpose: Reads the Software Revision Number, the Bird Computer Crystal
 *          Speed and the System Model Identification and prints it.
 * In:      fd:      Pointer to array of filedescriptors of the stations
 *          fdCount: Number of stations
 */

void readStatus(int *fd, int fdCount, StationRecord* station) 
{
    int i;
    fd_set result;
    char str[10];
    char str2[10];
    char str3[20];

    for (i=0; i<fdCount; i++)
    {      
        iFlush(fd[0]);
        sprintf(str,"%c%c%c",0xf1+i,0x4f,0x01);    // Software Revision Number
        writeToSerial(fd[0],str,3);
        result = waitForCharSingle(fd[0],3);
        if (!FD_ISSET(fd[0], &result))
        {
            fprintf(stderr,"Station %d: Timeout.\n",i+1);
            exit(1);
        }
        read(fd[0],str,2);

        iFlush(fd[0]);
        sprintf(str2,"%c%c%c",0xf1+i,0x4f,0x02);// Bird computer crystal speed
        writeToSerial(fd[0],str2,3);
        result = waitForCharSingle(fd[0],3);
        if (!FD_ISSET(fd[0], &result))
        {
            fprintf(stderr,"Station %d: Timeout.\n",i+1);
            exit(1);
        }
        read(fd[0],str2,2);

        iFlush(fd[0]);
        sprintf(str3,"%c%c%c",0xf1+i,0x4f,0x0f);// System Model Identification
        writeToSerial(fd[0],str3,3);
        result = waitForCharSingle(fd[0],3);
        if (!FD_ISSET(fd[0], &result))
        {
            fprintf(stderr,"Station %d: Timeout.\n",i+1);
            exit(1);
        }
        read(fd[0],str3,10);
        str3[10]=0;

        printf("Station %d: %s - %d Mhz - PROM Software Revision %d.%d\n",i+1,
               str3,str2[0],str[0],str[1]);

        if (strncmp(str3,"6DERC",5) == 0) station[i].erc = fobErc;
        else station[i].erc = fobBird;
    }
}


/*** function readFlockStatus
 *
 * Purpose: Reads the Flock System Status and prints out the information
 * In:      fd:      Pointer to array of filedescriptors of the stations
 *          fdCount: Number of stations
 */

void readFlockStatus(int *fd, int fdCount)
{
    char str[100];
    int count;
    fd_set result;

    iFlush(fd[0]);
    sprintf(str,"%c%c",0x4f,0x24);                      // Flock System Status
    writeToSerial(fd[0],str,2);
    oFlush(fd[0]);

    count = 0;
    do
    {
        result = waitForCharSingle(fd[0],3);
        if (!FD_ISSET(fd[0], &result))
        {
            fprintf(stderr,"Timeout reading Flock System Status.\n");
            exit(1);
        }
        read(fd[0],str+count,1);

#ifdef DEBUG_DATA
        fprintf(stderr,"Station %d: status %d %c %x\n",count+1,
                str[count],str[count],str[count]);
#endif

        printf("Station %d: ",count+1);
        if ( str[count] == 0) printf("No Bird");
        else
        {
            if ( (str[count]&0x80) == 0x80) printf("FBB accessible");
            else printf("FBB not accessible");
            if ( (str[count]&0x40) == 0x40) printf("-Running");
            else printf("-not Running");
            if ( (str[count]&0x20) == 0x20) printf("-Receiver");
            else printf("-no Receiver");
            if ( (str[count]&0x10) == 0x10) printf("-ERT");
        if ( (str[count]&0x8) == 0x8) printf("-ERT3");
            if ( (str[count]&0x4) == 0x4) printf("-ERT2");
            if ( (str[count]&0x2) == 0x2) printf("-ERT1");
            if ( (str[count]&0x1) == 0x1) printf("-ERT0/standard RT");
        }
        printf("\n");
        count++;
    }
    while (count<maxBirds);
}


/*** function configTracker
 *
 * Purpose: Uses Autoconfig to configure the stations and sets the default
 *          modes Group Mode off, Stream Mode off and Position/Angles
 * In:      fd:      Pointer to array of filedescriptors of the stations
 *          fdCount: Number of stations
 */

void configTracker(int *fd, int fdCount)
{
    int i;
    char str[10];

    sprintf(str,"%c%c%c",0x50,0x23,0x00);                // Set Group Mode off
    writeToSerial(fd[0],str,3);
    oFlush(fd[0]);
    osSleep(300);

                                                // Stop stream and button mode
    for (i=0; i<fdCount; i++)    
    {
        sprintf(str,"%c%c",0xf1+i,0x42);    
        writeToSerial(fd[0],str,2);                        // Stop Stream Mode
        oFlush(fd[0]);
        osSleep(300);
        iFlush(fd[0]);

        sprintf(str,"%c%c%c",0xf1+i,0x4d,0x00);         // Set Button Mode off
        writeToSerial(fd[0],str,3);
        oFlush(fd[0]);
    }

                                                        // Autoconfig Trackers
    osSleep(1000);
    iFlush(fd[0]);
    printf("Tracker autoconfig\n");
    if( ercId > 0 )            // if additional ERC, autoconfig with one more
    {
        sprintf(str,"%c%c%c",0x50,0x32,fdCount+1);    // Autoconfig with ERC
    } else {    
        sprintf(str,"%c%c%c",0x50,0x32,fdCount);      // Issue Autoconfig
    }
    writeToSerial(fd[0],str,3);
    osSleep(1000);
    oFlush(fd[0]);
 
    if( ercId > 0 )        // if additional ERC present, send next transmitter
    {                      // gr2000-12-05
        iFlush(fd[0]);
        printf("Tracker activate additional ERC bird at %i\n",ercId);
        sprintf(str,"%c%c",0x30,ercId<<4);
        writeToSerial(fd[0],str,2);
        osSleep(1000);
        oFlush(fd[0]);
    }


                   // Set Position/Angles and Stream mode off for all stations
    for (i=0; i<fdCount; i++)
    {
        iFlush(fd[0]);
        sprintf(str,"%c%c",0xf1+i,0x59);                    // Position_Angles
        writeToSerial(fd[0],str,2);
        sprintf(str,"%c%c",0xf1+i,0x42);                    // Stream mode off
        writeToSerial(fd[0],str,2);
        osSleep(100);
        iFlush(fd[0]);
    }
}


/*** function setStationMode
 *
 * Purpose: Set the output record format of the stations and its properties
 *          according to the config file
 * In:      fd:      Pointer to array of filedescriptors of the stations
 *          fdCount: Number of stations
 *          mode:    Either fobMulti or fobSingle
 * In/Out:  station: Pointer to the record of the station used to keep
 *                   track of its properties
 */

void setStationMode(int *fd, int fdCount, int mode, StationRecord* station)
{
    int i;
    char str[100];
    char str2[10];

    //--- set station mode according to format
    for (i=0; i<fdCount; i++)
    {
        station[i].numOfBytes=14;

        if (station[i].erc != fobErc)                     // Don't send to ERC
        {
            if (station[i].mode == positionQuaternion)
            {
                sprintf(str,"position/quaternion");
                sprintf(str2,"%c%c",0xf1+i,0x5d);
                station[i].numOfBytes=14;       // Record consists of 14 bytes
            }

            if (station[i].mode == positionAngles)
            {
                sprintf(str,"position/angles");
                sprintf(str2,"%c%c",0xf1+i,0x59);
                station[i].numOfBytes=12;       // Record consists of 12 bytes
            }
        
            if (station[i].mode == positionMatrix)
            {
                sprintf(str,"position/matrix");
                sprintf(str2,"%c%c",0xf1+i,0x5a);
                station[i].numOfBytes=24;       // Record consists if 24 bytes
            }

             // Using Group Mode every record from a station has an extra byte
             // containing the address of the bird
            if (mode == fobSingle) { station[i].numOfBytes++; }

            printf("Station %d set to %s\n",i+1,str);
            writeToSerial(fd[0],str2,2);
            oFlush(fd[0]);
        }
    }
}


/*** function startFlock
 *
 * Purpose: Starts Stream Mode and enables Button Mode and/or Group Mode
 *          if necessary
 * In:      fdCount: Number of stations
 *          mode:    Mode of operation, either fobSingle or fobMulti
 * In/Out:  station: Pointer to the record of the station used to keep
 *                   track of its properties
 */

void startFlock(int fdCount, int mode, StationRecord* station)
{
    int i;
    char str[10];

    if ( mode != fobSingle ) if ( mode != fobMulti) return;

    printf("Starting button mode\n");
    for (i=0; i<fdCount; i++)
    {
        if (station[i].buttonMode == buttonTracker)
        {
            station[i].numOfBytes++;            
            sprintf(str,"%c%c%c",0xf1+i,0x4d,0x01);      // Set Button Mode on
            writeToSerial(station[0].fd,str,3);
            oFlush(station[0].fd);
        }
    }

    if (mode == fobSingle)
    {
        printf("Starting group mode\n");
        sprintf(str,"%c%c%c",0x50,0x23,0x01);             // Set Group Mode on
        writeToSerial(station[0].fd,str,3);
        oFlush(station[0].fd);
        osSleep(1000);
    }

    printf("Starting stream mode\n");
    for (i=0; i<fdCount; i++)
    {
        if ((mode == fobMulti) || (i == 0))       // With fobSingle only start
        {                                         // stream mode at the master
            sprintf(str,"%c%c",0xf1+i,0x40);      // Start Stream Mode
            writeToSerial(station[0].fd,str,2);
            oFlush(station[0].fd);
            osSleep(50);
        }
    }
}


/*** function initTrackersFob
 *
 * Purpose: Initializes the stations of the tracker with the config file
 * In:      initFilename: Pointer to the name of the config file
 *          mode:         Mode of operation, either fobSingle or fobMulti
 *          fdCount:      Number of stations in the Flock Of Birds
 * In/Out:  station: Pointer to the record of the stations used to keep
 *                   track of its properties
 */

void initTrackersFob(char* initFilename, int mode, StationRecord* station,
                     int fdCount)
{
    getSerialDevNames(initFilename,station,fdCount);

    openSerialToFob(station,fdCount,mode);       // init the serial interfaces
    printf("\n");

    int *fd = new int[fdCount];
    for (int i=0; i<fdCount; i++) fd[i]=station[i].fd;

    configTracker(fd,fdCount);                          // AutoConfig Trackers
    printf("\n");

    testStationsFob(fd,fdCount);               // Test all stations for errors

    readStatus(fd,fdCount,station);             // Read status of all trackers
    printf("\n");

    readFlockStatus(fd,fdCount);                // Read and print Flock Status
    printf("\n");

    testStationsFob(fd,fdCount);               // Test all stations for errors

    readCfgFile(fd,fdCount,initFilename,station);   // Read configuration file

    readScales(fd,fdCount,station);                             // Read scales
    printf("\n");

    setStationMode(fd,fdCount,mode,station);      // Sets output record format
    printf("\n");

    testStationsFob(fd,fdCount);               // Test all stations for errors

    delete [] fd;
}
