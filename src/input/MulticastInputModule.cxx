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
#include <string>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Time_Value.h>

#include "MulticastInputModule.h"
#include "MulticastInputSource.h"

#include <math.h>
#include <stdio.h>
#include <iostream>

using namespace std;


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
            std::cout << "Error in converting MulticastInputSource number !" << endl;
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
			std::cout << "Source with number "<< number << " exists allready\n";
			return NULL;
		}
        MulticastInputSource * source = new MulticastInputSource( number); 
		sources.push_back( source );
        std::cout << "Built MulticastInputSource node. Number: " << number << endl;
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
					std::cout << "Error " << errno << " receiving data ! " << endl;
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
