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
/** source file for ARToolKitPlusModule module.
  *
  * @author Daniel Wagner
  *
  * $Header$
  * @file                                                                   */
 /* ======================================================================= */


#include "../OpenTracker.h"


#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "ARToolKitPlusModule.h"
#include "ARToolKitSource.h"

#ifdef USE_ARTOOLKITPLUS

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <iostream>



// Destructor method

namespace ot {


ARToolKitPlusModule::ARToolKitPlusModule() : Module(), NodeFactory(), imageGrabber(NULL)
{
	//wparam = NULL;
	sizeX = sizeY = -1;
	flipX = flipY = false;
	idbasedMarkers = false;
	//treshhold = 100;

	trackerNear = 1.0f;
	trackerFar = 1000.0f;
	tracker.init(NULL, trackerNear, trackerFar, this);
	tracker.setThreshold(100);
}


ARToolKitPlusModule::~ARToolKitPlusModule()
{
    sources.clear();
}

// This method is called to construct a new Node.

Node* ARToolKitPlusModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("ARToolKitSource") == 0 )
    {
        double center[2], size;
        int num;

        if( (num = attributes.get("center", center, 2)) != 2 )
        {
            //cout << "ARToolKit Wrong number of center coordinates : " << num << endl;
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:ARToolKit Wrong number of center coordinates : %d\n"), num));
            return NULL;
        }
        if( attributes.get("size", &size) != 1 )
        {
            //cout << "ARToolKit Not a size" << endl;
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:ARToolKit Not a size\n")));
            return NULL;
        }

        int id;
        string filename = attributes.get("tag-file");
		string markerid = attributes.get("tag-id");
        string fullname;

		if(markerid.length())
		{
			id = atoi(markerid.c_str());
		}
		else
		if(filename.length())
		{
			if( patternDirectory.compare("") != 0)
				context->addDirectoryFirst( patternDirectory );
        
			if( context->findFile(filename, fullname))
			{
				filename = fullname;
			}
			else
			{
				//cout << "ARToolkit could not find tag file " << filename << endl;
				LOG_ACE_ERROR("ot:ARToolkit could not find tag file %s\n", filename.c_str());
				return NULL;
			}
        
			if( patternDirectory.compare("") != 0)
				context->removeDirectory( patternDirectory );
        
			if((id = tracker.addPattern((char *)filename.c_str() )) < 0 )
			{
				//cout << "ARToolKit Error reading tag-file " <<
				//    attributes.get("tag-file") << " or " << filename << endl;
				LOG_ACE_ERROR("ot:ARToolKit Error reading tag-file %s or %s\n", attributes.get("tag-file").c_str(), filename.c_str());
				return NULL;
			}
		}
		else
		{

		}

        ARToolKitSource * source = new ARToolKitSource( id, center, size );
        sources.push_back( source );
        //cout << "Build ARToolKitSource " << filename << " id " << id << endl;
		LOG_ACE_INFO("ot:Build ARToolKitSource %s id %d\n", filename.c_str(), id);
        return source;
    }
    return NULL;
}


// initializes the ARToolKit module

void ARToolKitPlusModule::init(StringTable& attributes, ConfigNode * localTree)
{
    cameradata = attributes.get("camera-parameter");
    patternDirectory = attributes.get("pattern-dir");

	int tmpThreshold=100;
    
    if( attributes.get("treshhold", &tmpThreshold ) == 1 )
    {
        if( tmpThreshold < 0 )
            tmpThreshold = 0;
        else if( tmpThreshold > 255 )
            tmpThreshold = 255;
    }
	tracker.setThreshold(tmpThreshold);

	if( attributes.get("flipX").compare("true") == 0 )
		flipX = true;
	if( attributes.get("flipY").compare("true") == 0 )
		flipY = true;

	if( attributes.get("marker-mode").compare("idbased") == 0 )
		idbasedMarkers = true;


    // parsing camera config hints
	if(cameraDeviceHint.length()>0)
		for(unsigned int i = 0; i <localTree->countChildren(); i++)
		{
			ConfigNode * child = (ConfigNode *)localTree->getChild(i);
			StringTable & childAttr = child->getAttributes();

			std::string devName = childAttr.get("device-name"),
						devFile = childAttr.get("camera-parameter");

			if(devName.length()>0 && devFile.length()>0 && devName==cameraDeviceHint)
			{
				cameradata = devFile;
				break;
			}
		}


    if( patternDirectory.compare("") != 0)
        context->addDirectoryFirst( patternDirectory );

    string fullname;
    if( context->findFile(cameradata, fullname))
    {
        cameradata = fullname;
    }
    else
    {
        //cout << "ARToolkitModule could not find camera parameter file " << cameradata << endl;
		LOG_ACE_ERROR("ot:ARToolkitModule could not find camera parameter file %s\n", cameradata.c_str());
        initialized = 0;
        return;
    }

    if( patternDirectory.compare("") != 0)
        context->removeDirectory( patternDirectory );

	if(!tracker.loadCameraFile(cameradata.c_str(), trackerNear, trackerFar))
	{
		LOG_ACE_ERROR("ot:ARToolkitModule error loading camera parameters from %s\n", cameradata.c_str());
        initialized = 0;
        return;
	}

	tracker.activateIdBasedMarkers(idbasedMarkers);

	initialized = 1;
}


void ARToolKitPlusModule::update()
{
	updateARToolKit();
}


// pushes events into the tracker tree.

void ARToolKitPlusModule::pushState()
{
    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    {
        ARToolKitSource * source = (ARToolKitSource *)*it;
        if( source->modified == 1 )
        {
            source->state = source->buffer;
            source->modified = 0;
            source->updateObservers( source->state );
        }
    }
}


bool ARToolKitPlusModule::updateARToolKit()
{
	if(!initialized)
		return false;

	ARToolKitPlus::ARUint8 * frameData = NULL;
    ARToolKitPlus::ARMarkerInfo * markerInfo;
    ARToolKitSource * source;
    int markerNum;
    int j,k;
    ARFloat matrix[3][4];
	int newSizeX, newSizeY;
	ImageGrabber::FORMAT imgFormat0 = ARToolKitPlus::getPixelFormat()==ARToolKitPlus::PIXEL_FORMAT_RGBA ? ImageGrabber::RGBX8888 : ImageGrabber::LUM8,
						 imgFormat = imgFormat0;

	if(!imageGrabber || !imageGrabber->grab(frameData, newSizeX, newSizeY, imgFormat))
		return false;

	if(imgFormat!=imgFormat0)
	{
		LOG_ACE_ERROR("ot:ARToolkitModule got wrong image format\n");
		LOG_ACE_ERROR("       (%s instead of %s)\n", imgFormat==ImageGrabber::RGBX8888 ? "RGBX888" : "LUM",
												 imgFormat0==ImageGrabber::RGBX8888 ? "RGBX888" : "LUM");
		return false;
	}

	// if the image size changed we have to reinitialize
	// some ARToolKit internal stuff...
	if(newSizeX!=sizeX || newSizeY!=sizeY)
	{
		sizeX = newSizeX;
		sizeY = newSizeY;
		tracker.changeCameraSize(newSizeX, newSizeY);
	}

    if(tracker.arDetectMarker( frameData, tracker.getThreshold(), &markerInfo, &markerNum ) < 0 )
        return false;

    if( markerNum < 1 )
        return false;

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
			ARFloat source_center[2], source_size;
			
			source_center[0] = source->center[0];
			source_center[1] = source->center[1];
			source_size = source->size;

            if( tracker.arGetTransMat( &markerInfo[k], source_center, source_size, matrix ) >= 0 )
            {
                State & state = source->buffer;
                state.confidence = markerInfo[k].cf;

#ifdef ARTOOLKIT_UNFLIP_V
  #undef ARTOOLKIT_UNFLIP_V
#endif

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
                
                MathUtils::eulerToQuaternion(-euler_angles[Q_Z],euler_angles[Q_Y],-euler_angles[Q_X], state.orientation);
                
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
            }
        } 
		else  // marker not found 
		{
			// only if marker was found in the last grab (state.confidence > epsilon) set 
			// confidence to 0.0!
			State & state = source->buffer;
			if (state.confidence > 0.00000001f) 
			{
				state.confidence = 0.0f;
				state.timeStamp();
				source->modified = 1;
			}
		}
    }

	return true;
}


void
ARToolKitPlusModule::artLog(const char* nStr)
{
	LOG_ACE_INFO("ot:%s\n", nStr);
}


void
ARToolKitPlusModule::artLogEx(const char* nStr, ...)
{
	char tmpString[512];
    va_list marker;

	va_start(marker, nStr);
	vsprintf(tmpString, nStr, marker);

	artLog(tmpString);
}


} //namespace ot


#endif //USE_ARTOOLKITPLUS
