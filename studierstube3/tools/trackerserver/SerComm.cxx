// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SerComm
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of serialcommunication functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:19:53  rr      last modification
// ===========================================================================

#include "SerComm.h"

extern char serialBaudRate[];

/*** function iFlush
 *
 * Purpose: Flushes data received but not read from the input buffer of the
 *          given filedescriptor
 * In:      fd: Filedescriptor of the object to be flushed
 */

void iFlush(int fd)
{
    tcflush(fd, TCIFLUSH);                  // draining input buffer of junk
}


/*** function oFlush
 *
 * Purpose: Flushes data written but not transmitted from the output buffer
 *          of the given filedescriptor
 * In:      fd: Filedescriptor of the object to be flushed
 */

void oFlush(int fd)
{
    tcflush(fd, TCOFLUSH);                  // draining output buffer of junk
}


/*** function openSerialDevFob
 *
 * Purpose: Opens the serial device as raw I/O and sets the specified baud
 *          rate for the Flock Of Birds
 * In:      device:   Pointer to a string with the device name
 *          baudrate: Pointer to a string with the baudrate
 * Returns: The filedescriptor of the opened device or -1 if it failed
 */

int openSerial4Fob(const char *device, const char *baudrate)
{
    int fd, speed;
    long baud;
    struct termios status;

    //--- open device
    fd=open(device, O_RDWR);
    if (fd==-1)
    {
        fprintf(stderr,"Can't open device %s\n", device);
        return -1;
    }

    //--- baud rate constants
    baud=atol(baudrate);
    switch (baud)
    {
        case 300:    speed=B300;    break;
        case 1200:   speed=B1200;   break;
        case 2400:   speed=B2400;   break;
        case 4800:   speed=B4800;   break;
        case 9600:   speed=B9600;   break;
        case 19200:  speed=B19200;  break;
        case 38400:  speed=B38400;  break;
        case 57600:  speed=B57600;  break;
        case 115200: speed=B115200; break;
        default:     fprintf(stderr,"Unknown baud rate %d\n",baud);
                     return -1;
    }

    //--- configure attributes

    tcgetattr(fd, &status);

    cfmakeraw(&status);            //set RAW mode

    //--- ignore break condition and characters with parity errors
    //--- enable any character to restart output
    status.c_iflag = IGNBRK | IGNPAR | IXANY ;
    status.c_oflag = 0;                             //no output modifications
    status.c_cflag = CS8 | CREAD | HUPCL | CLOCAL ; //8data, 1stop, no parity
    status.c_lflag = 0;                          //disable all signals, echos

    if (cfsetispeed(&status, speed)==-1)
    {
        fprintf(stderr,"Can't set input speed of device %s\n",device);
        return -1;
    }

    if (cfsetospeed(&status, speed)==-1)
    {
        fprintf(stderr,"Can't set output speed of device %s\n",device);
        return -1;
    }

    if (tcsetattr(fd, TCSANOW, &status)==-1)
    {
        fprintf(stderr,"Can't set attributes of device %s\n", device);
        perror("Trackerserver");
        return -1;
    }
    iFlush(fd);

    return fd;
}


/*** function openSerialToFob
 *
 * Purpose: Opens the serial interface(s) to the tracker
 * In:      fdCount: Number of stations in the Flock Of Birds
 *          mode:    fobSingle (opening only one serial interface to the
 *                   master bird) or FOBmulti (opening individual serial
 *                   interfaces to each bird)
 * In/Out:  station: Pointer to an array of records holding the names of the
 *                   serial devices and where the filedescriptors of the
 *                   serial connections to the stations will be stored
 */

void openSerialToFob(StationRecord* station, int fdCount, int mode)
{
    int modem,i;

    for (i=0; i<fdCount; i++)
    {
      // if fobSingle only open first serial interface, otherwise open
      // individual interfaces to the birds
        if ((mode == fobMulti) || (i == 0))
        {
            printf("Initialising serial interface %s to Station%d\n",
                    station[i].serDevName,i+1);
            //changed baud rate to use paramter - ds2000-03-24
            station[i].fd=openSerial4Fob(station[i].serDevName,serialBaudRate);
            if (station[i].fd==-1)
            {
                perror("Error opening tracker!\n");
                exit(1);
            } 
            if (ioctl(station[i].fd,TIOCMGET,&modem)==-1)
            {
                perror("Can't read modem lines");
                exit(1);
            }
            modem &= ~TIOCM_RTS;
            if (ioctl(station[i].fd,TIOCMSET,&modem)==-1)
            {
                perror("Can't set modem lines");
                exit(1);
            }
        }
    }
}


/*** function waitForChar
 *
 * Purpose: Monitors the serial connections for a specified time until a
 *          character arrives
 * In:      station: Pointer to the record of the stations containing the
 *                   filedescriptor of the serial interconnection to the
 *                   station
 *          fdCount: Number of stations to be monitored
 *          waitime: Amount of time before timeout assumed
 * Returns: Descriptor set of the monitored stations
 */

fd_set waitForChar(StationRecord* station, int fdCount, int waitime)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    int i;

    // find out max fd to be monitored for a character
    int fdmax = 0;
    for (i=0; i<fdCount; i++)
        if (station[i].fd>fdmax) fdmax = station[i].fd;
        fdmax++;

    // loop while interrupted by signals
    while (1)
    {
        //--- setup
        tv.tv_sec = waitime;   // timeout
        tv.tv_usec = 0;

        FD_ZERO(&rfds);
        for (i=0; i<fdCount; i++) FD_SET(station[i].fd, &rfds);

        retval = select(fdmax, &rfds, NULL, NULL, &tv);
        /* assuming this is linux, tv should contain the remaining time */

        if (retval==0)
        {
            FD_ZERO(&rfds);
            return rfds;
        }
        else if (retval>0)
        {
        return rfds;
        }
        else if (errno!=EINTR)
        { /* any other error */
            perror("nonblockGetchar");
            exit(-1);
        } /* loop if interupted */
    }
}


/*** function waitForCharSingle
 *
 * Purpose: Monitors one serial interface for a specified time until a 
 *          character arrives.
 * In:      fd:      Filedescriptor of the serial interconnection to the
 *                   station which will be monitored
 *          waitime: Amount of time before timeout assumed
 * Returns: Descriptor set of the monitored station
 */

fd_set waitForCharSingle(int fd, int waitime)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    // loop while interrupted by signals
    while (1)
    {
        tv.tv_sec = waitime;   // timeout
        tv.tv_usec = 0;

        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
        /* assuming this is linux, tv should contain the remaining time */

        if (retval==0)
        {
            FD_ZERO(&rfds);
            return rfds;
        }
        else if (retval>0)
        {
        return rfds;
        }
        else if (errno!=EINTR)
        { /* any other error */
            perror("nonblockGetchar");
            exit(-1);
        } /* loop if interupted */
    }
}


/*** function writeToSerial
 *
 * Purpose: Writes data to the serial interface
 * In:      fd:  Filedescriptor of the serial interconnection to the
 *               station which the data will be written to
 *          str: Pointer to a string of characters to be written
 *          len: Number of characters to be written
 */

void writeToSerial(int fd, char *str, int len)
{
    int i;

    for (i=0; i<len; i++)
    {
        write(fd, str++, 1);
        tcdrain(fd);
        osSleep(10);
    }
}

/******************************************************************************


/*** function openSerialDevFastrak
 *
 * Purpose: Opens the serial device as raw I/O and sets the specified baud
 *          rate
 * In:      device:   Pointer to a string with the device name
 *          baudrate: Pointer to a string with the baudrate
 * Returns: The filedescriptor of the opened device or -1 if it failed
 */

int openSerialDevFastrak(const char *device, const char *baudrate)
{
    int fd, speed;
    long baud;
    struct termios status;

    //--- open device
    fd=open(device, O_RDWR);
    if (fd==-1)
    {
        fprintf(stderr,"Can't open device %s\n", device);
        return -1;
    }

    //--- baud rate constants

    baud=atol(baudrate);
    switch (baud)
    {
        case 300:    speed=B300;    break;
        case 1200:   speed=B1200;   break;
        case 2400:   speed=B2400;   break;
        case 4800:   speed=B4800;   break;
        case 9600:   speed=B9600;   break;
        case 19200:  speed=B19200;  break;
        case 38400:  speed=B38400;  break;
        case 57600:  speed=B57600;  break;
        case 115200: speed=B115200; break;
        default:     fprintf(stderr,"Unkown baud rate %d\n",baud);        
        return -1;
    }

    //--- configure attributes

    tcgetattr(fd, &status);

    cfmakeraw(&status);             // set RAW mode

    status.c_cflag|=CRTSCTS;

    if (cfsetispeed(&status, speed)==-1)
    {
        fprintf(stderr,"Can't set input speed of device %s\n",device);
        return -1;
    }

    if (cfsetospeed(&status, speed)==-1)
    {
        fprintf(stderr,"Can't set output speed of device %s\n",device);
        return -1;
    }

    if (tcsetattr(fd, TCSANOW, &status)==-1)
    {
        fprintf(stderr,"Can't set attributes of device %s\n", device);
        perror("Trackerserver");
        return -1;
    }
    iFlush(fd);

    return fd;
}


/*** function openSerialToFastrak
 *
 * Purpose: Opens the serial interface for the tracker with 115200 baud
 * In:	    sio:  Name of the serial device (e.g. /dev/cua0)
 *          name: Name of the device (e.g. 3d-Mouse)
 *          baud: Baudrate for the serial interface
 * Returns: Filedescriptor of the serial device
 */

int openSerialToFastrak(char *sio, const char *name, const char *baud)
{
    int fd;

    printf("Initialising serial interface %s to %s.\n",sio,name);
    fd=openSerialDevFastrak(sio,baud);
    if (fd==-1)
    {
        perror("Error opening tracker!\n");
    	exit(1);
    }
    return fd;
}


/*** function writeStrToSerial
 *
 * Purpose: Used to write data to the serial interface
 * In:      fd: Filedescriptor of the station to be written to
 *          c:  Pointer to a string of characters to be written
 */

void writeStrToSerial(int fd, const char *c)
{
    while (*c)
    {
        write(fd, c++, 1);
        tcdrain(fd);
        osSleep(10);
    }
}


/*** function nonBlockGetChar
 *
 * Purpose: Reads a byte non blocked from the tracker and the serial mouse
 * In:	    fdTracker: Filedescriptor of the serial interface to the tracker
 *          fdMouse:   Filedescriptor of the serial interface to the mouse
 *          waitime:   Waits 'waitime' seconds before timeout 
 * In/Out:  c:         The byte read is being stored here
 *          buttons:   Storage for the buttonbytes from the serial mouse
 * Returns: -1 if timeout or error occured
 *           0 else
 */

int nonBlockGetChar(int fdTracker, int fdMouse, int waitime, char *c,
                    char *buttons)
{
    char m;
    fd_set rfds;
    struct timeval tv;
    int retval;
    int fdmax=(fdMouse>fdTracker?fdMouse:fdTracker)+1;

    // Watch file "fd" to see when it has input
    FD_ZERO(&rfds);
    FD_SET(fdMouse, &rfds); 
    FD_SET(fdTracker, &rfds);

    // Wait up to "waitime" seconds
    tv.tv_sec = waitime;
    tv.tv_usec = 0;
 
    while (1)
    {                          // loop while interrupted by signals or buttons
        tv.tv_sec = waitime;                   // Wait up to "waitime" seconds
        tv.tv_usec = 0;

        FD_SET(fdMouse, &rfds); 
        FD_SET(fdTracker, &rfds);
        retval = select(fdmax, &rfds, NULL, NULL, &tv);

               // assuming this is linux, tv should contain the remaining time
        if (retval==0)
        {
	    return -1;                                            // timed out
        }
        else if (retval>0)
        {
	    if ((FD_ISSET(fdMouse, &rfds)))
            {
                if (read(fdMouse, &m, 1)==-1)	             // get mouse data
                {
                    perror("read mouse");
                    exit(1);
                }
                buttons[m>>6]=((~m)&0x3f);
            }
            if (FD_ISSET(fdTracker, &rfds))
            {
                return (read(fdTracker, c, 1));	        // return tracker data
            }
        }
        else if (errno!=EINTR)
        {
            perror("nonBlockGetChar");                      // any other error
            exit(-1);
        } // loop if interupted
    }
}
