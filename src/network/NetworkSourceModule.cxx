 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for NetworkSourceModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSourceModule.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */
 
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

void NetworkSourceModule::run()
{
    ACE_INET_Addr remoteAddr;
    ACE_Time_Value timeOut( 1, 0 );
    int retval;
    while(1)
    {
        do
        {
            if((retval = socket.recv( &buffer, sizeof( buffer ), remoteAddr, 0,
                    &timeOut )) == -1 )
            {
                if( errno != ETIME )
                {
                    cout << "Error receiving data !" << endl;
                    exit( -1 );
                } else
                {
                    cout << "Time out receiving data." << endl;
                }
            }
        } while( retval < 0 );
        // so we finally received something ...
        // find the group this was going to
        ReceiverVector::iterator it;
        for( it = groups.begin(); it != groups.end(); it++ )
        {
            if( (*it)->address == remoteAddr )
                break;
        }
        if( it != groups.end())
        {
            // decode it !
            // early exit if invalid record
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
                    for( station = (*it)->sources.begin(); 
                         station != (*it)->sources.end(); station++ )
                    {
		        if( (*station)->number == stationNumber )
                            break;
                    }
                    if( station != (*it)->sources.end())
                    {
                        State & state = (*station)->state;
                        int size = 5*sizeof(short int)+ntohs( si[4] );
                        // copy station, this is a critical section                        
                        lock();                        
                        state.button = ntohs( si[2] );
                        memcpy(state.position,&stationdata[size],3*sizeof(float));
                        convertFloatsNToHl(state.position,state.position,3);
                        size+=3*sizeof(float);                       
                        memcpy(state.orientation,&stationdata[size],4*sizeof(float));
                        convertFloatsNToHl(state.orientation,state.orientation,3);
                        (*station)->modified = 1;
                        state.timeStamp();
                        unlock();
                        // end of critical section
                    }
                }
                // goto next station
                stationdata += ntohs(si[3]);
            }
        }else
        {
            cout << "Received data from strange group " <<
                    remoteAddr.get_host_addr() << endl;
        }
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
            receiver->address.set( port, group.c_str()); 
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
        socket.subscribe( (*it)->address );
    }
    ThreadModule::start();
}
 
// closes the module and closes any communication sockets and stops thread 
void NetworkSourceModule::close()
{
    ThreadModule::stop();
    socket.close();
}   

// pushes state information into the tree

void NetworkSourceModule::pushState()
{
    // critical section start
    lock();
    for(ReceiverVector::iterator rec = groups.begin();rec != groups.end();rec++)
    {
        for(StationVector::iterator it =(*rec)->sources.begin();
                it != (*rec)->sources.end(); it ++ )
        {
            if((*it)->modified == 1 )
            {
                (*it)->source->state = (*it)->state;
                (*it)->modified = 0;
                (*it)->source->push();
            }
        }
    }
    unlock();
    // end of critical section
}          
