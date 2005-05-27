/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** source file for TrackerOT class.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: TrackerOT.cxx 4087 2004-12-10 15:03:37Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/OS.h>

#include <stbapi/tracker/TrackerOT.h>
#include <stbapi/tracker/StbSink.h>
#include <stbapi/tracker/StbSource.h>

#include <iostream>

using namespace std;

// constructor

TrackerOT::TrackerOT( const char * file ) : 
    isActive( 0 ), context( 0 ), maximum( 0 ), filename( file )
{    
	initializeContext( context );
}

// destructor

TrackerOT::~TrackerOT()
{
	// no real cleanup of either sinks nor sources !!
    sinks.clear();
}

// starts tracker

void TrackerOT::activate()
{
    if( isActive == 1 )
        return;
	context.addModule("StbConfig", *this);
    context.addFactory( *this );
    context.addModule( "StbInputConfig", eventHandler );
    context.addFactory( eventHandler );
    string stbroot;
    char * line = ACE_OS::getenv("STBROOT");
    if(line)
    {
        stbroot = line;
        stbroot += "/bin/workspace";
        context.addDirectoryLast(stbroot);
    }
    context.parseConfiguration( filename );
    if(line)
    {
        context.removeDirectory(stbroot);
    }
    context.start();
    isActive = 1;
}

// closes tracker

void TrackerOT::deactivate()
{
    if( isActive == 0 )
        return;
    isActive = 0;
    context.close();    
}

const vector<StationEventInfo *> & TrackerOT::getStateVector()
{
    if( !isActive )
    {
	    cout << "WARNING: Tracker is not active." << endl;
        return changedStations;
    }
    changedStations.clear();
    context.pushStates();
    context.pullStates();
    context.stop();
    return changedStations;
}

// returns maximum number of stations

int TrackerOT::getMaxStationNumber()
{
    return maximum+1;
}

// creates new StbSink nodes

Node * TrackerOT::createNode( const string& name,  StringTable& attributes)
{
    if( name.compare("StbSink") == 0 )
    {
        int station, event;
        if( attributes.get("station", &station) == 0 )
        {
            cout << "TrackerOT : Error in converting station number !" << endl;
            return NULL;
        }
        if( attributes.get("event").compare("on") == 0 )        
            event = 1;
        else
            event = 0;
        
        if( station < 0 )
        {
            cout << "TrackerOT : station number negative : " << station << endl;
            return NULL;
        }

        SinkVector::iterator it;
        for( it = sinks.begin(); it != sinks.end(); it ++ )
        {
            if((*it)->state.station == station )
            {
                cout << "TrackerOT : station number " << station << " already used !\n";
                return NULL;
            }
        }
        if( station > maximum )
            maximum = station;
        StbSink * sink = new StbSink(this, station, event );
        sinks.push_back( sink );
        cout << "Built StbSink node for station "<< station << endl;       
        return sink;
    } 
	if( name.compare("StbSource") == 0 )
	{		
		StbSource * source = new StbSource( attributes.get("node"),
											attributes.get("position"),
											attributes.get("orientation"),
											attributes.get("button"),
											attributes.get("confidence"));
		cout << "Built StbSource node for node " << attributes.get("node") << endl;
		sources.push_back( source );
		initialized = 1;
		return source;
	}
    return NULL;
}

void TrackerOT::pushState()
{
	SourceVector::iterator it;
	for( it = sources.begin(); it != sources.end(); it++ )
	{
		(*it)->pushState();
	}
}


void TrackerOT::start()
{
	SourceVector::iterator it;
	for( it = sources.begin(); it != sources.end(); it++ )
	{
		(*it)->checkForNodes();
	}
}

// returns the context used by the OpenTracker tracker

Context * TrackerOT::getContext()
{
    return & context;
}

// return event state for a given station

int TrackerOT::isEventEnabled( int which )
{
    for( SinkVector::iterator it = sinks.begin(); it != sinks.end(); it ++ )
    {
        if( (*it)->state.station == which )
            return (*it)->events;
    }
    return 0;
}

const char *TrackerOT::getFilename()
{
    return filename.c_str();
}

void TrackerOT::addChangedStation( StationEventInfo * stationInfo )
{
    changedStations.push_back( stationInfo );
}
