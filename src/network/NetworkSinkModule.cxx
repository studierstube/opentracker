 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for NetworkSinkModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                    */
 /* ======================================================================== */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <stdlib.h>
#include <string>
#include <algorithm>

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram_Mcast.h>
#include <ace/Time_Value.h>

#include "NetworkSinkModule.h"

#include <iostream>

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

//using namespace std;

namespace ot {

// definitions for the Network Data protocol
const int positionQuaternion=1;
const int positionAngles=2;
const int positionMatrix=3;

const int magicNum=0xbeef;
const int revNum=0x0200;

/** a simple struct to relate the address of a multicast group, a 
 * network data buffer and a socket that is used to build the packets send to the
 * group, depending on the network interface to use. 
 * All NetworkSink nodes point to one of these. 
 */
struct MulticastGroup {
    FlexibleTrackerDataRecord data;   
    char * nextRecord;
    std::string group;
    int port;
    std::string nic;
    ACE_INET_Addr address;
    ACE_SOCK_Dgram socket;
};

/** simple functor to find the right multicast group. */
struct FindGroup {
	std::string group;
	int port;
	std::string nic;

	FindGroup( const std::string & group_, int & port_, const std::string & nic_ ) :
		group( group_ ), port( port_), nic( nic_ )
	{};

	bool operator()( const MulticastGroup * other )
	{
		return (    group.compare( other->group ) == 0 
			     && port == other->port 
				 && nic.compare( other->nic ) == 0 );
	};
};

// initializes ConsoleModule

void NetworkSinkModule::init(StringTable& attributes,  ConfigNode * localTree)
{
    Module::init( attributes, localTree );
    if( attributes.containsKey("name"))
    {
        serverName = attributes.get("name");
    } else
    {
        serverName = "OpenTracker";
    }
}

// This method is called to construct a new Node.

Node * NetworkSinkModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("NetworkSink") == 0 )
    {
        std::string name = attributes.get("name");
        int number, port;
        int num = sscanf(attributes.get("number").c_str(), " %i", &number );
        if( num == 0 ){
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting NetworkSink number !\n")));
            return NULL;
        }
        std::string group = attributes.get("multicast-address");
        num = sscanf(attributes.get("port").c_str(), " %i", &port );
        if( num == 0 ){
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting NetworkSink port number !\n")));
            return NULL;
        }
		std::string nic = attributes.get("interface");

        GroupVector::iterator it = std::find_if( groups.begin(), groups.end(), FindGroup( group, port, nic ));
		/*
        for( ; it != groups.end(); it++)
        {
            if((*it)->group.compare( group ) == 0 && (*it)->port == port && (*it)->group.compare( )
                break;
        }*/
        MulticastGroup * groupData;
        if( groups.end() == it )
        {
            groupData = new MulticastGroup;
            groupData->group = group;
            groupData->port = port;
			groupData->nic = nic;
            groupData->address.set( port, group.c_str() );
		    
            // initialize Network data buffer
            groupData->data.headerId=htons(magicNum);
            groupData->data.revNum=htons(revNum);              
            groupData->data.commentLength = serverName.length();
            strcpy( groupData->data.data, serverName.c_str());
            groupData->data.headerLength= serverName.length() + (sizeof(short int))*6;
            groupData->data.commentLength=htons(groupData->data.commentLength);
            groupData->data.headerLength=htons(groupData->data.headerLength);
            groupData->data.maxStationNum = 0;
            
            groups.push_back( groupData );
        } else
        {
            groupData = *it;
        }
        // increase maximum of station numbers to suit the given number
        groupData->data.maxStationNum = (groupData->data.maxStationNum < number)?(number):(groupData->data.maxStationNum);
        
        NetworkSink * sink = new NetworkSink( name, number, groupData );
        nodes.push_back( sink );
		LOG_ACE_INFO("ot:Built NetworkSink node %s .\n", name.c_str());
        return sink;
    }
    return NULL;
}

// opens the network socket to use

void NetworkSinkModule::start()
{
    // only open a network connection if we actually have something to do
    if( nodes.size() > 0 )
    {
        // sets maxStationNum to network byte order
        for( GroupVector::iterator it = groups.begin() ; it != groups.end(); it++ )
        {
			if( (*it)->socket.open(ACE_Addr::sap_any) == -1 )
			{
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error opening socket in NetworkSinkModule !\n")));
				exit(1);
			}			
			if((*it)->nic.compare("") != 0 )
			{
				(*it)->socket.set_nic(ACE_TEXT_CHAR_TO_TCHAR((*it)->nic.c_str()));
			}
            (*it)->data.maxStationNum = htons((*it)->data.maxStationNum);
        }
    }
    Module::start();
}

// closes the network connection

void NetworkSinkModule::close()
{
	// sets maxStationNum to network byte order
	for( GroupVector::iterator it = groups.begin() ; it != groups.end(); it++ )
	{
		if( (*it)->socket.close() == -1 )
		{
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSinkModule !\n")));
		}
		delete (*it);		
	}
}
 
// checks the NetworkSink nodes and sends any new data to the network

void NetworkSinkModule::pullState()
{
    if( nodes.size() == 0 )
        return;
    // clear the network buffers
    GroupVector::iterator gr_it;
    for( gr_it = groups.begin(); gr_it != groups.end(); gr_it++ )
    {
        (*gr_it)->data.numOfStations = 0;
        (*gr_it)->nextRecord = ((char *)&(*gr_it)->data) + ntohs((*gr_it)->data.headerLength);
    }
    // for each NetworkSink node
    for( SinkVector::iterator it = nodes.begin(); it != nodes.end(); it ++ )
    {
        NetworkSink * sink = *it;
        if( sink->modified == 1 )
        {
            // write it to the network data buffer referenced by the node
            MulticastGroup * group = sink->group;
            State & state = sink->state;
            short int size, si[5];
            int time[2];
            float temp[4];
            char * index = ( char * ) group->nextRecord;
            
            // 3 position and 4 quaternion entries +
            // 5 short ints for other data + length of station name
            // + 1 double for timestamp
            size = (3+4)*sizeof(float) + 5*sizeof(short int);
            size += sink->stationName.length();
            size += sizeof(double);
            
            // Data per Station:
            // short int number of the station
            // short int format (Quaternion, Euler, Matrix)
            // short int button states (binary coded)
            // short int bytes per station (incl. this header)
            // short int length of the name of the station
            // n bytes name of the station
            // position and orientation according to format
            
            si[0] = htons( sink->stationNumber );
            si[1] = htons( positionQuaternion );
            si[2] = htons( state.button );
            si[3] = htons( size );
            si[4] = htons((short int)sink->stationName.length());
            memcpy( index, &si, 5 * sizeof( short int ));
                       
            // copy station name      
            index += 5 * sizeof( short int );
            memcpy(index, sink->stationName.c_str(), sink->stationName.length());

            // copy position data
            index += sink->stationName.length();
            convertFloatsHToNl(state.position,temp, 3 );
            memcpy(index, temp, 3*sizeof( float ));   
                     
            // copy orientation data
            index += 3*sizeof( float );
            convertFloatsHToNl(state.orientation,temp, 4 );
            memcpy(index, temp, 4*sizeof( float ));    
             
	    // copy timestamp data
            index += 4*sizeof( float );
            time[0] = htonl( ( (long*)(&state.time) )[0] );
            time[1] = htonl( ( (long*)(&state.time) )[1] );
            memcpy(index, time, 2*sizeof(int));
            index += 2*sizeof( int );
                    
            // add to nextRecord and numOfStations
            group->nextRecord += size;
            group->data.numOfStations++;
        }
        sink->modified = 0;
    }                                           
    // send any non-empty network data buffers
    for( gr_it = groups.begin(); gr_it != groups.end(); gr_it++ )
    {
        if( (*gr_it)->data.numOfStations > 0 )
        {
            (*gr_it)->data.numOfStations = htons( (*gr_it)->data.numOfStations );
            int size = (*gr_it)->nextRecord - (char*)&(*gr_it)->data;
            // send without blocking to avoid stalls in the mainloop, packet is thrown away !
            if( (*gr_it)->socket.send( &(*gr_it)->data, size, (*gr_it)->address,0, &ACE_Time_Value::zero ) < 0 )
            {
				LOG_ACE_ERROR("ot:NetworkSinkModule : Error sending packet for %s\n", (*gr_it)->group.c_str());
            }
        }
    }
}

// Converts num floats to network byte order

void NetworkSinkModule::convertFloatsHToNl(float* floats, float* result, int num)
{
    int i;
    union
    {
        float f;
        long int l;
    } convert;

    for (i=0; i<num; i++)                 
    {
	    convert.f = floats[i];
	    convert.l = htonl(convert.l);    // Convert host to network byte order
	    result[i] = convert.f;
    }
}

} // namespace ot
