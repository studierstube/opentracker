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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ARToolKitModule.cxx,v 1.28 2003/04/08 21:17:23 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ARToolKitModule.h"
#include "ARToolKitSource.h"

#ifdef USE_ARTOOLKIT

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>

#include <string>
#include <stdio.h>
#ifdef __sgi
#include <unistd.h>
#endif

// the following is just to get all the ARToolkit compile options that we need
#include <GL/gl.h>
#include <GL/glut.h>

#include <AR/ar.h>
#include <AR/param.h>
#ifdef WIN32
#include <AR/ARFrameGrabber.h>
#else
#include <AR/video.h>
#endif

// definitions used by ARToolKit
#ifndef GL_ABGR_EXT
#define GL_ABGR_EXT GL_ABGR
#endif
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT GL_BGRA
#endif
#ifndef GL_BGR_EXT
#define GL_BGR_EXT GL_BGR
#endif

// to determine the correct GL flag for the image mode
#ifdef  AR_PIX_FORMAT_ABGR
    #define IMAGE_MODE GL_ABGR_EXT
#elif defined(AR_PIX_FORMAT_BGRA)
    #define IMAGE_MODE GL_BGRA_EXT
#elif defined(AR_PIX_FORMAT_BGR)
    #define IMAGE_MODE GL_BGR_EXT
#elif defined(AR_PIX_FORMAT_RGB)
    #define IMAGE_MODE GL_RGB
#endif

using namespace std;

// constructor

ARToolKitModule::ARToolKitModule() : ThreadModule(), NodeFactory(), treshhold(100), stop(0), frame( NULL )
{
#ifdef WIN32
    CoInitialize(NULL);
    camera = new ARFrameGrabber;
#endif
}

// destructor clears any nodes
ARToolKitModule::~ARToolKitModule()
{
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++)
    {
        delete (*it);
    }
    sources.clear();
	if( frame != NULL )
		delete[] frame;
#ifdef WIN32
   	CoUninitialize();
#endif
}

// constructs a new Node

Node * ARToolKitModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("ARToolKitSource") == 0 )
    {
        double center[2], size;
		int num;
        if( (num = attributes.get("center", center, 2)) != 2 )
        {
            cout << "ARToolKit Wrong number of center coordinates : " << num << endl;
            return NULL;
        }
        if( attributes.get("size", &size) != 1 )
        {
            cout << "ARToolKit Not a size" << endl;
            return NULL;
        }
        int id;
		string filename = attributes.get("tag-file");
        string fullname;

        if( patternDirectory.compare("") != 0)
            context->addDirectoryFirst( patternDirectory );
        
        if( context->findFile(filename, fullname))
        {
            filename = fullname;
        }
        else
        {
            cout << "ARToolkit could not find tag file " << filename << endl;
            return NULL;
        }

        if( patternDirectory.compare("") != 0)
            context->removeDirectory( patternDirectory );        

        if((id = arLoadPatt((char *)filename.c_str() )) < 0 )
        {
            cout << "ARToolKit Error reading tag-file " << 
                attributes.get("tag-file") << " or " << filename << endl;
            return NULL;
        }
        ARToolKitSource * source = new ARToolKitSource( id, center, size );
        sources.push_back( source );
        cout << "Build ARToolKitSource " << filename << " id " << id << endl;
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

#ifndef WIN32   // linux and IRIX version
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
   
#ifdef __sgi
    arVideoStart( did );
#endif                

    arVideoCapStart();

#else // WIN32 version using DirectX 8

    //start the video capture
	camera->Init(did, sizeX, sizeY);
	
    sizeX = camera->GetWidth();
    sizeY = camera->GetHeight();
#endif

    ARParam cparam, wparam;    

    if( patternDirectory.compare("") != 0)
        context->addDirectoryFirst( patternDirectory );
    
    string fullname;
    if( context->findFile(cameradata, fullname))
    {
        cameradata = fullname;
    }
    else
    {
        cout << "ARToolkitModule could not find camera parameter file " << cameradata << endl;
        initialized = 0;
        return;
    }

    if( patternDirectory.compare("") != 0)
        context->removeDirectory( patternDirectory );    

    if( arParamLoad((char *)cameradata.c_str(), 1, &wparam ) < 0 )
    {
        cout << "ARToolkitModule error loading camera parameters from " << cameradata << endl;
        initialized = 0;
        return;
    }
    arParamChangeSize( &wparam, sizeX, sizeY, &cparam );
    
    arInitCparam( &cparam );

    frame = new unsigned char[sizeX*sizeY*AR_PIX_SIZE];
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
#ifndef WIN32
    OSUtils::sleep(1000);
    arVideoCapStop();
    arVideoClose();
#else
    delete camera;
#endif    
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
    patternDirectory = attributes.get("pattern-dir");

    if( attributes.get("treshhold", &treshhold ) != 1 )
    {
        treshhold = 100;
    } else
    {
        if( treshhold < 0 )
            treshhold = 0;
        else if( treshhold > 255 )
            treshhold = 255;
    }
    if( attributes.get("framerate", &rate) != 1 )
    {
        rate = 0.01;
    } else 
    {
        rate = rate / 1000;
    }    
    videomode = attributes.get("videomode");

#ifdef WIN32
    int data[3];
    int num = attributes.get("videomode", data, 3);
    if( num >= 1 )    
        did = data[0];
    if( num == 3 )
    {
        sizeX = data[1];
        sizeY = data[2];
    }
    if( videomode.find("vertical") != string::npos )
        camera->SetFlippedImageVertical(true);
    else if( videomode.find("horizontal") != string::npos )
        camera->SetFlippedImageHorizontal(true);
    else if( videomode.find("rotate") != string::npos )
        camera->SetFlippedImage(true);
#endif
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
      
#ifndef WIN32
		arVideoCapNext();
#endif
        double s = count/rate - ( OSUtils::currentTime() - startTime );
        if( s >= 10 )
        {
            OSUtils::sleep( s );
        }
        count ++;
    }
    cout << "ARToolKit Framerate " << 1000 * count / ( OSUtils::currentTime() - startTime ) << endl;

#ifndef WIN32
    arVideoCapStop();
    arVideoClose();
#ifdef __sgi
    arVideoStop( did );
    arVideoCleanupDevice( did );
#endif
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

#ifndef WIN32
#ifdef __sgi
    if((frameData = (ARUint8 *)arVideoGetImage(did)) == NULL )
    {
        sginap(1);
#else
    if((frameData = (ARUint8 *)arVideoGetImage()) == NULL )
    {
#endif
#else 
    camera->GrabFrame();
    if((frameData = (ARUint8 *)camera->GetBuffer()) == NULL ) {
#endif        
        return;
    }

    // copy info to internal pointer
    memcpy( frame, frameData, sizeX*sizeY*AR_PIX_SIZE );
    
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

// returns the width of the image in pixels

int ARToolKitModule::getSizeX()
{
    return sizeX;
}

// returns the height of the image in pixel

int ARToolKitModule::getSizeY()
{
    return sizeY;
}

// returns pointer to the image frame

unsigned char * ARToolKitModule::getFrame()
{
    return frame;
}

// returns the OpenGL flag describing the pixel format

int ARToolKitModule::getImageFormat()
{ 
    return IMAGE_MODE;
}

#endif
