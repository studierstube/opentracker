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
/** source file for NetworkSinkModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSinkModule.cxx,v 1.8 2001/04/03 21:44:50 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

// a trick to avoid warnings when ace includes the STL headers
#pragma warning(disable:4786)
#include <vector>

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>

#include "NetworkSinkModule.h"

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

/** a simple struct to relate the address of a multicast group and a 
 * network data buffer that is used to build the packets send to the
 * group. All NetworkSink nodes point to one of these. 
 */
struct MulticastGroup {
    FlexibleTrackerDataRecord data;   
    char * nextRecord;
    string group;
    int port;
    ACE_INET_Addr address;
};

// initializes ConsoleModule

void NetworkSinkModule::init(StringMap& attributes,  Node * localTree)
{
    if( attributes.find("name") != attributes.end())
    {
        serverName = attributes["name"];
    } else
    {
        serverName = "OpenTracker";
    }
    Module::init( attributes, localTree );
}

// This method is called to construct a new Node.

Node * NetworkSinkModule::createNode( string& name,  StringMap& attributes)
{
    if( name.compare("NetworkSink") == 0 )
    {
        string name = attributes["name"];
        int number, port;
        int num = sscanf(attributes["number"].c_str(), " %i", &number );
        if( num == 0 ){
            cout << "Error in converting NetworkSink number !" << endl;
            return NULL;
        }
        string group = attributes["multicast-address"];
        num = sscanf(attributes["port"].c_str(), " %i", &port );
        if( num == 0 ){
            cout << "Error in converting NetworkSink port number !" << endl;
            return NULL;
        }
        GroupVector::iterator it = groups.begin();
        for( ; it != groups.end(); it++)
        {
            if((*it)->group.compare( group ) == 0 && (*it)->port == port )
                break;
        }
        MulticastGroup * groupData;
        if( groups.end() == it )
        {
            groupData = new MulticastGroup;
            groupData->group = group;
            groupData->port = port;
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
        // increase maximum of station numbers 
        groupData->data.maxStationNum++;
        
        NetworkSink * sink = new NetworkSink( name, number, groupData );
        nodes.push_back( sink );
        cout << "Built NetworkSink node " << name << "." << endl;
        return sink;
    }
    return NULL;
}

// opens the network socket to use

void NetworkSinkModule::start()
{
	socket = new ACE_SOCK_Dgram;
    // only open a network connection if we actually have something to do
    if( nodes.size() > 0 )
    {
        if( socket->open( ACE_INET_Addr( 12346, "localhost" ) , PF_INET, 0, 1) == -1 )
        {
            cout << "Error opening socket in NetworkSinkModule !" << endl;
            exit(1);
        }
        // sets maxStationNum to network byte order
        for( GroupVector::iterator it = groups.begin() ; it != groups.end(); it++ )
        {
            (*it)->data.maxStationNum = htons((*it)->data.maxStationNum);                
        }
    }
    Module::start();
}

// closes the network connection

void NetworkSinkModule::close()
{
    if( socket->close() == -1 )
    {
        cout << "Error closing socket !" << endl;
    }
	delete socket;
}
 
// checks the NetworkSink nodes and sends any new data to the network

void NetworkSinkModule::pullState()
{
    if( nodes.size() == 0 )
        return;
    // clear the network buffers
    for( GroupVector::iterator gr_it = groups.begin(); gr_it != groups.end(); gr_it++ )
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
            float temp[4];
            char * index = ( char * ) group->nextRecord;
            
            // 3 position and 4 quaternion entries +
            // 5 short ints for other data + length of station name
            size = (3+4)*sizeof(float) + 5*sizeof(short int);
            size += sink->stationName.length();
            
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
             
            // add to nextRecord and numOfStations
            group->nextRecord += size;
            group->data.numOfStations++;
        }
        sink->modified = 0;
    }                                           
    // send any non-empty network data buffers
    for(gr_it = groups.begin(); gr_it != groups.end(); gr_it++ )
    {
        if( (*gr_it)->data.numOfStations > 0 )
        {
            (*gr_it)->data.numOfStations = htons( (*gr_it)->data.numOfStations );
            int size = (*gr_it)->nextRecord - (char*)&(*gr_it)->data;
            if( socket->send( &(*gr_it)->data, size, (*gr_it)->address ) < 0 )
            {
                cout << "NetworkSinkModule : Error sending packet for " << 
                        (*gr_it)->group << endl;
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
