/* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for ARToolKit interface module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header1$
  * @file                                                                   */
 /* ======================================================================= */

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

#include "ARToolKitModule.h"
#include "ARToolKitSource.h"

// constructs a new Node

Node * ARToolKitModule::createNode( string& name, StringMap& attributes)
{
    if( name.compare("ARToolKitSource") == 0 )
    {
        double vertex[4][2];
        int num = sscanf( attributes["vertex"].c_str(), " %lf %lf %lf %lf %lf %lf %lf %lf",
                &vertex[0][0], &vertex[0][1],
                &vertex[1][0], &vertex[1][1],
                &vertex[2][0], &vertex[2][1],
                &vertex[3][0], &vertex[3][1] );
        if( num != 8 )
        {
            cout << "Wrong number of vertices : " << num << endl;
            return NULL;
        }
        int id;
        if((id = arLoadPatt((char *)attributes["tag-file"].c_str() )) < 0 )
        {
            cout << "Error reading tag-file " << attributes["tag-file"] << endl;
            return NULL;
        }
        ARToolKitSource * source = new ARToolKitSource( id, vertex );
        sources.push_back( source );
        cout << "Build ARToolKitSource " << attributes["tag-file"] << endl;
        return source;
    }
    return NULL;
}

// opens the artoolkit library and video system

void ARToolKitModule::start()
{
    int sizeX, sizeY;
    ARParam cparam, wparam;
    
    if( sources.size() <= 0 )
        return;
    if( arVideoOpen() < 0 )
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
    
#ifdef __sgi
    arResampleFlag = 1;
#else
    arResampleFlag = 0;
#endif
    arInitCparam( &cparam );
    
#ifdef __sgi
    arVideoStart( did );
#endif                
    
    ThreadModule::start();
    cout << "ARToolKitModule started" << endl;
}

// closes the artoolkit library 

void ARToolKitModule::close()
{
    lock();
    stop = 1;
    unlock();
#ifdef __sgi
    arVideoStop( did );
    arVideoCleanupDevice( did );
#endif
    arVideoClose();
    ThreadModule::close();
}

// pushes events into the tracker tree 

void ARToolKitModule::pushState()
{
//    grab();
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {
        ARToolKitSource * source = (ARToolKitSource *)*it;
        lock();
        if( source->modified == 1 )
        {            
            source->state = source->buffer;
            source->modified = 0;
            unlock();
            source->push();
        } else 
        {
            unlock();
        }
    }
}

// initializes the ARToolKit module

void ARToolKitModule::init(StringMap& attributes, Node * localTree)
{
    ThreadModule::init( attributes, localTree );
    cameradata = attributes["camera-parameter"];

    int num = sscanf(attributes["treshhold"].c_str(), " %i", &treshhold );
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
}


// the work method for the module thread

void ARToolKitModule::run()
{
	// if we have no markers to check for, forget about the whole thing
	if( sources.size() == 0 )
	{
		return;
	}
    
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
    }
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
            if( arGetTransMat( &markerInfo[k], (double(*)[2]) source->vertex, matrix ) >= 0 )
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
                source->modified = 1;
                unlock();
//              cout << "Marker " << (*it)->getMarkerId() << " updated !" << endl;
            }
        }
    } 
}

#endif
