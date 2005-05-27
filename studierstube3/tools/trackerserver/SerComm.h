// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SerComm
//  TYPE:       h header file
//  PROJECT:    Studierstube
//  CONTENT:    Definition of serialcommunication functions used in the
//              trackerserver
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rr      Roman Rath
// ===========================================================================
//  HISTORY:
//
//  29-feb-00 13:18:12  rr      last modification
// ===========================================================================


#ifndef _SERCOMM_H_
#define _SERCOMM_H_

#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "GlobalDef.h"
#include <util/os.h>


/*** function iFlush
 *
 * Purpose: Flushes data received but not read from the input buffer of the
 *          given filedescriptor
 * In:      fd: Filedescriptor of the object to be flushed
 */
void iFlush(int fd);

/*** function oFlush
 *
 * Purpose: Flushes data written but not transmitted from the output buffer
 *          of the given filedescriptor
 * In:      fd: Filedescriptor of the object to be flushed
 */
void oFlush(int fd);

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
fd_set waitForChar(StationRecord* station, int fdCount, int waitime);

/*** function waitForCharSingle
 *
 * Purpose: Monitors one serial interface for a specified time until a 
 *          character arrives
 * In:      fd:      Filedescriptor of the serial interconnection to the
 *                   station which should be monitored
 *          waitime: Amount of time before timeout assumed
 * Returns: Descriptor set of the monitored station
 */
fd_set waitForCharSingle(int fd, int waitime);

/*** function writeToSerial
 *
 * Purpose: Writes data to the serial interface
 * In:      fd:  Filedescriptor of the serial interconnection to the
 *               station which the data will be written to
 *          str: Pointer to a string of characters to be written
 *          len: Number of characters to be written
 */
void writeToSerial(int fd, char *str, int len);

/*** function openSerialToFob
 *
 * Purpose: Opens the serial interface(s) to the tracker
 * In:      fdCount: Number of stations in the Flock Of Birds
 *          mode:    fobSingle (opening only one serial interface to the
 *                   master bird) or fobMulti (opening individual serial
 *                   interfaces to each bird)
 * In/Out:  station: Pointer to an array of records holding the names of the
 *                   serial devices and where the filedescriptors of the
 *                   serial connections to the stations will be stored
 */
void openSerialToFob(StationRecord* station, int fdCount, int mode);

/*** function openSerialToFastrak
 *
 * Purpose: Opens the serial interface for the tracker with 115200 baud
 * In:	    sio:  Name of the serial device (e.g. /dev/cua0)
 *          name: Name of the device (e.g. 3d-Mouse)
 *          baud: Baudrate for the serial interface
 * Returns: Filedescriptor of the serial device
 */
int openSerialToFastrak(char *sio, const char *name, const char *baud);

/*** function writeStrToSerial
 *
 * Purpose: Used to write data to the serial interface
 * In:      fd: Filedescriptor of the station to be written to
 *          c:  Pointer to a string of characters to be written
 */
void writeStrToSerial(int fd, const char *c);

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
                    char *buttons);

#endif
