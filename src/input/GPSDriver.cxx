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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSDriver.cxx,v 1.8 2003/04/08 18:59:59 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "../OpenTracker.h"
#include <ace/Reactor.h>
#include <iostream>
#include <algorithm>

#include "GPSDriver.h"
#include "GPS_Handler.h"
#include "DGPSIP_Handler.h"
#include "DGPSMirror_Handler.h"

using namespace std;

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
    vector<DGPSMirror_Handler *>::reverse_iterator it = clients.rbegin();
    while( it != clients.rend())
    {
        (*it)->destroy();
        it++;
    }
    clients.clear();
}

int GPSDriver::open( const std::string & device, int baud, const std::string & serveraddr, int port, int dgpsmirror)
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
			receiver = NULL;
            cerr << "GPSDriver could not open serial port " << device << " !\n";
		}
	}
	
	// open the tcp connection to the server, if required
	if( result == 0 && serveraddr.compare("") != 0 )
	{
		server = new DGPSIP_Handler( this );
		DGPSIP_Connector ipconnect( reactor );
		if( ipconnect.connect( server, ACE_INET_Addr( port, serveraddr.c_str() )) != 0 )
		{
			server = NULL;
            cerr << "GPSDriver could not open connection to DGPS server " << serveraddr << ":" << port << " !\n";
		}		
	}

    // open a mirror if we have a DGPS handler
    if( server != NULL && dgpsmirror != -1)
    {
        acceptor = new DGPSMirror_Acceptor( this );
        if( acceptor->open( ACE_INET_Addr( dgpsmirror) , reactor ) != 0 )
        {
            delete acceptor;
            acceptor = NULL;
            cerr << "GPSDriver could not open DGPS mirror server on port " << dgpsmirror << " !\n";
        }
    }
	return result;
}

void GPSDriver::close()
{
	if( server != NULL )
	{
		server->destroy();
		server = NULL;
	}
	if( receiver != NULL )
	{
		receiver->destroy();
		receiver = NULL;
	}
    if( acceptor != NULL )
    {
        delete acceptor;
        acceptor = NULL;
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

void GPSDriver::addClient( DGPSMirror_Handler * client )
{
    if( find( clients.begin(), clients.end(), client) == clients.end())
        clients.push_back( client );
}

void GPSDriver::removeClient( DGPSMirror_Handler * client )
{
    vector<DGPSMirror_Handler *>::iterator it = find( clients.begin(), clients.end(), client);
    if( it != clients.end())
        clients.erase( it );
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
    vector<DGPSMirror_Handler *>::iterator it;
    for( it = clients.begin(); it != clients.end(); it ++ )
    {
        if( (*it)->peer().send_n( buffer, len ) < 0 )
        {
            (*it)->destroy();
            it--;
        }
    }
}

void GPSDriver::setDebug( bool debug )
{
	debugOn = debug;
	verbose = (debugOn) ? (10) : (0);
}
