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
#include <ace/Unbounded_Set.h>
#include <ace/Thread.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>

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
struct NetworkSender {
    FlexibleTrackerDataRecord data;   
    char * nextRecord;

    NetworkSender( const FlexibleTrackerDataRecord & data_ ) :
        data( data_ )
    {}
};

struct UdpSender: NetworkSender {
    unsigned short port;
    std::string nic;
    ACE_SOCK_Dgram socket;

    UdpSender( const FlexibleTrackerDataRecord & data_, unsigned short port_, const std::string & nic_ ) :
        NetworkSender( data_ ), port( port_ ), nic( nic_ )
    {}
};

struct MulticastSender: UdpSender {
    std::string group;
    ACE_INET_Addr address;

    MulticastSender( const FlexibleTrackerDataRecord & data_, const std::string & group_, unsigned short port_, const std::string & nic_ ) :
        UdpSender( data_, port_, nic_ ), group( group_ ), address( port, group.c_str() )
    {}
};

struct UnicastSender: UdpSender {
    ACE_Thread_Mutex mutex;
    int stop;
    ACE_Unbounded_Set<ACE_INET_Addr> addresses;

    UnicastSender( const FlexibleTrackerDataRecord & data_, unsigned short port_, const std::string & nic_ ) :
        UdpSender( data_, port_, nic_ ), stop(0)
    {}
};

/** simple functor to find the right multicast sender. */
struct FindMulticastSender {
	std::string group;
	unsigned short port;
	std::string nic;

	FindMulticastSender( const std::string & group_, unsigned short & port_, const std::string & nic_ ) :
		group( group_ ), port( port_), nic( nic_ )
	{}

	bool operator()( const MulticastSender * other )
	{
		return (    group.compare( other->group ) == 0 
			     && port == other->port 
				 && nic.compare( other->nic ) == 0 );
	}
};

/** simple functor to find the right unicast sender. */
struct FindUnicastSender {
	unsigned short port;
	std::string nic;

	FindUnicastSender( unsigned short & port_, const std::string & nic_ ) :
		port( port_), nic( nic_ )
	{}

	bool operator()( const UnicastSender * other )
	{
		return (    port == other->port 
				 && nic.compare( other->nic ) == 0 );
	}
};

NetworkSinkModule::NetworkSinkModule() : Module(), NodeFactory()
{
}

// destructor cleans up any allocated memory
NetworkSinkModule::~NetworkSinkModule()
{
	for( MulticastSenderVector::iterator mc_it = multicasts.begin() ; mc_it != multicasts.end(); ++mc_it )
	{
		delete (*mc_it);		
	}
	for( UnicastSenderVector::iterator uc_it = unicasts.begin() ; uc_it != unicasts.end(); ++uc_it )
	{
		delete (*uc_it);		
	}
}

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
        FlexibleTrackerDataRecord data;   
        // initialize Network data buffer
        data.headerId=htons(magicNum);
        data.revNum=htons(revNum);              
        data.commentLength = serverName.length();
        strcpy( data.data, serverName.c_str());
        data.headerLength= serverName.length() + (sizeof(short int))*6;
        data.commentLength=htons(data.commentLength);
        data.headerLength=htons(data.headerLength);
        data.maxStationNum = 0;

        std::string name = attributes.get("name");
        int number;
        unsigned short port;
        int num = sscanf(attributes.get("number").c_str(), " %i", &number );
        if( num == 0 ){
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting NetworkSink number !\n")));
            return NULL;
        }
        num = sscanf(attributes.get("port").c_str(), " %hu", &port );
        if( num == 0 ){
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting NetworkSink port number !\n")));
            return NULL;
        }

		std::string nic = attributes.get("interface");
        if( attributes.get("mode").compare("multicast") == 0 )
        {
            std::string group = attributes.get("multicast-address");

            MulticastSenderVector::iterator it = std::find_if( multicasts.begin(), multicasts.end(), FindMulticastSender( group, port, nic ));
            MulticastSender * multicastData;
            if( multicasts.end() == it )
            {
                multicastData = new MulticastSender( data, group, port, nic );
                multicasts.push_back( multicastData );
            } else
            {
                multicastData = *it;
            }
            // increase maximum of station numbers to suit the given number
            multicastData->data.maxStationNum = (multicastData->data.maxStationNum < number)?(number):(multicastData->data.maxStationNum);
            
            NetworkSink * sink = new NetworkSink( name, number, multicastData );
            nodes.push_back( sink );
		    LOG_ACE_INFO("ot:Built NetworkSink node %s .\n", name.c_str());
            return sink;
        }
        if( attributes.get("mode").compare("unicast") == 0 )
        {
            UnicastSenderVector::iterator it = std::find_if( unicasts.begin(), unicasts.end(), FindUnicastSender( port, nic ));
            UnicastSender * unicastData;
            if( unicasts.end() == it )
            {
                unicastData = new UnicastSender( data, port, nic );
                unicasts.push_back( unicastData );
            } else
            {
                unicastData = *it;
            }
            // increase maximum of station numbers to suit the given number
            unicastData->data.maxStationNum = (unicastData->data.maxStationNum < number)?(number):(unicastData->data.maxStationNum);
            
            NetworkSink * sink = new NetworkSink( name, number, unicastData );
            nodes.push_back( sink );
		    LOG_ACE_INFO("ot:Built NetworkSink node %s .\n", name.c_str());
            return sink;
        }
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
        for( MulticastSenderVector::iterator mc_it = multicasts.begin() ; mc_it != multicasts.end(); ++mc_it )
        {
			if( (*mc_it)->socket.open(ACE_Addr::sap_any) == -1 )
			{
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error opening socket in NetworkSinkModule !\n")));
				exit(1);
			}			
			if((*mc_it)->nic.compare("") != 0 )
			{
				(*mc_it)->socket.set_nic(ACE_TEXT_CHAR_TO_TCHAR((*mc_it)->nic.c_str()));
			}
            (*mc_it)->data.maxStationNum = htons((*mc_it)->data.maxStationNum);
        }
        for( UnicastSenderVector::iterator uc_it = unicasts.begin() ; uc_it != unicasts.end(); ++uc_it )
        {
            if( (*uc_it)->socket.open(ACE_INET_Addr((*uc_it)->port)) == -1 )
			{
				ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error opening socket in NetworkSinkModule !\n")));
				exit(1);
			}			
			if((*uc_it)->nic.compare("") != 0 )
			{
				(*uc_it)->socket.set_nic(ACE_TEXT_CHAR_TO_TCHAR((*uc_it)->nic.c_str()));
			}
            (*uc_it)->data.maxStationNum = htons((*uc_it)->data.maxStationNum);
            ACE_Thread::spawn((ACE_THR_FUNC)NetworkSinkModule::runUnicastTransceiver, *uc_it );
        }
    }
    Module::start();
}

// closes the network connection

void NetworkSinkModule::close()
{
	for( MulticastSenderVector::iterator mc_it = multicasts.begin() ; mc_it != multicasts.end(); ++mc_it )
	{
		if( (*mc_it)->socket.close() == -1 )
		{
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSinkModule !\n")));
		}
	}
	for( UnicastSenderVector::iterator uc_it = unicasts.begin() ; uc_it != unicasts.end(); ++uc_it )
	{
        ACE_Guard<ACE_Thread_Mutex> guard( (*uc_it)->mutex );
        (*uc_it)->stop = 1;
	}
}
 
// checks the NetworkSink nodes and sends any new data to the network

void NetworkSinkModule::pullState()
{
    if( nodes.size() == 0 )
        return;
    // clear the network buffers
    for( MulticastSenderVector::iterator mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it )
    {
        (*mc_it)->data.numOfStations = 0;
        (*mc_it)->nextRecord = ((char *)&(*mc_it)->data) + ntohs((*mc_it)->data.headerLength);
    }
    for( UnicastSenderVector::iterator uc_it = unicasts.begin(); uc_it != unicasts.end(); ++uc_it )
    {
        (*uc_it)->data.numOfStations = 0;
        (*uc_it)->nextRecord = ((char *)&(*uc_it)->data) + ntohs((*uc_it)->data.headerLength);
    }
    // for each NetworkSink node
    for( SinkVector::iterator it = nodes.begin(); it != nodes.end(); ++it )
    {
        NetworkSink * sink = *it;
        if( sink->modified == 1 )
        {
            // write it to the network data buffer referenced by the node
            NetworkSender * network = sink->networkSender;
            State & state = sink->state;
            short int size, si[5];
            int time[2];
            float temp[4];
            char * index = ( char * ) network->nextRecord;
            
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
            network->nextRecord += size;
            network->data.numOfStations++;
        }
        sink->modified = 0;
    }                                           
    // send any non-empty network data buffers
    for( mc_it = multicasts.begin(); mc_it != multicasts.end(); mc_it++ )
    {
        if( (*mc_it)->data.numOfStations > 0 )
        {
            (*mc_it)->data.numOfStations = htons( (*mc_it)->data.numOfStations );
            int size = (*mc_it)->nextRecord - (char*)&(*mc_it)->data;
            // send without blocking to avoid stalls in the mainloop, packet is thrown away !
            if( (*mc_it)->socket.send( &(*mc_it)->data, size, (*mc_it)->address,0, &ACE_Time_Value::zero ) < 0 )
            {
                LOG_ACE_ERROR("ot:NetworkSinkModule : Error sending packet for %s:%hu\n", (*mc_it)->address.get_host_name(), (*mc_it)->address.get_port_number());
            }
        }
    }
    for( uc_it = unicasts.begin(); uc_it != unicasts.end(); uc_it++ )
    {
        if( (*uc_it)->data.numOfStations > 0 )
        {
            ACE_Guard<ACE_Thread_Mutex> guard( (*uc_it)->mutex );
            (*uc_it)->data.numOfStations = htons( (*uc_it)->data.numOfStations );
            int size = (*uc_it)->nextRecord - (char*)&(*uc_it)->data;
            // send without blocking to avoid stalls in the mainloop, packet is thrown away !
            for( ACE_Unbounded_Set_Iterator<ACE_INET_Addr> it = (*uc_it)->addresses.begin() ; ! it.done(); it.advance() )
                if( (*uc_it)->socket.send( &(*uc_it)->data, size, *it,0, &ACE_Time_Value::zero ) < 0 )
                {
				    LOG_ACE_ERROR("ot:NetworkSinkModule : Error sending packet for %s:%hu\n", (*it).get_host_name(), (*it).get_port_number());
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

void NetworkSinkModule::runUnicastTransceiver( void * data )
{
    UnicastSender * uc = (UnicastSender *) data;
    ACE_INET_Addr remoteAddr;
    ACE_Time_Value timeOut( 1, 0 );
    int retval;
    char command;
    while(1)
    {
        do
        {
            if((retval = uc->socket.recv( &command, sizeof( command ), remoteAddr, 0,
                    &timeOut )) == -1 )
            {
                if( errno != ETIME && errno != 0 )
                {
					ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d receiving data !\n"), errno));
                    exit( -1 );
                }
            }    
        } while( retval < 0 && uc->stop == 0);
        if( uc->stop != 0 )
            break;
        {
            ACE_Guard<ACE_Thread_Mutex> guard( uc->mutex );
            const char poll = 'P';
            const char leave = 'L';
            switch (command)
            {
            case poll:
                if( uc->addresses.find( remoteAddr ) )
                    uc->addresses.insert( remoteAddr );
                break;
            case leave:
                if( !uc->addresses.find( remoteAddr ) )
                    uc->addresses.remove( remoteAddr );
            }
        }
    }
    if( uc->socket.close() == -1)
	{
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSinkModule !\n")));
	}
	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping thread\n")));
}

} // namespace ot
