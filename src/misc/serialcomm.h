 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for serial communication.
  *
  * @author Thomas Peterseil
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/serialcomm.h,v 1.2 2001/11/23 10:03:54 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/* error code : fehler rückgaben kleiner 0, codiern fehler */

#ifndef _SERIAL_H
#define _SERIAL_H

#ifdef WIN32
#include <Windows.h>
#endif

/** describes the parameter for a serial port. used to open a port with
 * specific parameters */
typedef struct
{
    /// port to open
    char pathname[256];
    /// bits per second
    int baudrate;                  
    /// none, even, odd = 0, 2, 1
    int parity;  
    /// bits of data 5-8   
    int bits;         
    /// stopbits 1-2    
    int sbit;   
    /// yes (!=0) or no (0) 
    int hwflow;           
    /// yes (!=0) or no (0)                  
    int swflow;  
    /// yes (!=0) or no (0)                  
    int blocking;
} SerialParams;

/** describes a port, via its name and handle. This struct encapsulates 
 * the differences between UNIX and Windows handles etc. */
typedef struct
{
    char pathname[256];
#ifndef WIN32
	int fd;
#else
    HANDLE handle;
#endif
} SerialPort;

void initSerialParams(SerialParams *params);

int openSerialPort(SerialPort *port, SerialParams *params);

int closeSerialPort(SerialPort *port);

int setRTSSerialPort(SerialPort *port, int level);

int waitforoneSerialPort(SerialPort *port, long time);

int waitforallSerialPorts(SerialPort **ports, int count, int *setofports, long time);

int readfromSerialPort(SerialPort *port, char *buf, int count);

int writetoSerialPort(SerialPort *port, char *buf, int count);

#endif
