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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for GPS_Handler
  *
  * @author Gerhard Reitmayr
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPS_Handler.h,v 1.4 2003/04/03 16:04:27 tamer Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _GPS_HANDLER_H
#define _GPS_HANDLER_H
 
#include <ace/Connector.h>
#include <ace/TTY_IO.h>
#include <ace/DEV_Connector.h>
#include <ace/Svc_Handler.h>

class GPSDriver;

#define NMEABUFSZ (8 * 1024)

/**
 * This class retrieves data from the GPS receiver via the serial port. It uses
 * the NMEA parsing routines in nmea.cxx to parse incoming data and forward it 
 * to the GPSDriver and an optional attached DGPSIP server.
 * @ingroup input
 * @author Gerhard Reitmayr
 */
#ifdef WIN32
class GPS_Handler : public  ACE_Svc_Handler<ACE_TTY_IO, ACE_TTY_IO::PEER_ADDR, ACE_NULL_SYNCH>
#else
class GPS_Handler : public  ACE_Svc_Handler<ACE_TTY_IO, ACE_NULL_SYNCH>
#endif
{
public:
	/// default constructor for ace framework. Do not use !
	GPS_Handler() {};

	GPS_Handler( GPSDriver * parent_ );
	virtual ~GPS_Handler();
	
	virtual int open( void * factory );
	virtual int handle_input(ACE_HANDLE fd=ACE_INVALID_HANDLE);
	virtual int handle_signal( int, siginfo_t *, ucontext_t * );

protected:
	GPSDriver * parent;
	int nmeaind;
	char nmeabuf[NMEABUFSZ];
};

typedef ACE_Connector<GPS_Handler, ACE_DEV_CONNECTOR> GPS_Connector;

#endif // !defined(_GPS_HANDLER_H)
