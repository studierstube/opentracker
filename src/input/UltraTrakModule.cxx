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
/** source file for UltraTrakModule module.
  *
  * @author Rainer Splechtna
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/UltraTrakModule.cxx,v 1.7 2003/10/31 14:54:22 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

// a trick to avoid warnings when ace includes the STL headers
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <string>

#include <ace/INET_Addr.h>

#include "UltraTrakModule.h"

#include <iostream>

using namespace std;

// constructor initializing the thread manager
UltraTrakModule::UltraTrakModule() : ThreadModule(), NodeFactory(), stop(0)
{
	
}

// destructor cleans up any allocated memory
UltraTrakModule::~UltraTrakModule()
{
	stations.clear();
}

// Converts num floats from Network byte order.
void UltraTrakModule::convertFloatsNToHl(float* floats, float* result, int num)
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
	convert.l = ntohl(convert.l);  // Convert Network to host byte order
	result[i] = convert.f;
    }
}

// reads from the Ultratrak and parses Ultratrak packages
void UltraTrakModule::run()
{
	std::cout << "starting ultratrak module thread" << endl;

	ACE_INET_Addr local((u_short)port);
	ACE_INET_Addr remoteAddr;
	ACE_SOCK_Dgram socket = ACE_SOCK_Dgram(local);
	ACE_Time_Value timeOut( 1, 0 );
    int retval;

    while(1)
    {
        do
        {
            if((retval = socket.recv( &buffer, sizeof( buffer ), remoteAddr, 0,
                    &timeOut )) == -1 )
            {
                if( errno != ETIME && errno != 0 )
                {
                    std::cout << "Error " << errno << " receiving data !" << endl;
                    exit( -1 );
                }
            }    
        } while( retval < 0 && stop == 0);
        if( stop != 0 )
            break;
		
		int frameNum,frameSize,headerSize,recordSize;
		sscanf(buffer.header,"%*s %d %*s %d %*s %d %*s %d\n",
			&frameNum,&frameSize,&headerSize,&recordSize);
		int receivedStations = (frameSize-headerSize)/recordSize;

		for (int i=0; i<receivedStations; i++) 
		{
			//compute station number
			int stationNumber=(buffer.stationData[i].boardNumber-'0')*2 +
				buffer.stationData[i].stationNumber-'1';        

            if( stationNumber >= 0 && stationNumber <= ultraTrakMaxUnits)
            {
				// get source with same stationNumber
                StationVector::iterator station;
                for( station = stations.begin(); 
                     station != stations.end(); station++ )
                {
                    if( (*station)->number == stationNumber )
                        break;
                }

				// if source with same stationNumber exists
                if( station != stations.end())
                {
                    State & state = (*station)->state;
					// critical section start
					lock();                        
					// convert & store position
					memcpy(state.position,buffer.stationData[i].position,3*sizeof(float));
                    //convertFloatsNToHl(state.position,state.position,3);
					correctData(state.position, positionMapping, invertPosition);
					// convert & store orientation
					memcpy( state.orientation, buffer.stationData[i].orientation, 4*sizeof(float));
					//convertFloatsNToHl( state.orientation, state.orientation, 4);
					// UltraTrak delivers quaternions in w,x,y,z format, but
					// OpenTracker supposes x,y,z,w. Change it here ???. 
					/*float help = state.orientation[3];
					for (int j=0; j<3; j++)
						state.orientation[(j+3)%4] = state.orientation[j];
					state.orientation[2] = help;
					*/
					correctData(state.orientation, orientationMapping, invertOrientation);
					
                    (*station)->modified = 1;
                    state.timeStamp();
					// end of critical section
                    unlock();
				}
			}
		}//for all received stations 
	} // forever

    socket.close();
    std::cout << "Stopping thread" << endl;
}
    

//  constructs a new Node
Node * UltraTrakModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("UltraTrakSource") == 0 )
    { 
        int number;
        int num = sscanf(attributes.get("number").c_str(), " %i", &number );
        if( num == 0 ){
            std::cout << "Error in converting UltratrakSource number !" << endl;
            return NULL;
        }

		StationVector::iterator it;
		for( it = stations.begin(); it != stations.end(); it++ )
		{
			Station * station = (Station*)(*it);
			if( station->number == number )
			{
				break;
			}
		}
		if( it != stations.end())
		{
			std::cout << "Source with number "<< number << " exists allready \n";
			return NULL;
		}

		UltraTrakSource * source = new UltraTrakSource; 
		Station *station = new Station(number, source);
		stations.push_back( station );
		std::cout << "Built UltratrakSource node." << endl;

        return source;
    }
    return NULL;
}        


// opens the sockets needed for communication and starts the receive thread
void UltraTrakModule::start()
{
	if (isInitialized() && !stations.empty())
		ThreadModule::start();
}
 
// closes the module and closes any communication sockets and stops thread 
void UltraTrakModule::close()
{
    // stop thread
    lock();
    stop = 1;
    unlock();
}   

// pushes state information into the tree
void UltraTrakModule::pushState()
{
	if (stations.empty())
		return;

	for (StationVector::iterator it = stations.begin();
	it != stations.end(); it ++ )
	{       
		// critical section start
		lock();
		if((*it)->modified == 1 )
		{
			(*it)->source->state = (*it)->state;
			(*it)->modified = 0;
			unlock();
			(*it)->source->updateObservers( (*it)->source->state );
		}
		else
			unlock();
		// end of critical section

	}
}          



int UltraTrakModule::parseVector(const std::string & line, int * val )
{
    int help[3];
    int num;
    num = sscanf( line.c_str()," %d %d %d", &help[0], &help[1], &help[2] );
    if( num != 3 )
    {
        return 1;
    }
	for (int i=0; i<3; i++)
		if (help[i] < 0 || help[i] > 2)
			return 1;

    val[0] = help[0];
    val[1] = help[1];
    val[2] = help[2];
    return 0;
}


void UltraTrakModule::correctData(float* d, int *mapping, int *inversion)
{
    float h[3]; 
    int i;
    for(i=0; i<3; i++) h[i] = d[mapping[i]]*inversion[i];
    for(i=0; i<3; i++) d[i] = h[i];
}


void UltraTrakModule::initMappping(int *mapping)
{
	for (int i=0; i<3; i++)
		mapping[i] = i;
}

void UltraTrakModule::initInversion(int *inversion)
{
	for (int i=0; i<3; i++)
		inversion[i] = 0;
}

void UltraTrakModule::calcInversion(int *inversion)
{
	for (int i=0; i<3; i++)
		inversion[i] = inversion[i] ? -1 : 1;
}


void UltraTrakModule::init(StringTable& attributes, ConfigNode * localTree)
{
	ThreadModule::init( attributes, localTree );
    
	// Scannig port number from XML-File
	int num = sscanf(attributes.get("port").c_str(), " %i", &port );
	if( num == 0 )
	{
		port = 12345;
	}

    if( parseVector(attributes.get("positionMapping"), positionMapping ) != 0 )
	{
            std::cout << "Error parsing positionMapping !" << endl;
			initMappping(positionMapping);
	}
    if( parseVector(attributes.get("orientationMapping"), orientationMapping ) != 0 )
	{
            std::cout << "Error parsing orientationMapping !" << endl;
			initMappping(orientationMapping);
	}
    if( parseVector(attributes.get("invertPosition"), invertPosition ) != 0 )
	{
            std::cout << "Error parsing invertPosition !" << endl;
			initInversion(invertPosition);
	}
	calcInversion(invertPosition);
    if( parseVector(attributes.get("invertOrientation"), invertOrientation ) != 0 )
	{
            std::cout << "Error parsing invertOrientation !" << endl;
			initInversion(invertOrientation);
	}
	calcInversion(invertOrientation);
}
