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
/** source file for ARTDataTrackerModule module.
*
* @author Christopher Schmidt
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

#include "ARTDataTrackerModule.h"
#include "ARTDataTrackerSource.h"

#include <math.h>
#include <stdio.h>
#include <iostream>

using namespace std;

static const float DEG_TO_RAD = (float)(3.14159/180.0);

// -------------------------------------------------------------------------------------------------------
// constructor initializing the thread manager
ARTDataTrackerModule::ARTDataTrackerModule() : ThreadModule(), NodeFactory(), stop(0)
{
	
}
// -------------------------------------------------------------------------------------------------------

// Destructor method

ARTDataTrackerModule::~ARTDataTrackerModule()
{
	if (DataTracker)
		delete (DataTracker);
	sources.clear();
}


// This method is called to construct a new Node.

Node * ARTDataTrackerModule::createNode( const std::string& name, StringTable& attributes)
{
    if( name.compare("ARTDataTrackerSource") == 0 )
    { 
        stop = 0;
		int number;
        int num = sscanf(attributes.get("number").c_str(), " %i", &number );
        if( num == 0 )
		{
            std::cout << "Error in converting ARTDataTrackerSource number !" << endl;
            return NULL;
        }
		NodeVector::iterator it;
		for( it = sources.begin(); it != sources.end(); it++ ){
			ARTDataTrackerSource * source = (ARTDataTrackerSource*)(*it);
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
        ARTDataTrackerSource * source = new ARTDataTrackerSource( number); 
		sources.push_back( source );
        std::cout << "Built ARTDataTrackerSource node. Number: " << number << endl;
        return source;
	}
    return NULL;
}

// -------------------------------------------------------------------------------------------------------

void ARTDataTrackerModule::start()
{
	if( isInitialized() && !sources.empty())
        ThreadModule::start();    
}	


// -------------------------------------------------------------------------------------------------------

void ARTDataTrackerModule::close()
{
	lock();
	stop = 1;
	unlock();
}   

// -------------------------------------------------------------------------------------------------------

void ARTDataTrackerModule::run()
{
    ACE_Time_Value timeOut( 1, 0 );
    int retval;
	ACE_INET_Addr addr( port );
	ACE_Addr local( -1 , -1);
	socket = new ACE_SOCK_Dgram( addr );
	
    receiveBufferSize =  3 * sizeof(long) + 100 * sizeof(ARTDataTrackerChomp::BodyRecord) +  100 * sizeof(MarkerRecord);
	receiveBuffer = new char[receiveBufferSize];
    
	std::string receiveString;
	// mainloop for reading data from the port
    
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
		receiveString = std::string(receiveBuffer, retval);
		// call .chomp Method from DataTrackerInstance to bring the received  String into a Record
		DataTracker->chomp(receiveString);
		// brings the Record from the ARTDataTrackerChomp class to the BodyRecordTemp Record
        std::map<int, ARTDataTrackerChomp::BodyRecord> & BodyRecordTemp = DataTracker->getBodyRecord();

		NodeVector::iterator it;
		
		lock();
		for( it = sources.begin(); it != sources.end(); it++)
		{
							
			ARTDataTrackerSource * source = (ARTDataTrackerSource*)(*it);
			bodyID = source->number;
			if( BodyRecordTemp[bodyID].valid == true )
			{
				// convert Data such as quaternion and euler-Angles
				convert( BodyRecordTemp[bodyID] );
				// Brings the locationdata from BodyRecordTemp to  source->state.position !
				source->state.position[0] = BodyRecordTemp[bodyID].position[0];
				source->state.position[1] = BodyRecordTemp[bodyID].position[1];
				source->state.position[2] = BodyRecordTemp[bodyID].position[2];
				// Brings the calculated Quaternion Data from BodyRecordTemp to source->state.orientation !
				source->state.orientation[0] = BodyRecordTemp[bodyID].orientation[0];
				source->state.orientation[1] = BodyRecordTemp[bodyID].orientation[1];
				source->state.orientation[2] = BodyRecordTemp[bodyID].orientation[2];
				source->state.orientation[3] = BodyRecordTemp[bodyID].orientation[3];
				// Bring a timeStamp to source->state
				source->state.timeStamp();
				// Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
				// fixed to 0.5 ??
				source->state.confidence = 0.5;	
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


// -------------------------------------------------------------------------------------------------------

void ARTDataTrackerModule::convert( ARTDataTrackerChomp::BodyRecord & BodyRecordTemp )
{
	//	static const float DEG_TO_RAD = (3.14159/180.0);
	float m[3][3];
	//	set the matrix m with the values from our BodyRecordTemp.rotationMatrix
	//	as discribed in the DTrack Technical Appendix B the matrix is formed like this:
	//	in the ASCII String the rotation Data is like this: [b0 b1 b2 b3 b4 b5 b6 b7 b8]
	//	this formes the Rotation Matrix R:
	//		|b0 b3 b6|
	//	R = |b1 b4 b7|
	//		|b2 b5 b8|
	m[0][0] = BodyRecordTemp.rotationMatrix[0];
	m[0][1] = BodyRecordTemp.rotationMatrix[3];
	m[0][2] = BodyRecordTemp.rotationMatrix[6];
	m[1][0] = BodyRecordTemp.rotationMatrix[1];
	m[1][1] = BodyRecordTemp.rotationMatrix[4];
	m[1][2] = BodyRecordTemp.rotationMatrix[7];
	m[2][0] = BodyRecordTemp.rotationMatrix[2];
	m[2][1] = BodyRecordTemp.rotationMatrix[5];
	m[2][2] = BodyRecordTemp.rotationMatrix[8];
	
	// convert to Quaternion Format result is given back in .orientation Array
	MathUtils::matrixToQuaternion( m, BodyRecordTemp.orientation);
	
	// location
	BodyRecordTemp.position[0] = BodyRecordTemp.location[0];
	BodyRecordTemp.position[1] = BodyRecordTemp.location[1];
	BodyRecordTemp.position[2] = BodyRecordTemp.location[2];
}

// -------------------------------------------------------------------------------------------------------

// pushes events into the tracker tree
void ARTDataTrackerModule::pushState()
{
    if( isInitialized() )
    {
	    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it++ )
	    {
		    ARTDataTrackerSource *source = (ARTDataTrackerSource *) *it;
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

void ARTDataTrackerModule::init(StringTable& attributes, ConfigNode * localTree)
{
	ThreadModule::init( attributes, localTree );
	int num;
    	
	// Scannig port number from XML-File
	num = sscanf(attributes.get("port").c_str(), " %i", &port );
	if( num == 0 )
	{
		port = 12345;
	}
	
	bodyID = 0;		// just to make the compiler happy

	DataTracker = new ARTDataTrackerChomp();
}
