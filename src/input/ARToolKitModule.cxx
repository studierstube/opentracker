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
 * @author Thomas Pintaric, Gerhard Reitmayr
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "ARToolKitModule.h"
#include "ARToolKitSource.h"

#ifdef USE_ARTOOLKIT

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

#ifdef WIN32
#include <windows.h>
#ifdef _DEBUG
#pragma comment(lib,"libARd.lib")
#pragma comment(lib,"libARvideod.lib")
#else
#pragma comment(lib,"libAR.lib")
#pragma comment(lib,"libARvideo.lib")
#endif
#endif

#include <iostream>

#include <string>
#include <stdio.h>
#ifdef __sgi
#include <unistd.h>
#endif

// the following is just to get all the ARToolkit compile options that we need
#include <GL/gl.h>

#include <AR/ar.h>
#include <AR/param.h>
#include <AR/video.h>

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

//using namespace std;

namespace ot {

// constructor

ARToolKitModule::ARToolKitModule() : 
    ThreadModule(), 
    NodeFactory(), 
    treshhold(100), 
    stop(0), 
    stereo(false),
    frame( NULL )
{
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
}

// constructs a new Node

Node * ARToolKitModule::createNode( const std::string& name, StringTable& attributes)
{
    if( name.compare("ARToolKitSource") == 0 )
    {
        double center[2], size;
        int num;
        if( (num = attributes.get("center", center, 2)) != 2 )
        {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:ARToolKit Wrong number of center coordinates : %d\n"), num));
            return NULL;
        }
        if( attributes.get("size", &size) != 1 )
        {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:ARToolKit Not a size\n")));
            return NULL;
        }
        int id;
        std::string filename = attributes.get("tag-file");
        std::string fullname;
        
        if( patternDirectory.compare("") != 0)
            context->addDirectoryFirst( patternDirectory );
        
        if( context->findFile(filename, fullname))
        {
            filename = fullname;
        }
        else
        {
			LOG_ACE_ERROR("ot:ARToolkit could not find tag file %s\n", filename.c_str());
            return NULL;
        }
        
        if( patternDirectory.compare("") != 0)
            context->removeDirectory( patternDirectory );
        
        if((id = arLoadPatt((char *)filename.c_str() )) < 0 )
        {
			LOG_ACE_ERROR("ot:ARToolKit Error reading tag-file %s or %s\n", attributes.get("tag-file").c_str(), filename.c_str());
            return NULL;
        }
        ARToolKitSource * source = new ARToolKitSource( id, center, size );
        sources.push_back( source );
		LOG_ACE_INFO("ot:Build ARToolKitSource %s id %d\n", filename.c_str(), id);
        return source;
    }
    return NULL;
}

// opens the artoolkit library and video system

void ARToolKitModule::start()
{
    // if we don't have any nodes or are not initialized, forget it
    if( isInitialized() == 0)
    {
        return;
    }
    
#ifdef WIN32
	if(stereo)
	{
	    int error_ret = arVideoOpenStereo((char *)videomode.c_str(),
										  (char *)videomode2.c_str());

		if(error_ret != 0)
		{
            LOG_ACE_ERROR("ot:ARToolKitModule Error opening stereo video source(s) !\n");
		    exit(1);
		}
	}
	else
#endif

    if( arVideoOpen((char *) videomode.c_str() ) < 0 )
    {
        LOG_ACE_ERROR("ot:ARToolKitModule Error opening video source !\n");
        exit(1);
    }
    
#ifdef __sgi
    if( arVideoSetupDevice( did, AR_VIDEO_INTERLEAVED, AR_VIDEO_RGB_8, AR_VIDEO_1_P_1 ) < 0 )
    {
        LOG_ACE_ERROR("ot:ARToolKitModule Error setting up video source !\n");
        exit(1);
    }
    if( arVideoInqSize(did, &sizeX, &sizeY ) < 0 )
#else
    if( arVideoInqSize( &sizeX, &sizeY ) < 0 )
#endif
    {
        LOG_ACE_ERROR("ot:ARToolKitModule Error querying video size !\n");
        exit(1);
    }
        
#ifdef __sgi
    arVideoStart( did );
#endif
        
    arVideoCapStart();
    
    ARParam cparam, wparam;
    
    if( patternDirectory.compare("") != 0)
        context->addDirectoryFirst( patternDirectory );
    
	std::string fullname;
    if( context->findFile(cameradata, fullname))
    {
        cameradata = fullname;
    }
    else
    {
        LOG_ACE_ERROR("ot:ARToolkitModule could not find camera parameter file %s\n", cameradata);
        initialized = 0;
        return;
    }
    
    if( patternDirectory.compare("") != 0)
        context->removeDirectory( patternDirectory );
    
    if( arParamLoad((char *)cameradata.c_str(), 1, &wparam ) < 0 )
    {
        LOG_ACE_ERROR("ot:ARToolkitModule error loading camera parameters from %s\n", cameradata);
        initialized = 0;
        return;
    }
    arParamChangeSize( &wparam, sizeX, sizeY, &cparam );
    
    arInitCparam( &cparam );
    
    arParamDisp( &cparam );

    frame = new unsigned char[sizeX*sizeY*AR_PIX_SIZE];
    ThreadModule::start();
    LOG_ACE_INFO("ot:ARToolKitModule started\n");
}

// closes the artoolkit library

void ARToolKitModule::close()
{
    // if we don't have any nodes or are not initialized, forget it
    if( isInitialized() == 0)
    {
        return;
    }
    lock();
    stop = 1;
    unlock();

    // join the thread to wait for proper cleanup
    ThreadModule::close();

    LOG_ACE_INFO("ot:ARToolkit stopped\n");
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
    if( attributes.containsKey("videomode2"))
    {
        videomode2 = attributes.get("videomode2");
        stereo = true;
    }

#ifndef WIN32
    // platform specific videomode override
    if( attributes.containsKey("videolinuxmode"))
    {
        videomode = attributes.get("videolinuxmode");
        if( attributes.containsKey("videolinuxmode2"))
        {
            videomode2 = attributes.get("videolinuxmode2");
            stereo = true;
        }
    }
#endif

}

// the work method for the module thread

void ARToolKitModule::run()
{    
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
        // if we have markers to check for then do some work 
        if( sources.size() != 0 )
        {
            grab();
        }
        
        double s = count/rate - ( OSUtils::currentTime() - startTime );
        if( s >= 10 )
        {
            OSUtils::sleep( s );
        }
        count ++;
    }

    arVideoCapStop();
    arVideoClose();

    LOG_ACE_INFO("ot:ARToolKit Framerate %f\n", 1000 * count / ( OSUtils::currentTime() - startTime ));
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

#ifndef WIN32
    frame = frameData;
#endif

    if( arDetectMarker( frameData, treshhold, &markerInfo, &markerNum ) < 0 )
    {
        arVideoCapNext(); // release buffer
        return;
    }
    if( markerNum < 1 )
    {
        arVideoCapNext(); // release buffer
        return;
    }

    arVideoCapNext();

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
                state.confidence = (float)markerInfo[k].cf;

#ifdef ARTOOLKIT_UNFLIP_V

                //  --- correct ARToolkit's vertical image mirroring ---

                MathUtils::Matrix4x4 matrix_4x4;
                for(int r = 0; r < 3; r ++ )
                    for(int c = 0; c < 4; c ++ )
                        matrix_4x4[r][c] = (float)matrix[r][c];

                matrix_4x4[3][0] = 0; matrix_4x4[3][1] = 0;
                matrix_4x4[3][2] = 0; matrix_4x4[3][3] = 1;

                MathUtils::Matrix4x4 matrix_4x4_corrected;

                // fix translation
                MathUtils::matrixMultiply(MathUtils::matrix4x4_flipY,matrix_4x4,matrix_4x4_corrected);

                MathUtils::Vector3 euler_angles;
                MathUtils::MatrixToEuler(euler_angles,matrix_4x4);

                MathUtils::eulerToQuaternion((float)-euler_angles[Q_Z],(float)euler_angles[Q_Y],(float)-euler_angles[Q_X], state.orientation);

                state.position[0] = (float)matrix_4x4_corrected[0][3];
                state.position[1] = (float)matrix_4x4_corrected[1][3];
                state.position[2] = (float)matrix_4x4_corrected[2][3];
                //  -----------------------------------------------------------
#else
                //  --- DO NOT correct ARToolkit's vertical image mirroring ---

                float m[3][3];
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
                //  -----------------------------------------------------------
#endif
                state.timeStamp();
                source->modified = 1;
                unlock();
            }
        } 
        else  // marker not found 
        {
            // only if marker was found in the last grab (state.confidence > epsilon) set 
            // confidence to 0.0!
            State & state = source->buffer;
            if (state.confidence > 0.00000001f) 
            {
                lock();
                state.confidence = 0.0f;
                state.timeStamp();
                source->modified = 1;
                unlock();
            }
        }
    }
}

// returns whether two cameras are configured

bool ARToolKitModule::isStereo()
{
	return stereo;
}
	
// returns the width of the image in pixels

int ARToolKitModule::getSizeX(int stereo_buffer)
{
#ifdef WIN32
	int width, height;
    lock();
    if(stop != 1)
	    arVideoInqSize(&width,&height, (STEREO_BUFFER) stereo_buffer);
	unlock();
	return width;
#else
	return sizeX;
#endif
}

// returns the height of the image in pixel

int ARToolKitModule::getSizeY(int stereo_buffer)
{
#ifdef WIN32
	int width, height;
    lock();
    if(stop != 1)
	    arVideoInqSize(&width,&height, (STEREO_BUFFER) stereo_buffer);
	unlock();
	return height;
#else
	return sizeY;
#endif
}

// returns whether the grabbed image is flipped horizontally
// or vertically

void ARToolKitModule::getFlipping(bool* isFlippedH, bool* isFlippedV, int stereo_buffer)
{
#ifdef WIN32
    lock();
    if(stop != 1)
		arVideoInqFlipping(isFlippedH,isFlippedV, (STEREO_BUFFER) stereo_buffer);
	unlock();
#endif
}

// returns pointer to the image frame

unsigned char * ARToolKitModule::lockFrame(MemoryBufferHandle* pHandle, int stereo_buffer)
{
    lock();
    if(stop == 1)
    {
        unlock();
        return(NULL);
    }
    else
    {
        unsigned char *pixel_data;
#ifdef WIN32
        pixel_data = arVideoLockBuffer(reinterpret_cast<::MemoryBufferHandle *>(pHandle), (STEREO_BUFFER) stereo_buffer);
#else
        pixel_data = frame;
#endif
        unlock();
        return(pixel_data);
    }
}

void ARToolKitModule::unlockFrame(MemoryBufferHandle Handle, int stereo_buffer)
{
#ifdef WIN32
    lock();
    if(stop != 1)
    {
        ::MemoryBufferHandle handle;
        handle.n = Handle.n;
        handle.t = Handle.t;
        arVideoUnlockBuffer(handle, (STEREO_BUFFER) stereo_buffer);
    }
    unlock();
#endif
}

// returns the OpenGL flag describing the pixel format

int ARToolKitModule::getImageFormat(int stereo_buffer)
{
	// both cameras must use the same pixel format
    return IMAGE_MODE;
}

} // namespace ot

#endif  // USE_ARTOOLKIT
