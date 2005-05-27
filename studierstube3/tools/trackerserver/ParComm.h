// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       ParComm
//  TYPE:       h header file
//  PROJECT:    Studierstube
//  CONTENT:    Definition of parallelcommunication functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  01-mar-00 12:28:12  rr      last modification
// ===========================================================================

#ifndef _PARCOMM_H_
#define _PARCOMM_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*** function outPort
 *
 * Purpose: Writes data to parallel interface
 * In:      fd:         Filedescriptor of the parallel interface
 *          data:       Data to be output on the parallel interface
 *          which_port: Address of BasePort
 */
void outPort(int fd, unsigned char what, off_t which_port);

/*** function inPort
 *
 * Purpose: Reads data from the parallel interface
 * In:      fd:         Filedescriptor of the parallel interface
 *          which_port: Address of the BasePort
 * Returns: one byte of data being read
 */
int inPort(int fd, off_t which_port);

/*** function openParallelPort
 *
 * Purpose: Opens and initializes the parallel interface
 * In:      devName:  Name of the parallel device to be opened
 *          basePort: Baseport of the parallel device to be opened
 * Returns: Filedescriptor of the parallel interface
 */
int openParallelPort(char* devName, short int basePort);

#endif
