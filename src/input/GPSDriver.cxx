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
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

#define ACE_NLOGGING

#include "../OpenTracker.h"
#include <ace/Reactor.h>
#include <iostream>
#include <algorithm>
#include <memory>

using namespace std;

#include "GPSDriver.h"
#include "GPS_Handler.h"
#include "DGPSIP_Handler.h"
#include "DGPSMirror_Handler.h"

GPSDriver::GPSDriver(ACE_Reactor * reactor_) :
reactor( reactor_ ),
receiver( NULL ),
server( NULL ),
acceptor( NULL ),
rtcmdev( NULL ),
fix( false )
{
	if( NULL == reactor )
		reactor = ACE_Reactor::instance();
}

GPSDriver::~GPSDriver()
{
	if( receiver != NULL )
		close();
	listeners.clear();
    std::vector<DGPSMirror_Handler *>::reverse_iterator it = clients.rbegin();
    while( it != clients.rend())
    {
        (*it)->destroy();
        it++;
    }
    clients.clear();
}

int GPSDriver::open( const std::string & device, int baud, const std::string & serveraddr, int port, int dgpsmirror, const std::string & rtcmdev )
{
    if( getDebug())
        std::cout << "GPSDriver open\n";

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
		params.ctsenb = 0;
        params.rcvenb = 1;
        params.rtsenb = 1;
        params.dsrenb = 0;
		result = receiver->peer().control(ACE_TTY_IO::SETPARAMS, &params );
		if( result != 0 )
		{
			receiver = NULL;
			std::cerr << "GPSDriver could not open serial port " << device << " !\n";
		}
	}
    if( getDebug())
        std::cout << "GPSDriver opened serial port " << device << endl;
	
	// open the tcp connection to the server, if required
	if( result == 0 && serveraddr.compare("") != 0 )
	{
		server = new DGPSIP_Handler( this );
		DGPSIP_Connector ipconnect( reactor );
		if( ipconnect.connect( server, ACE_INET_Addr( port, serveraddr.c_str() )) != 0 )
		{
			server = NULL;
			std::cerr << "GPSDriver could not open connection to DGPS server " << serveraddr << ":" << port << " !\n";
		}
        if( getDebug())
            std::cout << "GPSDriver opened connection to " << serveraddr << endl;
	}

    // open a mirror if we have a DGPS handler
    if( server != NULL && dgpsmirror != -1)
    {
        acceptor = new DGPSMirror_Acceptor( this );
        if( acceptor->open( ACE_INET_Addr( dgpsmirror) , reactor ) != 0 )
        {
            delete acceptor;
            acceptor = NULL;
            std::cerr << "GPSDriver could not open DGPS mirror server on port " << dgpsmirror << " !\n";
        }
        if( getDebug())
            std::cout << "GPSDriver opened mirror listener.\n";
    }

    // open a dedicated serial port for RTCM output, if we have one
    // it assumes the same baud rate as the serial port
    if( rtcmdev.compare("") != 0)
    {
        this->rtcmdev = new ACE_TTY_IO;
        ACE_DEV_Connector connector( *this->rtcmdev, ACE_DEV_Addr(rtcmdev.c_str()));
        // set the appropriate parameters
        ACE_TTY_IO::Serial_Params params;		
        result = this->rtcmdev->control( ACE_TTY_IO::GETPARAMS, &params);
        params.baudrate = baud;
        params.databits = 8;
        params.stopbits = 1;
        params.parityenb = 0;
        params.ctsenb = 0;
        params.rcvenb = 1;
        params.rtsenb = 1;
        params.dsrenb = 0;
        result = this->rtcmdev->control(ACE_TTY_IO::SETPARAMS, &params );			
        if( result != 0 )
        {
            delete this->rtcmdev;
            this->rtcmdev = NULL;
            std::cerr << "GPSDriver could not open serial port " << rtcmdev << " !\n";
        }
        if( getDebug())
            std::cout << "GPSDriver opened serial port " << rtcmdev << endl;
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
    if( rtcmdev != NULL )
    {
        delete rtcmdev;
        rtcmdev = NULL;
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
    if( std::find( clients.begin(), clients.end(), client) == clients.end())
        clients.push_back( client );
}

void GPSDriver::removeClient( DGPSMirror_Handler * client )
{
    std::vector<DGPSMirror_Handler *>::iterator it = std::find( clients.begin(), clients.end(), client);
    if( it != clients.end())
        clients.erase( it );
}

void GPSDriver::new_line( const char * line )
{
    auto_ptr<GPResult> result((GPResult *)GPSParser::parse(line));
    // for some messages we use some data on the side 
    if( result->type == GPResult::GPGGA )
    {
        fix = (((GPGGA *)result.get())->fix > 0);
        hdop = ((GPGGA *)result.get())->hdop;
        numsat = ((GPGGA *)result.get())->numsats;
    }
    std::map<GPSListener *, void *>::iterator it;
    for( it = listeners.begin(); it != listeners.end(); it++ )
        (*it).first->newData( result.get(), line, (*it).second );
}

void GPSDriver::send_rtcm( const char * buffer, const int len )
{
    if( NULL != this->rtcmdev )
    {
        ACE_DEBUG((LM_DEBUG, "GPSDriver::send_rtcm send to device\n"));
        rtcmdev->send_n( buffer, len );
    }
	else if( NULL != receiver )
    {
        ACE_DEBUG((LM_DEBUG, "GPSDriver::send_rtcm send to receiver\n"));
		receiver->peer().send_n( buffer, len );
    }
    std::vector<DGPSMirror_Handler *>::iterator it;
    for( it = clients.begin(); it != clients.end(); it ++ )
    {
        ACE_DEBUG((LM_DEBUG, "GPSDriver::send_rtcm send to peer\n"));
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
}
