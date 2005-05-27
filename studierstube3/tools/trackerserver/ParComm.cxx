// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       ParComm
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of parallelcommunication functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  01-mar-00 12:20:12  rr      last modification
// ===========================================================================

#include "ParComm.h"

/*** function outPort
 *
 * Purpose: Writes data to parallel interface
 * In:      fd:         Filedescriptor of the parallel interface
 *          data:       Data to be output on the parallel interface
 *          which_port: Address of BasePort
 */

void outPort(int fd, unsigned char data, off_t which_port)
{
    lseek(fd, which_port, SEEK_SET);
    write(fd, &data, 1);
}


/*** function inPort
 *
 * Purpose: Reads data from the parallel interface
 * In:      fd:         Filedescriptor of the parallel interface
 *          which_port: Address of the BasePort
 * Returns: one byte of data being read
 */

int inPort(int fd, off_t which_port)
{
    unsigned char data;
        
    lseek(fd, which_port, SEEK_SET);
    read(fd, &data, 1);

    return data;
}


/*** function openParallelPort
 *
 * Purpose: Opens and initializes the parallel interface
 * In:      devName:  Name of the parallel device to be opened
 *          basePort: Baseport of the parallel device to be opened
 * Returns: Filedescriptor of the parallel interface
 */

int openParallelPort(char* devName, short int basePort)
{
    int fdport;

    fdport=open(devName, O_RDWR);
    if (fdport==-1)
    {
        fprintf(stderr,"Can't open device '%s'.\n",devName);
        exit(1);
    }

    /* Set LPT to byte input mode */
    outPort(fdport, 0x20, basePort+0x402);
    outPort(fdport, 0x00, basePort);
    outPort(fdport, 0x20, basePort+2);

    return(fdport);
}
