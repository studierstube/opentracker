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
  * =======================================================================
  * PROJECT: OpenTracker
  * ======================================================================= */
/** source file for ARToolKit interface module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ARToolKitModule.cxx,v 1.19 2001/11/29 12:48:10 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ARToolKitModule.h"
#include "ARToolKitSource.h"

#ifdef USE_ARTOOLKIT

#include <AR/ar.h>
#include <AR/video.h>
#include <AR/param.h>

#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based iostream library
#else
#include <iostream.h>
#endif
#include <string>
#include <stdio.h>
#ifdef __sgi
#include <unistd.h>
#endif

using namespace std;

// destructor clears any nodes
ARToolKitModule::~ARToolKitModule()
{
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++)
    {
        delete (*it);
    }
    sources.clear();
}

// constructs a new Node

Node * ARToolKitModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("ARToolKitSource") == 0 )
    {
        double center[2], size;
        int num = sscanf( attributes.get("center").c_str(), " %lf %lf",
                &center[0], &center[1]);
        if( num != 2 )
        {
            cout << "ARToolKit Wrong number of center coordinates : " << num << endl;
            return NULL;
        }
        num = sscanf( attributes.get("size").c_str(), " %lf", &size );
        if( num != 1 )
        {
            cout << "ARToolKit Not a size" << endl;
            return NULL;
        }
        int id;
        if((id = arLoadPatt((char *)attributes.get("tag-file").c_str() )) < 0 )
        {
            cout << "ARToolKit Error reading tag-file " << attributes.get("tag-file") << endl;
            return NULL;
        }
        ARToolKitSource * source = new ARToolKitSource( id, center, size );
        sources.push_back( source );
        cout << "Build ARToolKitSource " << attributes.get("tag-file") << endl;
        return source;
    }
    return NULL;
}

// opens the artoolkit library and video system

void ARToolKitModule::start()
{
	// if we don't have any nodes or are not initialized, forget it
	if( sources.size() == 0 || isInitialized() == 0)
	{
		return;
	}
    ARParam cparam, wparam;
    
    if( sources.size() <= 0 )
        return;
    if( arVideoOpen((char *) videomode.c_str() ) < 0 )
    {
        cout << "Error opening video source !" << endl;
        exit(1);
    }
    
#ifdef __sgi
    if( arVideoSetupDevice( did, AR_VIDEO_INTERLEAVED, AR_VIDEO_RGB_8, AR_VIDEO_1_P_1 ) < 0 )
    {
        cout << "Error setting up video source !" << endl;
        exit(1);
    }
    if( arVideoInqSize(did, &sizeX, &sizeY ) < 0 )           
#else
    if( arVideoInqSize( &sizeX, &sizeY ) < 0 )
#endif
    {
        cout << "Error querying video size !" << endl;
        exit(1);
    }
    
    if( arParamLoad((char *)cameradata.c_str(), 1, &wparam ) < 0 )
    {
        cout << "Error loading camera parameters !" << endl;
        exit(1);        
    }
    arParamChangeSize( &wparam, sizeX, sizeY, &cparam );
    
    arInitCparam( &cparam );
    
#ifdef __sgi
    arVideoStart( did );
#endif                

    arVideoCapStart();
    ThreadModule::start();
    cout << "ARToolKitModule started" << endl;
}

// closes the artoolkit library 

void ARToolKitModule::close()
{
	// if we don't have any nodes or are not initialized, forget it
	if( sources.size() == 0 || isInitialized() == 0)
	{
		return;
	}
    lock();
    stop = 1;
    unlock();
    // ThreadModule::close();
    arVideoCapStop();
    arVideoClose();
    cout << "ARToolkit stopped\n";
}

// pushes events into the tracker tree 

void ARToolKitModule::pushState()
{
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {
        ARToolKitSource * source = (ARToolKitSource *)*it;
        lock();
        if( source->modified == 1 )
        {            
            source->state = source->buffer;
            source->modified = 0;
            unlock();
            source->updateObservers( source->state );
        } else 
        {
            unlock();
        }
    }
}

// initializes the ARToolKit module

void ARToolKitModule::init(StringTable& attributes, ConfigNode * localTree)
{
    ThreadModule::init( attributes, localTree );
    cameradata = attributes.get("camera-parameter");

    int num = sscanf(attributes.get("treshhold").c_str(), " %i", &treshhold );
    if( num == 0 )
    {
        treshhold = 100;
    } else
    {
        if( treshhold < 0 )
            treshhold = 0;
        else if( treshhold > 255 )
            treshhold = 255;
    }
    num = sscanf(attributes.get("framerate").c_str(), " %lf", &rate );
    if( num == 0 )
    {
        rate = 0.01;
    } else 
    {
        rate = rate / 1000;
    }    
    videomode = attributes.get("videomode");
}

// the work method for the module thread

void ARToolKitModule::run()
{
	// if we have no markers to check for, forget about the whole thing
	if( sources.size() == 0 )
	{
		return;
	}

    unsigned int count = 0;
    double startTime = OSUtils::currentTime();    
    while(1)
    {
        lock();
        if( stop == 1 )
        {           
            unlock();
            break;
        } else 
        { 
            unlock();
        }
        grab();
      
        double s = count/rate - ( OSUtils::currentTime() - startTime );
        if( s >= 10 )
        {
            OSUtils::sleep( s );
        }
        count ++;
    }
    cout << "ARToolKit Framerate " << 1000 * count / ( OSUtils::currentTime() - startTime ) << endl;
#ifdef __sgi
    arVideoStop( did );
    arVideoCleanupDevice( did );
#endif
}

// grabs a frame and processes the data 
void ARToolKitModule::grab()
{
    ARUint8 * frameData;
    ARMarkerInfo * markerInfo;
    ARToolKitSource * source;
    int markerNum;
    int j,k;    
    double matrix[3][4];
    float m[3][3];
#ifdef __sgi
    if((frameData = (ARUint8 *)arVideoGetImage(did)) == NULL )
    {
        sginap(1);
#else
    if((frameData = (ARUint8 *)arVideoGetImage()) == NULL )
    {
#endif
        return;
    }

    // copy info to internal pointer
    frame = frameData;
    

    if( arDetectMarker( frameData, treshhold, &markerInfo, &markerNum ) < 0 )
    {
        return;
    }
    if( markerNum < 1 )
    {
        return;
    }
        
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {        
        source = (ARToolKitSource *)*it;
        k = -1;
        // searches for the most confident marker matching the source's markerId
        for( j = 0; j < markerNum; j ++ )
        {
            if( source->markerId == markerInfo[j].id )
            {
                if( k == -1 )
                {
                    k = j;
                }
                else
                {
                    if( markerInfo[k].cf < markerInfo[j].cf )
                    {
                        k = j;
                    }
                }
            }
        }
        if( k != -1 )                          
        {                   
            // if one found transform it to coordinates,
            if( arGetTransMat( &markerInfo[k], source->center, source->size, matrix ) >= 0 )
            {
                lock();
                State & state = source->buffer;                    
                state.confidence = markerInfo[k].cf;                    
    		    for( int r = 0; r < 3; r ++ )
        	    {
	                for( int s = 0; s < 3; s ++ )
	                {
    			        m[r][s] = (float)matrix[r][s];
		            }
    		    }
                MathUtils::matrixToQuaternion( m, state.orientation );
                state.position[0] = (float)matrix[0][3];
                state.position[1] = (float)matrix[1][3];
                state.position[2] = (float)matrix[2][3];
                state.timeStamp();
                source->modified = 1;
                unlock();
            }
        }
    } 
}

int ARToolKitModule::getSizeX()
{
    return sizeX;
}

int ARToolKitModule::getSizeY()
{
    return sizeY;
}

unsigned char * ARToolKitModule::getFrame()
{
    return frame;
}
#endif
