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
/** source file for GPSDriver
  *
  * @author Gerhard Reitmayr
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSDriver.cxx,v 1.1 2003/03/27 18:26:02 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "../OpenTracker.h"
#include <ace/Reactor.h>

#include "GPSDriver.h"
#include "GPS_Handler.h"
#include "DGPSIP_Handler.h"

int verbose = 0;

GPSDriver::GPSDriver(ACE_Reactor * reactor_) :
reactor( reactor_ ),
receiver( NULL ),
server( NULL )
{
	if( NULL == reactor )
		reactor = ACE_Reactor::instance();
}

GPSDriver::~GPSDriver()
{
	if( receiver != NULL )
		close();
	listeners.clear();
}

int GPSDriver::open( const std::string & device, int baud, const std::string & serveraddr, int port )
{
	// todo open everything here
	int result;
	// open the serial port the the GPS receiver
	receiver = new GPS_Handler( this );
	GPS_Connector gpsconnect( reactor );
	result = gpsconnect.connect( receiver, ACE_DEV_Addr(device.c_str()));
	if( result == 0)
	{
		// set the appropriate parameters
		ACE_TTY_IO::Serial_Params params;		
		result = receiver->peer().control( ACE_TTY_IO::GETPARAMS, &params);
		params.baudrate = baud;
		params.databits = 8;
		params.stopbits = 1;
		params.parityenb = 0;
		params.rtsenb = 1;
		params.ctsenb = 1;
		result = receiver->peer().control(ACE_TTY_IO::SETPARAMS, &params );			
		if( result != 0 )
		{
			receiver->destroy();
			delete receiver;
			receiver = NULL;
		}
	}
	
	// open the tcp connection to the server, if required
	if( serveraddr.compare("") != 0 )
	{
		server = new DGPSIP_Handler( this );
		DGPSIP_Connector ipconnect( reactor );
		if( ipconnect.connect( server, ACE_INET_Addr( port, serveraddr.c_str() )) != 0 )
		{
			server->destroy();
			delete server;
			server = NULL;
		}		
	}
	return result;
}

void GPSDriver::close()
{
	if( server != NULL )
	{
		server->destroy();
		delete server;
		server = NULL;
	}
	if( receiver != NULL )
	{
		receiver->destroy();
		delete receiver;
		receiver = NULL;
	}
}

void GPSDriver::addListener( GPSListener * listener, void * userData )
{
	listeners[listener] = userData;
}

void GPSDriver::removeListener( GPSListener * listener, void * userData )
{
	std::map<GPSListener *, void *>::iterator it = listeners.find( listener );
	if( it != listeners.end())
		listeners.erase( listener );
}

void GPSDriver::new_point( const GPSListener::GPSPoint & point )
{
	std::map<GPSListener *, void *>::iterator it;
	for( it = listeners.begin(); it != listeners.end(); it++ )
		(*it).first->newPoint( point, (*it).second );
}

void GPSDriver::send_dgpsip( const char * buffer, const int len )
{
	if( NULL != server )
		server->peer().send_n( buffer, len );
}

void GPSDriver::send_rtcm( const char * buffer, const int len )
{
	if( NULL != receiver )
		receiver->peer().send_n( buffer, len );
}

void GPSDriver::setDebug( bool debug )
{
	debugOn = debug;
	verbose = (debugOn) ? (10) : (0);
}
	