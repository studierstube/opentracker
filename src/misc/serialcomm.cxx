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
/** source file for serial communication.
  *
  * @author Thomas Peterseil
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/misc/serialcomm.cxx,v 1.2 2002/01/18 19:59:06 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "serialcomm.h"

// Windows Implementation

#ifdef WIN32  
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define DEBUGING 1

#if DEBUGING>0
#define DEBUG(msg) printf("%s\n", (msg));
#else 
#define DEBUG(msg) 
#endif

void initSerialParams(SerialParams *params)
{
    *params->pathname = (char) 0;
    params->baudrate = 1200;
    params->blocking = 1;
    params->bits = 8;
    params->parity = 0;
    params->sbit = 1;
    params->hwflow = 1;
    params->swflow = 0;
}

int openSerialPort(SerialPort *port, SerialParams *params)
{
    COMMTIMEOUTS timeout;
    DCB dcb;
    DWORD baud;

    port->handle = CreateFile(params->pathname,
        GENERIC_READ | GENERIC_WRITE,
        0, 0, OPEN_EXISTING, 0, 0);

    if (port->handle == INVALID_HANDLE_VALUE)
    {
        DEBUG("Error: CreateFile failed")
        return -1;
    }

    /* nonblocking not implemented yet
    if (!params->blocking) 
    {    
    }
                                      */

    strcpy(port->pathname, params->pathname);

    if (!GetCommTimeouts(port->handle, &timeout))
    {
        DEBUG("Error: GetCommTimeouts failed")
        return -1;
    }

    /* all timeouts in millisec */
    timeout.ReadIntervalTimeout = MAXDWORD;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.ReadTotalTimeoutConstant = 0;
    timeout.WriteTotalTimeoutMultiplier = 10;
    timeout.WriteTotalTimeoutConstant = 50;

    if (!SetCommTimeouts(port->handle, &timeout))
    {
        DEBUG("Error: SetCommTimeouts failed")
        return -1;
    }

    FillMemory(&dcb, sizeof(dcb), 0);

    if (!GetCommState(port->handle, &dcb))
    {
        DEBUG("ERROR: GetCommState failed")
        return -1;
    }

    dcb.DCBlength = sizeof(dcb);

    switch (params->baudrate)
    {
        case 300: baud = CBR_300; break;
        case 600: baud = CBR_600; break;
        case 1200: baud = CBR_1200; break;
        case 2400: baud = CBR_2400; break;
        case 4800: baud = CBR_4800; break;
        case 9600: baud = CBR_9600; break;
        case 19200: baud = CBR_19200; break;
        case 38400: baud = CBR_38400; break;
        case 57600: baud = CBR_57600; break;
        case 115200: baud = CBR_115200; break;
        default:
            DEBUG("Error: no such bitrate")
            return -1;
    }

    dcb.BaudRate = baud;

    if ((params->bits >= 5) && (params->bits <= 8))
    {
        dcb.ByteSize = params->bits;
    } else 
    {
        DEBUG("Error: only 5-8 databits")
        return -1;
    }

    switch (params->parity)
    {
        case 0:
            dcb.fParity = FALSE;
            dcb.Parity = NOPARITY;
            break;
        case 1:
            dcb.fParity = TRUE;
            dcb.Parity = ODDPARITY;
            break;
        case 2:
            dcb.fParity = TRUE;
            dcb.Parity = EVENPARITY;
            break;
        default:
            DEBUG("Error: parity 0 for none 1 for odd or 2 for even")
            return -1;
    }

    switch (params->sbit)
    {
        case 1:
            dcb.StopBits = ONESTOPBIT;
            break;
        case 2:
            dcb.StopBits = TWOSTOPBITS;
            break;
        default:
            DEBUG("Error: 1 or 2 stop bits")
            return -1;
    }

    /* not really sure how to set */ 
    if (params->hwflow)
    {
        dcb.fOutxCtsFlow = TRUE;
        dcb.fOutxDsrFlow = TRUE;
        dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
        dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    } else
    {
        dcb.fOutxCtsFlow = FALSE;
        dcb.fOutxDsrFlow = FALSE;
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
    }

    if (params->swflow)
    {
        dcb.fInX = TRUE;
        dcb.fOutX = TRUE;
    } else
    {
        dcb.fInX = FALSE;
        dcb.fOutX = FALSE;
    }

    dcb.fBinary = TRUE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
    dcb.fAbortOnError = FALSE;

    if (!PurgeComm(port->handle, PURGE_TXABORT | PURGE_RXABORT |
        PURGE_TXABORT | PURGE_RXCLEAR))
    {
        DEBUG("Error: PurgeComm failed")
        return -1;
    }      

    if (!SetCommState(port->handle, &dcb))
    {
        DEBUG("Error: SetCommState failed")
        return -1;
    }      

    return 0;
}

int closeSerialPort(SerialPort *port)
{
    if (!CloseHandle(port->handle))
    {
        DEBUG("Error: CloseHandle failed")
        return -1;
    }
    return 0;
}


int setRTSSerialPort(SerialPort *port, int level)
{
    DCB dcb;
	
    if (!GetCommState(port->handle, &dcb))
    {
        DEBUG("ERROR: GetCommState failed")
        return -1;
    }

	if (level)
	{	
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
    } else {
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
    }

    if (!SetCommState(port->handle, &dcb))
    {
        DEBUG("Error: SetCommState failed")
        return -1;
    }      
	
    return 0;
}

int waitforoneSerialPort(SerialPort *port, long time)
{
	DWORD available = 0;
    DWORD eventmask;
    int i;

    if (PeekNamedPipe(port->handle, NULL,
		0, NULL, &available, NULL) && available)
	{
		return (0);
	}

/* following dosn't allow a timeout

    if (!SetCommMask(port->handle, EV_RXCHAR))
	{
        DEBUG("Error: SetCommMask failed")
		return -1;
	}

	if (WaitCommEvent(port->handle, &eventmask, NULL))
	{
        return 0;
	}
*/
/* this is also a very bad solution
   maybe it should be done with overlapped */

    for (i = 0; i < (time / 10); i++)
	{
        Sleep(10);
        if (PeekNamedPipe(port->handle, NULL,
	    	0, NULL, &available, NULL) && available)
		{
	    	return (0);
		}
	}

    return(-1);
}
/*
int waitforallSerialPorts(SerialPort **ports, int count, 
    int *setofports, long time)
{
    fd_set rfds;
    struct timeval timeout;
    int i;
    int maxfd = 0;

    *setofports = 0;
    FD_ZERO(&rfds);
    for (i = 0; i < count; i++)
    {
        FD_SET(ports[i]->fd, &rfds);
        if (maxfd < ports[i]->fd) { maxfd = ports[i]->fd; }
    }

    timeout.tv_sec = time / 1000;
    timeout.tv_usec = (time % 1000)*1000;

    if (select(maxfd + 1, &rfds, NULL, NULL, &timeout) > 0)
    {
        for (i = 0; i < count; i++)
        {
            if (FD_ISSET(ports[i]->fd, &rfds))
            {
               *setofports |= 1 << i;
            }
        }
        return(0);
    }
    return(-1);
}
*/


int readfromSerialPort(SerialPort *port, char *buf, int count)
{
    DWORD numread;
    if (!ReadFile(port->handle, buf, count, &numread, NULL))
    {
        return -1;
    } else {
        return numread;
    }
}

int writetoSerialPort(SerialPort *port, char *buf, int count)
{
    DWORD numwritten;

    if (!WriteFile(port->handle, buf, count, &numwritten, 0))
	{
		return -1;
	} else {
		return numwritten;
	}
}

#else  // UNIX implementation

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define DEBUGING 1

#if DEBUGING>0
#define DEBUG(msg) printf("%s\n", (msg));
#else 
#define DEBUG(msg) 
#endif

void initSerialParams(SerialParams *params)
{
    *params->pathname = (char) 0;
    params->baudrate = 1200;
    params->blocking = 1;
    params->bits = 8;
    params->parity = 0;
    params->sbit = 1;
    params->hwflow = 1;
    params->swflow = 0;
}

int openSerialPort(SerialPort *port, SerialParams *params)
{
    struct termios portinfo;
    int n;
    speed_t baud;

    port->fd = open(params->pathname, 
        O_RDWR | O_NOCTTY | O_NONBLOCK);  
  
       /* need nonblocking open cause sometimes the device may be in
        * canonical mode.
        * O_NDELAY (System V) vs O_NONBLOCK (POSIX.1)
        * on linux the same, irix there is a different value of the
        * macros but i think the same behavior. */

    if (port->fd == -1)
    {
        DEBUG("Error: sorry open failed")
        return -1;
    }

    if (params->blocking)
    {    
        n = fcntl(port->fd, F_GETFL, 0);
        fcntl(port->fd, F_SETFL, n & ~O_NONBLOCK);   /* disabling nonblocking */
    }

    strcpy(port->pathname, params->pathname);

    bzero(&portinfo, sizeof(portinfo));

    switch (params->baudrate)
    {
        case 300: baud = B300; break;
        case 600: baud = B600; break;
        case 1200: baud = B1200; break;
        case 2400: baud = B2400; break;
        case 4800: baud = B4800; break;
        case 9600: baud = B9600; break;
        case 19200: baud = B19200; break;
        case 38400: baud = B38400; break;
        case 57600: baud = B57600; break;
        case 115200: baud = B115200; break;
        default:
            DEBUG("Error: no such bitrate")
            return -1;
    }
    cfsetispeed(&portinfo, baud);
    cfsetospeed(&portinfo, baud);

    switch (params->bits)
    {
        case 5: portinfo.c_cflag |= CS5; break;
        case 6: portinfo.c_cflag |= CS6; break;
        case 7: portinfo.c_cflag |= CS7; break;
        case 8: portinfo.c_cflag |= CS8; break;
        default:
            DEBUG("Error: only 5-8 databits")
            return -1;
    }

    switch (params->parity)
    {
        case 0: break;
        case 1:
            portinfo.c_cflag |= PARENB | PARODD; 
            portinfo.c_iflag |= IGNPAR | INPCK;
            break;
        case 2:
            portinfo.c_cflag |= PARENB;
            portinfo.c_iflag |= IGNPAR | INPCK;
            break;
        default:
            DEBUG("Error: parity 0 for none 1 for odd or 2 for even")
    }

    switch (params->sbit)
    {
        case 1: break;
        case 2: portinfo.c_cflag |= CSTOPB; break;
        default:
            DEBUG("Error: 1 or 2 stop bits")
            return -1;
    }

    if (params->hwflow)
    {
#if defined CRTSCTS
        portinfo.c_cflag |= CRTSCTS;
#else
#if defined CNEW_RTSCTS
        portinfo.c_cflag |= CNEW_RTSCTS;  /* RiscOS API compliance. */
#endif
#endif

    }

    if (params->swflow)
    {
        portinfo.c_iflag |= IXON | IXOFF;
    }


    portinfo.c_iflag |= IGNBRK | IXANY;
    portinfo.c_cflag |= CLOCAL | CREAD;

    portinfo.c_oflag = 0;
    portinfo.c_lflag = 0;
    portinfo.c_cc[VTIME] = 0;   
    portinfo.c_cc[VMIN] = 1; 

    if (tcflush(port->fd, TCIFLUSH) == -1)
    {
        DEBUG("Error: tcflush TCIFLUSH failed")
        return -1;
    }      

    if (tcflush(port->fd, TCOFLUSH) == -1)
    {
        DEBUG("Error: tcflush TCOFLUSH failed")
        return -1;
    }      

    if (tcsetattr(port->fd, TCSANOW, &portinfo) == -1)
    {
        DEBUG("Error: tcsetattr failed")
        return -1;
    }      

    return 0;
}

int closeSerialPort(SerialPort *port)
{
    close(port->fd);
    return 0;
}

int setRTSSerialPort(SerialPort *port, int level)
{
    int n;

    if (ioctl(port->fd,TIOCMGET,&n)==-1)
    {
        DEBUG("Can't read modem lines\n");
        return -1;
    }
    if (level) {
        n |= TIOCM_RTS;
    } else {
        n &= ~TIOCM_RTS;
    }
    if (ioctl(port->fd,TIOCMSET,&n)==-1)
    {
        DEBUG("Can't set modem lines\n");
        return -1;
    }

    return 0;
}

int waitforoneSerialPort(SerialPort *port, long time)
{
    fd_set rfds;
    struct timeval timeout;

    FD_ZERO(&rfds);
    FD_SET(port->fd, &rfds);

    timeout.tv_sec = time / 1000;
    timeout.tv_usec = (time % 1000)*1000;

    if (select(port->fd + 1, &rfds, NULL, NULL, &timeout) > 0)
    {
        if (FD_ISSET(port->fd, &rfds))
        {
            return(0);
        }
    }
    return(-1);
}

int waitforallSerialPorts(SerialPort **ports, int count, 
    int *setofports, long time)
{
    fd_set rfds;
    struct timeval timeout;
    int i;
    int maxfd = 0;

    *setofports = 0;
    FD_ZERO(&rfds);
    for (i = 0; i < count; i++)
    {
        FD_SET(ports[i]->fd, &rfds);
        if (maxfd < ports[i]->fd) { maxfd = ports[i]->fd; }
    }

    timeout.tv_sec = time / 1000;
    timeout.tv_usec = (time % 1000)*1000;

    if (select(maxfd + 1, &rfds, NULL, NULL, &timeout) > 0)
    {
        for (i = 0; i < count; i++)
        {
            if (FD_ISSET(ports[i]->fd, &rfds))
            {
               *setofports |= 1 << i;
            }
        }
        return(0);
    }
    return(-1);
}

int readfromSerialPort(SerialPort *port, char *buf, int count)
{
    return read(port->fd, buf, count);
}

int writetoSerialPort(SerialPort *port, char *buf, int count)
{
    return write(port->fd, buf, count);
}

#endif
