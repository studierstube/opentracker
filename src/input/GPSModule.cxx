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
/** source file for GPSModule
  *
  * @author Gerhard Reitmayr
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSModule.cxx,v 1.5 2003/04/08 18:59:59 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/Reactor.h>

#include "GPSModule.h"
#include "GPSSource.h"

ACE_Reactor gps_reactor;

GPSModule::GPSModule() :
driver( NULL ),
source( NULL )
{
}

GPSModule::~GPSModule()
{
}

void GPSModule::init(StringTable& attributes,  ConfigNode * localTree)
{
	device = attributes.get("dev");
	dgpsServer = attributes.get("DGPSserver");
	if( attributes.get( "baudrate", &baudRate ) != 1 )
	{
		baudRate = 9600;		// default GPS receiver baud rate
	}
	if( attributes.get( "DGPSport", &dgpsPort ) != 1 )
	{
		dgpsPort = 2101;		// default DGPSIP port
	}
	if( attributes.get("debug").compare("on") == 0 )
	{
		debug = true;
	}
	else
	{
		debug = false;
	}
    if( attributes.get("dgpsmirror", &dgpsmirror) != 1 )
    {
        dgpsmirror = -1;
    }
    Module::init( attributes, localTree );
	cout << "GPSModule initialized for port " << device << " and server " << dgpsServer << endl;
}

Node * GPSModule::createNode( const std::string & name, StringTable & attributes )
{
	if( name.compare("GPSSource") == 0 )
	{
		if( source != NULL )
		{
			cout << "Only one GPSSource can be build !" << endl;
			return NULL;
		}
		if( !isInitialized() )
		{
			cout << "GPSModule is not initialized, cannot build GPSSource !" << endl;
			return NULL;
		}
		source = new GPSSource;
		cout << "Built GPSSource node." << endl;
		return source;
	}
	return NULL;	
}

void GPSModule::pushState()
{
	if( source != NULL )
	{
		lock();
		if( changed )
		{
			source->state = buffer;
			changed = false;
			unlock();
			source->updateObservers( source->state );
		}
		else
			unlock();
	}
}

void GPSModule::start()
{
	if( isInitialized() && source != NULL )
	{
		changed = false;
		ThreadModule::start();
	}
}

void GPSModule::close()
{
	if( driver != NULL )
	{
		if( driver->getReactor() != NULL )
		{
			driver->getReactor()->end_reactor_event_loop();
		}
		ThreadModule::close();
	}
	
}

void GPSModule::run()
{
	driver = new GPSDriver( &gps_reactor );
	driver->setDebug( debug ); // only for debug purposes ...
	driver->addListener( this );
	driver->getReactor()->owner(ACE_Thread::self());
	if( driver->open( device, baudRate, dgpsServer, dgpsPort, dgpsmirror ) != 0 )
	{
		cout << "GPSModule could not start GPSDriver !\n";
		return;
	}
	driver->getReactor()->run_reactor_event_loop();
	driver->close();
	delete driver;
}

void GPSModule::newPoint( const GPSPoint & point, void * userData )
{
	lock();
	buffer.timeStamp();
	buffer.position[0] = point.lat * MathUtils::GradToRad;
	buffer.position[1] = point.lon * MathUtils::GradToRad;
	buffer.position[2] = point.height;
	buffer.confidence = 1 / point.hdop;
	changed = true;
	unlock();
}
