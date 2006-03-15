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
/** source file for NetworkSourceModule module.
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

//#include <ace/Thread_Manager.h>
#include <ace/Thread.h>
#include <ace/Synch.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram_Mcast.h>
 
#include "NetworkSourceModule.h"

#include <iostream>

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

//using namespace std;

namespace ot {

struct Station
{
    int number;    
    State state;
    int modified;
    NetworkSource * source;

    Station( const int number_, NetworkSource * source_ ) :
        number( number_ ), modified( 0 ), source( source_ )
    {};
};

typedef std::vector<Station *> StationVector;

// definitions for the Network Data protocol
const int positionQuaternion=1;
const int positionAngles=2;
const int positionMatrix=3;

const int magicNum=0xbeef;
const int revNum=0x0200;

struct NetworkReceiver
{
    /// Mutex to synchronize access to Station data
    ACE_Thread_Mutex mutex;
    /// buffer for incoming package
    FlexibleTrackerDataRecord buffer;
    StationVector sources;
    int stop;

    NetworkReceiver( ) :
        stop(0)
    {}
};

struct UdpReceiver: NetworkReceiver
{
    unsigned short port;

    UdpReceiver( unsigned short port_ ) :
        NetworkReceiver(), port( port_ )
    {}
};

struct MulticastReceiver: UdpReceiver
{
    ACE_SOCK_Dgram_Mcast socket;
    std::string group;

    MulticastReceiver( const std::string & group_, unsigned short port_ ) :
        UdpReceiver( port_ ), group( group_ )
    {}
};

struct UnicastReceiver: UdpReceiver
{
    ACE_SOCK_Dgram socket;
    std::string host;
    ACE_INET_Addr address;

    UnicastReceiver( const std::string & host_, unsigned short port_, const ACE_INET_Addr & address_ ) :
        UdpReceiver( port_ ), host( host_ ), address( address_ )
    {}
};

/** simple functor to find the right multicast receiver. */
struct FindMulticastReceiver {
	std::string group;
	unsigned short port;

	FindMulticastReceiver( const std::string & group_, unsigned short & port_ ) :
		group( group_ ), port( port_)
	{}

	bool operator()( const MulticastReceiver * other )
	{
		return (    group.compare( other->group ) == 0 
			     && port == other->port );
	}
};

/** simple functor to find the right unicast receiver. */
struct FindUnicastReceiver {
	std::string host;
	unsigned short port;

	FindUnicastReceiver( const std::string & host_, unsigned short & port_ ) :
		host( host_ ), port( port_)
	{}

	bool operator()( const UnicastReceiver * other )
	{
		return (    host.compare( other->host ) == 0 
				 && port == other->port );
	}
};

// constructor initializing the thread manager
NetworkSourceModule::NetworkSourceModule() : Module(), NodeFactory()
{
//	manager = new ACE_Thread_Manager;
}

// destructor cleans up any allocated memory
NetworkSourceModule::~NetworkSourceModule()
{
    for( MulticastReceiverVector::iterator mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it )
    {
        for( StationVector::iterator st = (*mc_it)->sources.begin(); st != (*mc_it)->sources.end(); ++st )
        {
            delete (*st);            
        }
        (*mc_it)->sources.clear();
        delete (*mc_it);
    }
    for( UnicastReceiverVector::iterator uc_it = unicasts.begin(); uc_it != unicasts.end(); ++uc_it )
    {
        for( StationVector::iterator st = (*uc_it)->sources.begin(); st != (*uc_it)->sources.end(); ++st )
        {
            delete (*st);            
        }
        (*uc_it)->sources.clear();
        delete (*uc_it);
    }
}

// Converts num floats from network byte order.
void NetworkSourceModule::convertFloatsNToHl(float* floats, float* result, int num)
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
	    convert.l = ntohl(convert.l);  // Convert host to network byte order
	    result[i] = convert.f;
    }
}

// reads from the network and parses network packages
void NetworkSourceModule::runMulticastReceiver( void * data )
{
    MulticastReceiver * rec = (MulticastReceiver *) data;
    FlexibleTrackerDataRecord & buffer = rec->buffer;
    ACE_INET_Addr remoteAddr;
    ACE_Time_Value timeOut( 1, 0 );
    int retval;
   
    while(1)
    {
        do
        {
            if((retval = rec->socket.recv( &buffer, sizeof( buffer ), remoteAddr, 0,
                    &timeOut )) == -1 )
            {
                if( errno != ETIME && errno != 0 )
                {
					ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d receiving data !\n"), errno));
                    exit( -1 );
                }
            }    
        } while( retval < 0 && rec->stop == 0);
        if( rec->stop != 0 )
            break;
        processRecord( rec );
    }
    if( rec->socket.close() == -1)
	{
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSourceModule !\n")));
	}
	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping thread\n")));
}
    
// reads from and writes to the network and parses network packages
void NetworkSourceModule::runUnicastTransceiver( void * data )
{
    UnicastReceiver * rec = (UnicastReceiver *) data;
    FlexibleTrackerDataRecord & buffer = rec->buffer;
    ACE_INET_Addr remoteAddr;
    ACE_Time_Value timeOut( 1, 0 );
    int retval;
    const char poll = 'P';
    const char leave = 'L';
   
    while(1)
    {
        do
        {
            if((retval = rec->socket.recv( &buffer, sizeof( buffer ), remoteAddr, 0,
                    &timeOut )) == -1 )
            {
                if( errno == ETIME )
                {
                    rec->socket.send( &poll, sizeof(poll), rec->address,0, &ACE_Time_Value::zero );
                }
            }    
        } while( retval < 0 && rec->stop == 0);
        if( rec->stop != 0 )
            break;
        processRecord( rec );
    }
    rec->socket.send( &leave, sizeof(leave), rec->address,0, &ACE_Time_Value::zero );
    if( rec->socket.close() == -1)
	{
		ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error closing socket in NetworkSourceModule !\n")));
	}
	ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Stopping thread\n")));
}
 
bool NetworkSourceModule::processRecord( NetworkReceiver * rec )
{
    FlexibleTrackerDataRecord & buffer = rec->buffer;
    float help[3][3];
	if( ((unsigned short) ntohs(buffer.headerId) != magicNum)||
	((unsigned short) ntohs(buffer.revNum) != revNum))
	    return false;
    int maxNumber = ntohs(buffer.maxStationNum);
    int number = ntohs(buffer.numOfStations);
    // skip server name 
    char * stationdata = &buffer.data[ntohs(buffer.commentLength)];
    short int si[5];
    for( int cnt = 0; cnt < number; cnt ++ )
    {
        memcpy(si, stationdata, 5*sizeof( short int ));
        // Data per Station:
        // short int number of the station
        // short int format (Quaternion, Euler, Matrix)
        // short int button states (binary coded)
        // short int bytes per station (incl. this header)
        // short int length of the name of the station
        // n bytes name of the station
        // position and orientation according to format
        int stationNumber = ntohs(si[0]);
        int format = ntohs( si[1] );
        if( stationNumber >= 0 && stationNumber <= maxNumber && 
            format == positionQuaternion )
        {
            StationVector::iterator station;
            for( station = rec->sources.begin(); 
                    station != rec->sources.end(); ++station )
            {
                if( (*station)->number == stationNumber )
                    break;
            }
            if( station != rec->sources.end())
            {
                State & state = (*station)->state;
                int size = 5*sizeof(short int)+ntohs( si[4] );
                int time[2];
                // copy station, this is a critical section                        
                {
                    ACE_Guard<ACE_Thread_Mutex> guard( rec->mutex );
                    state.button = ntohs( si[2] );
                    memcpy(state.position,&stationdata[size],3*sizeof(float));
                    convertFloatsNToHl(state.position,state.position,3);
                    size+=3*sizeof(float); 
                    switch( format )
                    {
                        case positionQuaternion :
                            memcpy( state.orientation, &stationdata[size], 4*sizeof(float));
                            convertFloatsNToHl( state.orientation, state.orientation, 4);
                            size += 4*sizeof(float);
                            break;
                        case positionAngles :                            
                            memcpy( help, &stationdata[size], 3*sizeof(float));
                            convertFloatsNToHl( help[0], help[0], 3);
                            MathUtils::eulerToQuaternion( help[0][0], help[0][1], help[0][2],
                                                        state.orientation );
			                size += 3*sizeof(float);	                                                         
                            break;
                        case positionMatrix :
                            memcpy( help, &stationdata[size], 9*sizeof(float));
                            convertFloatsNToHl( (float*)help, (float*)help, 9 );
                            MathUtils::matrixToQuaternion( help, state.orientation );
                            size += 9*sizeof(float);
                            break;
                    }
                    
                    // copy timestamp data
                    time[0] = ntohl( ( (long*) &stationdata[size] )[0] );
                    time[1] = ntohl( ( (long*) &stationdata[size] )[1] );
                    memcpy(&state.time, (char*)&time, 2*sizeof(long));
                    size += 2*sizeof(int);
                    
                    (*station)->modified = 1;
                }
            }
        }
        // goto next station
        stationdata += ntohs(si[3]);
    }        
    return true;
}

//  constructs a new Node
Node * NetworkSourceModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("NetworkSource") == 0 )
    { 
        int number;
        unsigned short port;
        int num = sscanf(attributes.get("number").c_str(), " %i", &number );
        if( num == 0 ){
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting NetworkSource number !\n")));
            return NULL;
        }
        num = sscanf(attributes.get("port").c_str(), " %hu", &port );
        if( num == 0 ){
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting NetworkSource port number !\n")));
            return NULL;
        }
        if( attributes.get("mode").compare("multicast") == 0 )
        {
            std::string group = attributes.get("multicast-address");
            NetworkSource * source = new NetworkSource; 
            MulticastReceiverVector::iterator it = std::find_if( multicasts.begin(), multicasts.end(), FindMulticastReceiver( group, port ));
            MulticastReceiver * receiver;
            if( multicasts.end() == it )
            {
                receiver = new MulticastReceiver( group, port );           
                multicasts.push_back( receiver );
                receiver->sources.push_back( new Station( number, source ));
            } else
            {
                receiver = *it;
                StationVector::iterator sit;        
                for(sit = receiver->sources.begin(); sit != receiver->sources.end();
                        ++sit )
                {
                    if((*sit)->number == number )
                        break;
                }
                if( sit != receiver->sources.end())
                {
                    LOG_ACE_ERROR("ot:There is already a node for station %d in group %s:%hu !\n", number, group.c_str(), port);
                    delete source;
                    return NULL;
                }
                receiver->sources.push_back( new Station( number, source ));
            }                        
		    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Built NetworkSource node.\n")));
            return source;
        }
        if( attributes.get("mode").compare("unicast") == 0 )
        {
            std::string address = attributes.get("address");
            NetworkSource * source = new NetworkSource; 
            UnicastReceiverVector::iterator it = std::find_if( unicasts.begin(), unicasts.end(), FindUnicastReceiver( address, port ));
            UnicastReceiver * receiver;
            if( unicasts.end() == it )
            {
                receiver = new UnicastReceiver( address, port, ACE_INET_Addr( port, address.c_str() ) );           
                unicasts.push_back( receiver );
                receiver->sources.push_back( new Station( number, source ));
            } else
            {
                receiver = *it;
                StationVector::iterator sit;        
                for(sit = receiver->sources.begin(); sit != receiver->sources.end();
                        ++sit )
                {
                    if((*sit)->number == number )
                        break;
                }
                if( sit != receiver->sources.end())
                {
                    LOG_ACE_ERROR("ot:There is already a node for station %d in %s:%hu !\n", number, address, port);
                    delete source;
                    return NULL;
                }
                receiver->sources.push_back( new Station( number, source ));
            }                        
		    ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Built NetworkSource node.\n")));
            return source;
        }
    }
    return NULL;
}        

// opens the sockets needed for communication and starts the receive thread
void NetworkSourceModule::start()
{
    for( MulticastReceiverVector::iterator mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it )
    {
        (*mc_it)->socket.subscribe( ACE_INET_Addr((*mc_it)->port, (*mc_it)->group.c_str()));
        ACE_Thread::spawn((ACE_THR_FUNC)NetworkSourceModule::runMulticastReceiver, *mc_it );
    }    
    for( UnicastReceiverVector::iterator uc_it = unicasts.begin(); uc_it != unicasts.end(); ++uc_it )
    {
        if( (*uc_it)->socket.open(ACE_Addr::sap_any) == -1 )
		{
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error opening socket in NetworkSourceModule !\n")));
			exit(1);
		}			
        ACE_Thread::spawn((ACE_THR_FUNC)NetworkSourceModule::runUnicastTransceiver, *uc_it );
    }
}
 
// closes the module and closes any communication sockets and stops thread 
void NetworkSourceModule::close()
{
    for( MulticastReceiverVector::iterator mc_it = multicasts.begin(); mc_it != multicasts.end(); ++mc_it )
    {
        ACE_Guard<ACE_Thread_Mutex> guard( (*mc_it)->mutex );
        (*mc_it)->stop = 1;
    }
    for( UnicastReceiverVector::iterator uc_it = unicasts.begin(); uc_it != unicasts.end(); ++uc_it )
    {
        ACE_Guard<ACE_Thread_Mutex> guard( (*uc_it)->mutex );
        (*uc_it)->stop = 1;
    }
}   

// pushes state information into the tree
void NetworkSourceModule::pushState()
{
    for(MulticastReceiverVector::iterator mc_it = multicasts.begin();mc_it != multicasts.end();++mc_it)
    {
        for(StationVector::iterator it =(*mc_it)->sources.begin();
                it != (*mc_it)->sources.end(); ++it )
        {   
            bool updateObservers = false;
	        // critical section
            {
                ACE_Guard<ACE_Thread_Mutex> guard( (*mc_it)->mutex );
                if((*it)->modified == 1 )
                {
                    (*it)->source->state = (*it)->state;
                    (*it)->modified = 0;
                    updateObservers = true;
                }
            }
	        (*it)->source->updateObservers( (*it)->source->state );
        }
    }  
    for(UnicastReceiverVector::iterator uc_it = unicasts.begin();uc_it != unicasts.end();++uc_it)
    {
        for(StationVector::iterator it =(*uc_it)->sources.begin();
                it != (*uc_it)->sources.end(); ++it )
        {          
            bool updateObservers = false;
	        // critical section
            {
                ACE_Guard<ACE_Thread_Mutex> guard( (*uc_it)->mutex );
                if((*it)->modified == 1 )
                {
                    (*it)->source->state = (*it)->state;
                    (*it)->modified = 0;
                    updateObservers = true;
                }
            }
	        (*it)->source->updateObservers( (*it)->source->state );
        }
    }  
}          

} // namespace ot
