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
/** source file for TCPModule Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <string>

#include <ace/ACE.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include "TCPModule.h"

#include <iostream>

using namespace std;

#define SERVER_DATA_PRECISION 100
#define SERVER_DATA_SHIFT     1000

// destuctor, clears up any sinks

TCPModule::~TCPModule(){
    for(NodeVector::iterator it = sinks.begin(); it != sinks.end(); it++ ){
        delete (*it);
    }
    sinks.clear();
}

// creates new nodes for the parser

Node * TCPModule::createNode( const std::string& name,  StringTable& attributes){
    if( name.compare("TCPSink") == 0 ){
        int station, posFlag, rotFlag, buttonFlag,timeFlag;
        int num = sscanf( attributes.get("station").c_str()," %i", 
&station );
        if( num != 1 ){
            std::cout << "TCPModule : not a number in TCPSink station " << attributes.get("station") << endl;
            return NULL;
        }
        if( attributes.get("position").compare("on") == 0 )
            posFlag = 1;
        else
            posFlag = 0;
        if( attributes.get("orientation").compare("on") == 0 )
            rotFlag = 1;
        else
            rotFlag = 0;
        if( attributes.get("button").compare("on") == 0 )
            buttonFlag = 1;
        else
            buttonFlag = 0;
        if( attributes.get("time").compare("on") == 0 )
            timeFlag = 1;
        else
            timeFlag = 0;
        TCPSink * sink = new TCPSink( station, posFlag, rotFlag, buttonFlag, timeFlag );
        sinks.push_back( sink );
        std::cout << "TCPSink for station " << station << " created." << endl;
        return sink;
    }
    return NULL;
}

// checks the stations for changes and transmits changes to clients

void TCPModule::pullState(){
    char buffer[8*sizeof(int)+2*sizeof(short unsigned)+sizeof(double)];
    short unsigned protocol;
    int index;

    if( isInitialized() == 1 && connections.size() != 0 && sinks.size() != 0 ){
        for( NodeVector::iterator it = sinks.begin(); it != sinks.end(); it ++ ){
            TCPSink * sink = (TCPSink*) (*it);
            State & state = sink->state;
            if( sink->changed == 1 ){
                protocol = sink->posFlag | (sink->orientFlag << 1) | (sink->buttonFlag << 2) |
                           (sink->timeFlag << 3);
                index = 0;
                // write converted data into buffer
                *(int*)(&buffer[index]) = htonl(sink->station);
                index += sizeof(int);
                *(short unsigned *)(&buffer[index]) = htons(protocol);
                index += sizeof(short unsigned);
                if( sink->posFlag == 1 ){
                    *(int*)(&buffer[index]) = htonl(*(int*)(&state.position[0]));             
                    index += sizeof(int);
                    *(int*)(&buffer[index]) = htonl(*(int*)(&state.position[1])); 
                    index += sizeof(int);
                    *(int*)(&buffer[index]) = htonl(*(int*)(&state.position[2]));
                    index += sizeof(int);
                }
                if( sink->orientFlag == 1 ){
                    *(int*)(&buffer[index]) = htonl(*(int*)(&state.orientation[0]));
                    index += sizeof(int);
                    *(int*)(&buffer[index]) = htonl(*(int*)(&state.orientation[1]));
                    index += sizeof(int);
                    *(int*)(&buffer[index]) = htonl(*(int*)(&state.orientation[2]));
                    index += sizeof(int);
                    *(int*)(&buffer[index]) = htonl(*(int*)(&state.orientation[3]));
                    index += sizeof(int);
                }
                if( sink->buttonFlag == 1 ){
                    *(short int*)(&buffer[index]) = htons(state.button);
                    index += sizeof(short int);
                }
                if( sink->timeFlag == 1 ){
                    *(int*)(&buffer[index]) = htonl((*(long*)(&state.time))&&0xffffffff);
                    index += sizeof(int);
                    *(int*)(&buffer[index]) = htonl((*(long*)(&state.time))>>32);
                    index += sizeof(int);
                }
                // send to all connections
                lock();
                for( std::vector<ACE_SOCK_Stream *>::iterator conit = connections.begin(); conit != connections.end(); conit ++ ){
                    if( (*conit)->send_n( buffer, index, 0 ) != index ){
                        (*conit)->close();
                        delete (*conit);
                        connections.erase( conit-- );
                        std::cout << "TCPModule : closed connection\n";
                    }
                }
                unlock();
                sink->changed = 0;
            }
        }
    }
}

// tells the acceptor thread to stop

void TCPModule::close(){
    if( isInitialized() == 1 ){
        running = 0;
    }
}

// sets the port the module is listening to and initializes module

void TCPModule::init(StringTable& attributes,  ConfigNode * localTree){
    if(attributes.get("port").compare("") != 0 ){
        int num = sscanf(  attributes.get("port").c_str(), " %i", &port );
        if( num == 1 ){
            ThreadModule::init( attributes, localTree );
            std::cout << "TCPModule listening to port " << port << endl;
        }
    }
}
    
// starts the main loop

void TCPModule::start(){
    if( isInitialized() == 1 ){
        ThreadModule::start();
        std::cout << "TCPModule started !" << endl;
    }
}

// the thread functions opens an acceptor socket and listens to new connections

void TCPModule::run(){
    ACE_INET_Addr client, server( port );
    ACE_Time_Value timeout(10000); 
    char buffer[100];

    ACE_SOCK_Acceptor acceptor( server );
    running = 1;

    while(running){
        ACE_SOCK_Stream * stream = new ACE_SOCK_Stream();
        if(acceptor.accept(*stream, &client, &timeout) == -1){ 
            std::cout << "TCPModule : error listening to socket\n";
            continue; 
        } 
        client.addr_to_string( buffer, 100 );
        std::cout << "TCPModule : new connection from " << buffer << endl;
        lock();
        connections.push_back( stream );
        unlock();
    }
    acceptor.close();
    for( std::vector<ACE_SOCK_Stream *>::iterator conit = connections.begin(); conit != connections.end(); conit ++ ){
        (*conit)->close();
        delete (*conit);
    }
    connections.clear();
}
