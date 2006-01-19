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
/** source file for MulticastInputModule module.
*
* @author Thomas Psik
*
* $Id$
* @file                                                                   */
/* ======================================================================= */
// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <stdlib.h>
#include <string>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Time_Value.h>
#include <ace/Log_Msg.h>

#include "MulticastInputModule.h"
#include "MulticastInputSource.h"

#include <math.h>
#include <stdio.h>
#include <iostream>

//using namespace std;


namespace ot {


static const float DEG_TO_RAD = (float)(3.14159/180.0);


// -------------------------------------------------------------------------------------------------------
// constructor initializing the thread manager
MulticastInputModule::MulticastInputModule() : ThreadModule(), NodeFactory(), stop(0)
{
	
}
// -------------------------------------------------------------------------------------------------------

// Destructor method

MulticastInputModule::~MulticastInputModule()
{
	sources.clear();
}


// This method is called to construct a new Node.

Node * MulticastInputModule::createNode( const std::string& name, StringTable& attributes)
{
    if( name.compare("MulticastInputSource") == 0 )
    { 
        stop = 0;
		int number;
        int num = sscanf(attributes.get("number").c_str(), " %i", &number );
        if( num == 0 )
		{
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error in converting MulticastInputSource number !\n")));
            return NULL;
        }
		NodeVector::iterator it;
		for( it = sources.begin(); it != sources.end(); it++ ){
			MulticastInputSource * source = (MulticastInputSource*)(*it);
			if( source->number == number )
			{
				break;
			}
		}
		if( it != sources.end())
		{
			ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Source with number %d exists allready\n"), number));
			return NULL;
		}
        MulticastInputSource * source = new MulticastInputSource( number); 
		sources.push_back( source );
		ACE_DEBUG((LM_INFO, ACE_TEXT("ot:Built MulticastInputSource node. Number: %d\n"), number));
        return source;
	}
    return NULL;
}

// -------------------------------------------------------------------------------------------------------

void MulticastInputModule::start()
{
	if( isInitialized() && !sources.empty())
        ThreadModule::start();    
}	


// -------------------------------------------------------------------------------------------------------

void MulticastInputModule::close()
{
	lock();
	stop = 1;
	unlock();
}   

// -------------------------------------------------------------------------------------------------------

void MulticastInputModule::run()
{
    ACE_Time_Value timeOut( 1, 0 );
    int retval;
	ACE_INET_Addr addr( port );
//	ACE_Addr local( -1 , -1);
	socket = new ACE_SOCK_Dgram( addr );
	
	receiveBufferSize =  3 * sizeof(short int) + 8 * sizeof(StationRecord);
	receiveBuffer = new char[receiveBufferSize];
    


	std::string receiveString;
	// mainloop for reading data from the port
    StationRecord *recordTemp = new StationRecord[8];
	
	while(1)
	{
		do
		{	
			if( (retval = socket->recv( receiveBuffer, receiveBufferSize , addr, 0, &timeOut )) == -1 )
			{
				if(errno != ETIME && errno != 0)
				{
					ACE_DEBUG((LM_ERROR, ACE_TEXT("ot:Error %d receiving data !\n"), errno));
					exit( -1 );
				}
			}
		} while( retval < 0 && stop == 0);
		
		if( stop != 0 )
		{
			break;
		}
		
		// from here the String is in the Buffer!
		// converts c-String into String
	//	printf("recv size %d\n",retval);
	//	printf(".");
		parseString(std::string(receiveBuffer, retval), recordTemp);
		NodeVector::iterator it;
		
		lock();
		for( it = sources.begin(); it != sources.end(); it++)
		{
							
			MulticastInputSource * source = (MulticastInputSource*)(*it);
			int bodyID = source->number;
			if( recordTemp[bodyID].valid == 1 )
			{
				// Brings the locationdata from BodyRecordTemp to  source->state.position !
				source->state.position[0] = recordTemp[bodyID].position[0];
				source->state.position[1] = recordTemp[bodyID].position[1];
				source->state.position[2] = recordTemp[bodyID].position[2];
		//		printf("%f %f %f\n",source->state.position[0], source->state.position[1], source->state.position[2]);
				// Brings the calculated Quaternion Data from BodyRecordTemp to source->state.orientation !
				MathUtils::eulerToQuaternion( recordTemp[bodyID].orientation[0], recordTemp[bodyID].orientation[1], recordTemp[bodyID].orientation[2],
                                                         source->state.orientation );
				// Bring a timeStamp to source->state
				source->state.timeStamp();
				// Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
				// fixed to 0.5 ??
				source->state.confidence = recordTemp[bodyID].confidence;	
				// Source was definitly changed !
				source->changed = 1;
			 }
			else
			{
				// only if marker was found in the last grab (state.confidence > epsilon) set 
				// confidence to 0.0!
				if (source->state.confidence > 0.000001f)
				{
					source->changed = 1;
					source->state.confidence = 0.0f;
				}
		  }

		}
		unlock();
	}
	socket->close();
}

	// data package
	// short int version
	// short int numberOfStations
	// short int flags
	// stations[8]:
	// short int valid
	// short int stationId
	// float[3] position
	// float[4] rotation (quaternion)
	// short int buttonbytes
	// float     confidence

void MulticastInputModule::parseString(std::string input, StationRecord *recordTemp) 
{
//	assert(recordTemp!=NULL);
	
	const char *stream = input.c_str();

	version = ntohs(*((short int*) stream));
	stream +=sizeof(u_short);
	u_short numberOfStations = ntohs(*((short int*) stream));
	stream +=sizeof(u_short);
	u_short flags = ntohs(*((short int*) stream));
	stream +=sizeof(u_short);
	
	if (input.size()< 6+numberOfStations*(3*sizeof(u_short)+7*sizeof(float)))
	{
		printf("version %d numberOfStations %d flags %d\n",version, numberOfStations, flags);
		printf("network packet size is %d instead of %d\nEXIT.",input.size(),  6+numberOfStations*sizeof(StationRecord));
		exit(1);
	}
	
	if (version == 1) {
		for (int i=0; i<numberOfStations; i++) {
	// short int valid
	// short int stationId
	// float[3] position
	// float[3] rotation (quaternion)
	// short int buttonbytes
	// float     confidence
		recordTemp[i].valid = ntohs(*((u_short*) stream));
		stream +=sizeof(u_short);
		recordTemp[i].id = ntohs(*((u_short*) stream));
		stream +=sizeof(u_short);

	/*	for (int j=0; j < 4 ; j++) 
		{
			printf("%d ",stream[j]);
		}
			printf("\n");
		
		u_long c = 0;
		char buffer[4];
		memcpy((void*)&c, (void*) stream, sizeof(u_long));
		
		
		for (j=0; j < 4 ; j++) 
		{
			printf("%d ",buffer[j]);
		}
			

		printf("\n-- %f -----\n", t);
	*/
		union longToFloat {
			long l;
			float f;
		} con;

		con.l = ntohl(*((long int*) stream));	
		recordTemp[i].position[0] = con.f;
		stream +=sizeof(u_long);


		con.l = ntohl(*((long int*) stream));	
		recordTemp[i].position[1] = con.f;
		stream +=sizeof(u_long);

		con.l = ntohl(*((long int*) stream));	
		recordTemp[i].position[2] = con.f;
		stream +=sizeof(u_long);
		
		con.l = ntohl(*((long int*) stream));	
		recordTemp[i].orientation[0] = con.f;
		stream +=sizeof(u_long);
		
		con.l = ntohl(*((long int*) stream));	
		recordTemp[i].orientation[1] = con.f;
		stream +=sizeof(u_long);

		con.l = ntohl(*((long int*) stream));	
		recordTemp[i].orientation[2] = con.f;
		stream +=sizeof(u_long);
		
		recordTemp[i].buttons = ntohs(*((u_short*) stream));
		stream +=sizeof(u_short);

		con.l = ntohl(*((long int*) stream));	
		recordTemp[i].confidence = con.f;
		stream +=sizeof(u_long);
/*
		printf("%d %d - %f/%f/%f - %f/%f/%f/%f - %d - %f\n", 
			recordTemp[i].valid,
			recordTemp[i].id,
			recordTemp[i].position[0],
			recordTemp[i].position[1],
			recordTemp[i].position[2],
			recordTemp[i].orientation[0],
			recordTemp[i].orientation[1],
			recordTemp[i].orientation[2],
			recordTemp[i].orientation[3],
			recordTemp[i].buttons,
			recordTemp[i].confidence);
*/
		}
  } else {
		printf("received wrong version %d instead of 1. EXIT.\n",version);
		exit(1);
	}
}

// pushes events into the tracker tree
void MulticastInputModule::pushState()
{
    if( isInitialized() )
    {
	    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it++ )
	    {
		    MulticastInputSource *source = (MulticastInputSource *) *it;
		    lock();
		    if( source->changed == 1 )
		    {			
			    source->updateObservers( source->state );
			    source->changed = 0;
		    }
		    unlock();
	    }
    }
}

// -------------------------------------------------------------------------------------------------------

// initializes the ARTDataTracker module

void MulticastInputModule::init(StringTable& attributes, ConfigNode * localTree)
{
	ThreadModule::init( attributes, localTree );
	int num;
    
	// Scanning port number from XML-File
	num = sscanf(attributes.get("port").c_str(), " %i", &port );
	if( num == 0 )
	{
		port = 12348;
	}
	
}


}  // namespace ot
