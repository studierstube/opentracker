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
/** source file for VRPN module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/VRPNModule.cxx,v 1.1 2004/03/19 08:08:59 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "VRPNModule.h"
#include "VRPNSource.h"
#include "VRPNSink.h"

#ifdef USE_VRPN

#ifdef WIN32
#pragma comment(lib,"vrpn.lib")
#endif

// #define ACE_NTRACE 0

#include <ace/Log_Msg.h>

#include <vrpn_Connection.h>

using namespace std;

VRPNModule::VRPNModule() : 
Module(), 
NodeFactory()
{
}

VRPNModule::~VRPNModule()
{
    VRPNSourceVector::iterator it;
    for( it = sources.begin(); it != sources.end(); it++ )
    {
        delete (*it);
    }
    sources.clear();
    VRPNSinkVector::iterator it2;
    for( it2 = sinks.begin(); it2 != sinks.end(); it2++ )
    {
        delete (*it2);
    }
    sinks.clear();
}

void VRPNModule::init(StringTable & attributes,  ConfigNode * localTree)
{
    Module::init( attributes, localTree );
    if( attributes.get("port", &port ) != 1 )
        port = vrpn_DEFAULT_LISTEN_PORT_NO;
    ip = attributes.get("interface");
}

// called to construct a new Node.

Node * VRPNModule::createNode( const string& name, StringTable& attributes)
{
    ACE_TRACE(ACE_TEXT("VRPNModule::createNode"));
    if( name.compare("VRPNSource") == 0 )
    {
        const string & nameVal = attributes.get("name");
        VRPNSource::Type typeVal = VRPNSource::TRACKER;;
        if( attributes.get("type").compare("button") == 0 )
            typeVal = VRPNSource::BUTTON;
        int stationVal = 0;
        attributes.get("station", &stationVal);
        VRPNSource * source = new VRPNSource;
        source->name = nameVal;
        source->type = typeVal;
        source->station = stationVal;
        sources.push_back( source );
        return source;
    } 
    else if( name.compare("VRPNSink") == 0 )  
    {
        const string & nameVal = attributes.get("name");
        VRPNSink::Type typeVal = VRPNSink::TRACKER;
        if( attributes.get("type").compare("button") == 0 )
            typeVal = VRPNSink::BUTTON;
        int stationVal = 0;
        attributes.get("station", &stationVal);
        VRPNSink * sink = new VRPNSink;
        sink->name = nameVal;
        sink->type = typeVal;
        sink->station = stationVal;
        sinks.push_back( sink );
        return sink;
    }
    return NULL;
}

void VRPNModule::start()
{
    ACE_TRACE(ACE_TEXT("VRPNModule::start"));
    VRPNSourceVector::iterator it;
    for( it = sources.begin(); it != sources.end(); it++ )
    {
        (*it)->start();
    }

    if( ip.compare("") == 0 )
        connection = new vrpn_Synchronized_Connection( port );
    else
        connection = new vrpn_Synchronized_Connection( port, NULL, NULL, ip.c_str() );

    VRPNSinkVector::iterator it2;
    for( it2 = sinks.begin(); it2 != sinks.end(); it2++ )
    {
        (*it2)->start(connection);
    }
}

void VRPNModule::pushState()
{
    VRPNSourceVector::iterator it;
    for( it = sources.begin(); it != sources.end(); it++ )
    {
        (*it)->mainloop();
    }
}

void VRPNModule::pullState()
{
    ACE_TRACE(ACE_TEXT("VRPNModule::pullState"));
    VRPNSinkVector::iterator it;
    for( it = sinks.begin(); it != sinks.end(); it++ )
    {
        (*it)->mainloop();
    }
    connection->mainloop();
}

#endif
