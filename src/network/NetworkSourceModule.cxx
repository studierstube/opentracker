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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for NetworkSourceModule module.
  *
  * @todo implement receiving angles and matrices as rotational values
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSourceModule.cxx,v 1.7 2001/03/27 06:08:51 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

#include <ace/Thread_Manager.h>
#include <ace/Synch.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram_Mcast.h>
 
#include "NetworkSourceModule.h"

#include <string.h>

#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

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
    string group;
    int port;
    int stop;
};

// constructor initializing the thread manager
NetworkSourceModule::NetworkSourceModule() : Module(), NodeFactory()
{
	manager = new ACE_Thread_Manager;
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
    while(1)
    {
        do
        {
            if((retval = rec->socket.recv( &buffer, sizeof( buffer ), remoteAddr, 0,
                    &timeOut )) == -1 )
            {
                if( errno != ETIME && errno != 0 )
                {
                    cout << "Error " << errno << " receiving data !" << endl;
                    exit( -1 );
                }
            }
        } while( retval < 0 );
 
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
            if( stationNumber > 0 && stationNumber <= maxNumber && 
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
                    memcpy(state.orientation,&stationdata[size],4*sizeof(float));
                    convertFloatsNToHl(state.orientation,state.orientation,4);
                    (*station)->modified = 1;
                    state.timeStamp();
                    rec->mutex.release();
                    // end of critical section
                }
            }
            // goto next station
            stationdata += ntohs(si[3]);
        }
        // test for stop flag, critical section
        rec->mutex.acquire();
        if( rec->stop == 1 )
        {
            rec->mutex.release();
			cout << "Stopping thread" << endl;
            break;
        } else {
            rec->mutex.release();
        }
        // end of critical section
    }
}
    
 
//  constructs a new Node

Node * NetworkSourceModule::createNode( string& name,  StringMap& attributes)
{
    if( name.compare("NetworkSource") == 0 )
    { 
        int number, port;
        int num = sscanf(attributes["number"].c_str(), " %i", &number );
        if( num == 0 ){
            cout << "Error in converting NetworkSource number !" << endl;
            return NULL;
        }
        string group = attributes["multicast-address"];
        num = sscanf(attributes["port"].c_str(), " %i", &port );
        if( num == 0 ){
            cout << "Error in converting NetworkSource port number !" << endl;
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
            receiver = new MulticastReceiver;
            receiver->group = group;
            receiver->port = port;
//            receiver->address.set( port, group.c_str()); 
            receiver->stop = 0;            
            groups.push_back( receiver );
            Station * station = new Station;
            station->number = number;
            station->source = source;
            station->modified = 0;
            receiver->sources.push_back( station );
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
                cout << "There is allready a node for station " << number <<
                        " in group " << group << " !" << endl;
                delete source;
                return NULL;
            }
            Station * station = new Station;
            station->number = number;
            station->source = source;
            station->modified = 0;
            receiver->sources.push_back( station );
        }                        
        cout << "Built NetworkSource node." << endl;
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
    for( it = groups.begin(); it != groups.end(); it++)
    {
        (*it)->socket.close();
    }
}   

// pushes state information into the tree

void NetworkSourceModule::pushState()
{
    for(ReceiverVector::iterator rec = groups.begin();rec != groups.end();rec++)
    {
        // critical section start
        (*rec)->mutex.acquire();
        for(StationVector::iterator it =(*rec)->sources.begin();
                it != (*rec)->sources.end(); it ++ )
        {          
            if((*it)->modified == 1 )
            {
                (*it)->source->state = (*it)->state;
                (*it)->modified = 0;
                (*it)->source->updateObservers( (*it)->source->state );
            }
        }
        (*rec)->mutex.release();
        // end of critical section
    }  
}          
