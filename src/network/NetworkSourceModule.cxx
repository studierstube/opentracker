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
/** source file for NetworkSourceModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSourceModule.cxx,v 1.23 2003/10/31 14:54:46 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <string>

#include <ace/Thread_Manager.h>
#include <ace/Synch.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram_Mcast.h>
 
#include "NetworkSourceModule.h"

#include <iostream>

using namespace std;

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

struct MulticastReceiver
{
    ACE_SOCK_Dgram_Mcast socket;
    /// Mutex to synchronize access to Station data
    ACE_Thread_Mutex mutex;
    /// buffer for incoming package
    FlexibleTrackerDataRecord buffer;
    StationVector sources;
    std::string group;
    int port;
    int stop;

    MulticastReceiver( const std::string & group_, int port_ ) :
        group( group_ ), port( port_ ), stop(0)
    {};
};

// constructor initializing the thread manager
NetworkSourceModule::NetworkSourceModule() : Module(), NodeFactory()
{
//	manager = new ACE_Thread_Manager;
}

// destructor cleans up any allocated memory
NetworkSourceModule::~NetworkSourceModule()
{
    for( ReceiverVector::iterator it = groups.begin(); it != groups.end(); it++)
    {
        for( StationVector::iterator st = (*it)->sources.begin(); st != (*it)->sources.end(); st++ )
        {
            delete (*st);            
        }
        (*it)->sources.clear();
        delete (*it);
    }
    groups.clear();
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
void NetworkSourceModule::run( void * data )
{
    MulticastReceiver * rec = (MulticastReceiver *) data;
    FlexibleTrackerDataRecord & buffer = rec->buffer;
    ACE_INET_Addr remoteAddr;
    ACE_Time_Value timeOut( 1, 0 );
    int retval;
    float help[3][3];
   
    while(1)
    {
        do
        {
            if((retval = rec->socket.recv( &buffer, sizeof( buffer ), remoteAddr, 0,
                    &timeOut )) == -1 )
            {
                if( errno != ETIME && errno != 0 )
                {
                    std::cout << "Error " << errno << " receiving data !" << endl;
                    exit( -1 );
                }
            }    
        } while( retval < 0 && rec->stop == 0);
        if( rec->stop != 0 )
            break;
	    if( ((unsigned short) ntohs(buffer.headerId) != magicNum)||
		((unsigned short) ntohs(buffer.revNum) != revNum))
	        continue;
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
                     station != rec->sources.end(); station++ )
                {
                    if( (*station)->number == stationNumber )
                        break;
                }
                if( station != rec->sources.end())
                {
                    State & state = (*station)->state;
                    int size = 5*sizeof(short int)+ntohs( si[4] );
                    // copy station, this is a critical section                        
                    rec->mutex.acquire();                        
                    state.button = ntohs( si[2] );
                    memcpy(state.position,&stationdata[size],3*sizeof(float));
                    convertFloatsNToHl(state.position,state.position,3);
                    size+=3*sizeof(float); 
                    switch( format )
                    {
                        case positionQuaternion :
                            memcpy( state.orientation, &stationdata[size], 4*sizeof(float));
                            convertFloatsNToHl( state.orientation, state.orientation, 4);
                            break;
                        case positionAngles :                            
                            memcpy( help, &stationdata[size], 3*sizeof(float));
                            convertFloatsNToHl( help[0], help[0], 3);
                            MathUtils::eulerToQuaternion( help[0][0], help[0][1], help[0][2],
                                                         state.orientation );
                            break;
                        case positionMatrix :
                            memcpy( help, &stationdata[size], 9*sizeof(float));
                            convertFloatsNToHl( (float*)help, (float*)help, 9 );
                            MathUtils::matrixToQuaternion( help, state.orientation );
                            break;
                    }
                    (*station)->modified = 1;
                    state.timeStamp();
                    rec->mutex.release();
                    // end of critical section
                }
            }
            // goto next station
            stationdata += ntohs(si[3]);
        }        
    }
    rec->socket.close();
    std::cout << "Stopping thread" << endl;
}
    
 
//  constructs a new Node
Node * NetworkSourceModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("NetworkSource") == 0 )
    { 
        int number, port;
        int num = sscanf(attributes.get("number").c_str(), " %i", &number );
        if( num == 0 ){
            std::cout << "Error in converting NetworkSource number !" << endl;
            return NULL;
        }
        std::string group = attributes.get("multicast-address");
        num = sscanf(attributes.get("port").c_str(), " %i", &port );
        if( num == 0 ){
            std::cout << "Error in converting NetworkSource port number !" << endl;
            return NULL;
        }
        NetworkSource * source = new NetworkSource; 
        ReceiverVector::iterator it;
        for( it = groups.begin(); it != groups.end(); it++)
        {
            if((*it)->group.compare( group ) == 0 && (*it)->port == port )
                break;        
        }
        MulticastReceiver * receiver;
        if( it == groups.end())
        {
            receiver = new MulticastReceiver( group, port );           
            groups.push_back( receiver );
            receiver->sources.push_back( new Station( number, source ));
        } else
        {
            receiver = *it;
            StationVector::iterator sit;        
            for(sit = receiver->sources.begin(); sit != receiver->sources.end();
                    sit++ )
            {
                if((*sit)->number == number )
                    break;
            }
            if( sit != receiver->sources.end())
            {
                std::cout << "There is allready a node for station " << number <<
                             " in group " << group << " !" << endl;
                delete source;
                return NULL;
            }
            receiver->sources.push_back( new Station( number, source ));
        }                        
        std::cout << "Built NetworkSource node." << endl;
        return source;
    }
    return NULL;
}        

// opens the sockets needed for communication and starts the receive thread
void NetworkSourceModule::start()
{
    if( groups.size() == 0 )
        return;
    for( ReceiverVector::iterator it = groups.begin(); it != groups.end(); it++)
    {
        (*it)->socket.subscribe( ACE_INET_Addr((*it)->port, (*it)->group.c_str()));
        ACE_Thread::spawn((ACE_THR_FUNC)NetworkSourceModule::run, *it );
    }    
}
 
// closes the module and closes any communication sockets and stops thread 
void NetworkSourceModule::close()
{
    for( ReceiverVector::iterator it = groups.begin(); it != groups.end(); it++)
    {
        (*it)->mutex.acquire();
        (*it)->stop = 1;
        (*it)->mutex.release();
    }
}   

// pushes state information into the tree
void NetworkSourceModule::pushState()
{
    for(ReceiverVector::iterator rec = groups.begin();rec != groups.end();rec++)
    {
        for(StationVector::iterator it =(*rec)->sources.begin();
                it != (*rec)->sources.end(); it ++ )
        {          
	        // critical section start
			(*rec)->mutex.acquire();
            if((*it)->modified == 1 )
            {
                (*it)->source->state = (*it)->state;
                (*it)->modified = 0;
		        (*rec)->mutex.release();
	            (*it)->source->updateObservers( (*it)->source->state );
            }
			else
		        (*rec)->mutex.release();
			// end of critical section
        }
    }  
}          
