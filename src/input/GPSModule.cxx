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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSModule.cxx,v 1.7 2003/07/02 07:28:37 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/Reactor.h>
#include <ace/FILE_Connector.h>

#include "GPSModule.h"
#include "GPSSource.h"
#include "GPSDirectionSource.h"

ACE_Reactor gps_reactor;

GPSModule::GPSModule() :
driver( NULL ),
dirSource( NULL ),
logFile( NULL ),
source( NULL )
{
}

GPSModule::~GPSModule()
{
    if( source != NULL )
        delete source;
    if( dirSource != NULL )
        delete dirSource;
    if( logFile != NULL )
        delete logFile;
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
    if( attributes.containsKey("logfile") )
    {
        logFile = new ACE_FILE_IO;
        ACE_FILE_Connector connector; 
        if( connector.connect( *logFile, ACE_FILE_Addr( attributes.get("logfile").c_str() ), 0, ACE_Addr::sap_any, 0, O_WRONLY|O_CREAT|O_APPEND))
        {
            delete logFile;
            logFile = NULL;
            cout << "DGPSModule error opening log file " << attributes.get("logfile") << endl;
        }
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
    if( name.compare("GPSDirectionSource") == 0 )
    {
        if( dirSource != NULL )
        {
            cout << "Only one GPSDirectionSource can be build !" << endl;
            return NULL;
        }
        if( !isInitialized() )
        {
            cout << "GPSModule is not initialized, cannot build GPSDirectionSource !" << endl;
            return NULL;
        }
        dirSource = new GPSDirectionSource;
        cout << "Built GPSDirectionSource node." << endl;
        return dirSource;
    }

	return NULL;	
}

void GPSModule::pushState()
{
	if( source != NULL )
	{
		lock();
		if( source->state.time < source->buffer.time )
		{
			source->state = source->buffer;
			unlock();
			source->updateObservers( source->state );
		}
		else
			unlock();
    }
    if( dirSource != NULL )
    {
        lock();
        if( dirSource->state.time < dirSource->buffer.time )
        {
            dirSource->state = dirSource->buffer;
            unlock();
            dirSource->updateObservers( dirSource->state );
        }
        else
			unlock();
	}
}

void GPSModule::start()
{
	if( isInitialized() && source != NULL && dirSource != NULL )
	{
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
    if( logFile != NULL )
        logFile->close();
}

void GPSModule::run()
{
	driver = new GPSDriver( &gps_reactor );
	driver->setDebug( debug ); // only for debug purposes ...
    if( source != NULL )
        driver->addListener( source, this );
    if( dirSource != NULL )
        driver->addListener( dirSource, this );
    if( logFile != NULL )
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

void GPSModule::newData( const GPResult * res, const char * line, void * userData )
{
    logFile->send( line, ACE_OS::strlen(line));
}
