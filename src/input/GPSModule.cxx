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
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/OS.h>
#include <ace/Reactor.h>
#include <ace/Thread.h>
#include <ace/FILE_Connector.h>
#include <ace/FILE_IO.h>

#include "GPSModule.h"
#include "GPSSource.h"
#include "GPSDirectionSource.h"
#include "GPSGarminCompass.h"
#include "GPSGarminAltitude.h"
#include "GPSInfoSource.h"

#include <iostream>

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

//using namespace std;


namespace ot {


ACE_Reactor gps_reactor;

GPSModule::GPSModule() :
source( NULL ),
dirSource( NULL ),
compassSource( NULL ),
altitudeSource( NULL ),
infoSource( NULL ),
driver( NULL ),
logFile( NULL )
{
}

GPSModule::~GPSModule()
{
    if( source != NULL )
        delete source;
    if( dirSource != NULL )
        delete dirSource;
    if( compassSource != NULL )
        delete compassSource;
    if( altitudeSource != NULL )
        delete altitudeSource;
    if( infoSource != NULL )
        delete infoSource;
    if( logFile != NULL )
        delete logFile;
}

void GPSModule::init(StringTable& attributes,  ConfigNode * localTree)
{
	device = attributes.get("dev");
	dgpsServer = attributes.get("DGPSserver");
    rtcmdev = attributes.get("rtcmdev");
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
    if( attributes.get("DGPSmirror", &dgpsmirror) != 1 )
    {
        dgpsmirror = -1;
    }
    if( attributes.containsKey("logfile") )
    {
        logFile = new ACE_FILE_IO;
        ACE_FILE_Connector connector; 
        if( connector.connect( *logFile, ACE_FILE_Addr(ACE_TEXT_CHAR_TO_TCHAR(attributes.get("logfile").c_str())), 0, ACE_Addr::sap_any, 0, O_WRONLY|O_CREAT|O_APPEND))
        {
            delete logFile;
            logFile = NULL;
			LOG_ACE_ERROR("ot:DGPSModule error opening log file %s\n", attributes.get("logfile").c_str());
        }
    }
    Module::init( attributes, localTree );
	LOG_ACE_INFO("ot:GPSModule initialized for port %s and server %s\n", device.c_str(), dgpsServer.c_str());
}

Node * GPSModule::createNode( const std::string & name, StringTable & attributes )
{
	if( name.compare("GPSSource") == 0 )
	{
		if( source != NULL )
		{
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSSource can be build !\n")));
			return NULL;
		}
		if( !isInitialized() )
		{
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSSource !\n")));
			return NULL;
		}
		source = new GPSSource;
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built GPSSource node.\n")));
		return source;
	}
    if( name.compare("GPSDirectionSource") == 0 )
    {
        if( dirSource != NULL )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSDirectionSource can be build !\n")));
            return NULL;
        }
        if( !isInitialized() )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSDirectionSource !\n")));
            return NULL;
        }
        dirSource = new GPSDirectionSource;
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built GPSDirectionSource node.\n")));
        return dirSource;
    }
    if( name.compare("GPSGarminCompass") == 0 )
    {
        if( compassSource != NULL )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSGarminCompass can be build !\n")));
            return NULL;
        }
        if( !isInitialized() )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSGarminCompass !\n")));
            return NULL;
        }
        compassSource = new GPSGarminCompass;
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built GPSGarminCompass node.\n")));
        return compassSource;
    }
    if( name.compare("GPSGarminAltitude") == 0 )
    {
        if( altitudeSource != NULL )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSGarminAltitude can be build !\n")));
            return NULL;
        }
        if( !isInitialized() )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSGarminAltitude !\n")));
            return NULL;
        }
        altitudeSource = new GPSGarminAltitude;
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built GPSGarminAltitude node.\n")));
        return altitudeSource;
    }
    if( name.compare("GPSInfoSource") == 0 )
    {
        if( infoSource != NULL )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Only one GPSInfoSource can be build !\n")));
            return NULL;
        }
        if( !isInitialized() )
        {
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule is not initialized, cannot build GPSInfoSource !\n")));
            return NULL;
        }
        infoSource = new GPSInfoSource;
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built GPSInfoSource node.\n")));
        return infoSource;
    }
	return NULL;	
}

void GPSModule::pushState()
{
    // here for some template magic:
    // updateSource is a template member function but the compiler appearantly
    // creates different variants for each type passed in :)
    updateSource(source);
    updateSource(dirSource);
    updateSource(compassSource);
    updateSource(altitudeSource);
    updateSource(infoSource);
}

void GPSModule::start()
{
	if( isInitialized() && (source != NULL || dirSource != NULL || compassSource != NULL || altitudeSource != NULL || infoSource != NULL ))
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
    if( compassSource != NULL )
        driver->addListener( compassSource, this );
    if( altitudeSource != NULL )
        driver->addListener( altitudeSource, this );
    if( infoSource != NULL )
        driver->addListener( infoSource, this );
    
    if( logFile != NULL )
        driver->addListener( this );
	driver->getReactor()->owner(ACE_Thread::self());
	if( driver->open( device, baudRate, dgpsServer, dgpsPort, dgpsmirror, rtcmdev ) != 0 )
	{
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:GPSModule could not start GPSDriver !\n")));
		return;
	}
    if( debug )
	{
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:GPSModule started GPSDriver !\n")));
	}
	driver->getReactor()->run_reactor_event_loop();
	driver->close();
	delete driver;
}

void GPSModule::newData( const GPResult * res, const char * line, void * userData )
{
    logFile->send( line, ACE_OS::strlen(line));
}


} // namespace ot
