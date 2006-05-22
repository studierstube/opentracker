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
/** source file for ARToolKitPlusModule module.
  *
  * @author Daniel Wagner
  * @author Erick Mendez
  *
  * $Header$
  * @file                                                                   */
 /* ======================================================================= */


// this will remove the warning 4786
#include "../tool/disable4786.h"
#include <stdlib.h>
#include <algorithm>
#include <cctype>

#include "../OpenTracker.h"

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

#include "ARToolKitPlusModule.h"
#include "ARToolKitSource.h"
#include "ARToolKitMultiMarkerSource.h"

#ifdef USE_ARTOOLKITPLUS

//#include <ARToolKitPlus/MemoryManager.h>
//#include <ARToolKitPlus/MemoryManagerMemMap.h>


// in SAM we use another mechanism to link against ARToolKitPlus
// in order to use the release DLL even in debug mode.
// for that case we can define OT_DO_NOT_LINK_ARTOOLKITPLUS
// to deactivate the automatic linking process
//
#ifdef ARTOOLKITPLUS_DLL
#  include <ARToolKitPlus/TrackerSingleMarker.h>
#  pragma message("compiling against ARToolKitPlus DLL")
#  ifndef OT_DO_NOT_LINK_ARTOOLKITPLUS
#    if defined(DEBUG) || defined(_DEBUG)
#      pragma comment(lib, "ARToolKitPlusDllD.lib")
#      pragma message("ARToolKitPlusModule: linking against ARToolKitPlusDllD.lib")
#    else
#      pragma comment(lib, "ARToolKitPlusDll.lib")
#      pragma message("ARToolKitPlusModule: linking against ARToolKitPlusDll.lib")
#    endif
#  endif
#else
#  pragma message("compiling against ARToolKitPlus Lib")
#  include <ARToolKitPlus/TrackerSingleMarkerImpl.h>
#  ifndef OT_DO_NOT_LINK_ARTOOLKITPLUS
#    if (defined(DEBUG) || defined(_DEBUG))
#      pragma comment(lib, "ARToolKitPlusD.lib")
#      pragma message("ARToolKitPlusModule: linking against ARToolKitPlusD.lib")
#    else
#      pragma comment(lib, "ARToolKitPlus.lib")
#      pragma message("ARToolKitPlusModule: linking against ARToolKitPlus.lib")
#    endif
#  endif
#endif

#include <ARToolKitPlus/Logger.h>

class ARToolKitPlusModuleLogger : public ARToolKitPlus::Logger
{
	// implement ARToolKitPlus::Logger
	void artLog(const char* nStr)
	{
		LOG_ACE_INFO("ot:%s\n", nStr);
	}

	void artLogEx(const char* nStr, ...)
	{
		char tmpString[512];
		va_list marker;

		va_start(marker, nStr);
		vsprintf(tmpString, nStr, marker);

		artLog(tmpString);
	}
};

#include <stdio.h>
#include <stdlib.h>
#include <iostream>


namespace ot {

// FIXME: 
// The pixel formats of ImageGrabber, ARToolkitPlus and OpenVideo are not coincident 
// therefore translation functions had to be implemented. A common Pixel Format should be used.
// Erick Mendez 20060303
//const char* ImageGrabber::formatStrings[3] = {  "RGBX8888",  "RGB565",  "LUM8"  };


bool convertPixelFormat_ImageGrabber_to_ARToolKitPlus(ImageGrabber::FORMAT nSrcFormat, ARToolKitPlus::PIXEL_FORMAT &nDstFormat)
{
	switch(nSrcFormat)
	{
	case ImageGrabber::XBGR8888:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_ABGR;
		return true;
	case ImageGrabber::BGRX8888:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_BGRA;
		return true;
	case ImageGrabber::BGR888:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_BGR;
		return true;
	case ImageGrabber::RGBX8888:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_RGBA;
		return true;
	case ImageGrabber::RGB888:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_RGB;
		return true;
	case ImageGrabber::RGB565:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_RGB565;
		return true;
	case ImageGrabber::LUM8:
		nDstFormat = ARToolKitPlus::PIXEL_FORMAT_LUM;
		return true;
	default:
		return false;
	}
}

bool convertPixelFormat_ARToolKitPlus_to_ImageGrabber(ARToolKitPlus::PIXEL_FORMAT nSrcFormat, ImageGrabber::FORMAT &nDstFormat)
{
	switch(nSrcFormat)
	{
	case ARToolKitPlus::PIXEL_FORMAT_ABGR:
		nDstFormat = ImageGrabber::XBGR8888;
		return true;
	case ARToolKitPlus::PIXEL_FORMAT_BGRA:
		nDstFormat = ImageGrabber::BGRX8888;
		return true;
	case ARToolKitPlus::PIXEL_FORMAT_BGR:
		nDstFormat = ImageGrabber::BGR888;
		return true;
	case ARToolKitPlus::PIXEL_FORMAT_RGBA:
		nDstFormat = ImageGrabber::RGBX8888;
		return true;
	case ARToolKitPlus::PIXEL_FORMAT_RGB:
		nDstFormat = ImageGrabber::RGB888;
		return true;
	case ARToolKitPlus::PIXEL_FORMAT_RGB565:
		nDstFormat = ImageGrabber::RGB565;
		return true;
	case ARToolKitPlus::PIXEL_FORMAT_LUM:
		nDstFormat = ImageGrabber::LUM8;
		return true;
	default:
		return false;
	}
}


ARToolKitPlusModule::ARToolKitPlusModule() : imageGrabber(NULL), ThreadModule(), NodeFactory()
{
	doBench = false;
	sizeX = sizeY = -1;
	//flipX = flipY = false;

	trackerNear = 1.0f;
	trackerFar = 1000.0f;

	logger = new ARToolKitPlusModuleLogger;


#ifdef ARTOOLKITPLUS_DLL

	tracker = ARToolKitPlus::createTrackerSingleMarker(320,240, 6,6,6);

#else

	// NOTE: if you get an error here complaining about 6 template parameters
	//       then please update to the latest version of ARToolKitPlus
	tracker = new ARToolKitPlus::TrackerSingleMarkerImpl<6,6,6, 32,32>(320,240);

#endif //ARTOOLKITPLUS_DLL


	tracker->init(NULL, trackerNear, trackerFar, logger);
	tracker->setThreshold(100);

	bestCFs = NULL;
	maxMarkerId = MAX_MARKERID;
	useMarkerDetectLite = false;

	stop = 0;
}

ARToolKitPlusModule::~ARToolKitPlusModule()
{
    sources.clear();
	sourcesMap.clear();

	delete tracker;
	delete logger;
}

// This method is called to construct a new Node.

Node* ARToolKitPlusModule::createNode( const std::string& name, StringTable& attributes)
{
    if( name.compare("ARToolKitPlusSource") == 0 )
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
        std::string filename = attributes.get("tag-file");
		std::string markerid = attributes.get("tag-id");
        std::string fullname;

		// see if we have a marker-id field
		if(markerid.length())
		{
			id = atoi(markerid.c_str());
		}
		else
		// otherwise look for a marker-filename
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
				LOG_ACE_ERROR("ot:ARToolkit could not find tag file %s\n", filename.c_str());
				return NULL;
			}

			if( patternDirectory.compare("") != 0)
				context->removeDirectory( patternDirectory );
        
			if((id = tracker->addPattern((char *)filename.c_str() )) < 0 )
			{
				LOG_ACE_ERROR("ot:ARToolKit Error reading tag-file %s or %s\n", attributes.get("tag-file").c_str(), filename.c_str());
				return NULL;
			}
		}
		else
		{
		// not good, this means we have a problem...
		}

		// we need to store the maximum marker id for later...
		if(id>maxMarkerId)
		{
			maxMarkerId = id;
			if(bestCFs)
			{
				delete bestCFs;
				bestCFs = NULL;
			}
		}

        ARToolKitSource * source = new ARToolKitSource( id, center, size );
		source->type = "ARToolKitSource";
        sources.push_back( source );
		sourcesMap.insert(std::make_pair(id, source));

        //cout << "Build ARToolKitSource " << filename << " id " << id << endl;
		LOG_ACE_INFO("ot:Build ARToolKitSource %s id %d\n", filename.c_str(), id);
        return source;
    }

    if( name.compare("ARToolKitPlusMultiMarkerSource") == 0 )
    {
		std::string filename = attributes.get("cfg-file");
        std::string fullname;

		if(patternDirectory.compare("") != 0)
			context->addDirectoryFirst(patternDirectory);

		if(context->findFile(filename, fullname))
			filename = fullname;
		else
		{
			LOG_ACE_ERROR("ot:ARToolkit could not find multi-cfg file %s\n", filename.c_str());
			return NULL;
		}

		ARToolKitPlus::ARMultiMarkerInfoT* mmConfig = tracker->arMultiReadConfigFile(filename.c_str());

		if(mmConfig)
		{
			ARToolKitMultiMarkerSource * source = new ARToolKitMultiMarkerSource( filename, mmConfig );
			source->type = "ARToolKitMultiMarkerSource";
			sources.push_back( source );

			// we store the ids of all markers in this config...
			for(int i=0; i<mmConfig->marker_num; i++)
			{
				int mId = mmConfig->marker[i].patt_id;

				sourcesMap.insert(std::make_pair(mId, source));

				if(mId>maxMarkerId)
					maxMarkerId = mId;
			}

			if(bestCFs)
			{
				delete bestCFs;
				bestCFs = NULL;
			}

			return source;
		}
		else
		{
			LOG_ACE_ERROR("ot:ARToolKit Error reading multi-cfg %s\n");
			return NULL;
		}
	}

    return NULL;
}


// initializes the ARToolKit module

#define MAKE_STRING_LOWER(STR)  std::transform(STR.begin(), STR.end(), STR.begin(), std::tolower);

void ARToolKitPlusModule::init(StringTable& attributes, ConfigNode * localTree)
{
	cameradata = attributes.get("camera-parameter");
    patternDirectory = attributes.get("pattern-dir");

	std::string undistmode = attributes.get("undist-mode");
	std::string detectmode = attributes.get("detect-mode");
	std::string posemode = attributes.get("pose-estimator");
	std::string threshold = attributes.get("treshold");
	std::string markermode = attributes.get("marker-mode");

	MAKE_STRING_LOWER(undistmode);
	MAKE_STRING_LOWER(detectmode);
	MAKE_STRING_LOWER(posemode);
	MAKE_STRING_LOWER(threshold);
	MAKE_STRING_LOWER(markermode);


	// marker detection mode: lite vs. full
	//
	if(useMarkerDetectLite = (detectmode=="lite"))
	{
		LOG_ACE_INFO("ot:ARToolkitModule using marker detect mode 'lite'\n");
	}
	else
	{
		LOG_ACE_INFO("ot:ARToolkitModule using marker detect mode 'normal'\n");
	}


	if(undistmode=="none")
	{
		tracker->setUndistortionMode(ARToolKitPlus::UNDIST_NONE);
		LOG_ACE_INFO("ot:ARToolkitModule lens undistortion disabled\n");
	}
	else if(undistmode=="lut")
	{
		tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
		LOG_ACE_INFO("ot:ARToolkitModule lens undistortion set to lookup-table\n");
	}
	else
	{
		LOG_ACE_INFO("ot:ARToolkitModule using default lens undistortion (this will be slow!)\n");
	}


	// pose estimator settings
	//
	if(posemode=="cont")
	{
		tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_ORIGINAL_CONT);
		LOG_ACE_INFO("ot:ARToolkitModule using CONT pose estimator\n");
	}
	else if(posemode=="rpp")
	{
		tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);
		LOG_ACE_INFO("ot:ARToolkitModule using robust pose estimator\n");
	}
	else
	{
		LOG_ACE_INFO("ot:ARToolkitModule using default pose estimator\n");
	}


	// setting of threshold value
	//
	if(threshold == "auto")
	{
		tracker->activateAutoThreshold(true);
		LOG_ACE_INFO("ot:ARToolkitModule auto-thresholding activated\n");
	}
	else
	{
		int tmpThreshold = atoi(threshold.c_str());

		if( tmpThreshold < 0 )
			tmpThreshold = 0;
		else if( tmpThreshold > 255 )
			tmpThreshold = 255;

		tracker->setThreshold(tmpThreshold);
		LOG_ACE_INFO("ot:ARToolkitModule manual thresholding with '%s'\n", tmpThreshold);
	}



	// setting for id-based vs. template-based markers
	//
	if(markermode == "idbased")
	{
		tracker->setMarkerMode(ARToolKitPlus::MARKER_ID_SIMPLE);
		LOG_ACE_INFO("ot:ARToolkitModule using id-based markers\n");
	}
	else
	if(markermode == "bch")
	{
		tracker->setMarkerMode(ARToolKitPlus::MARKER_ID_BCH);
		LOG_ACE_INFO("ot:ARToolkitModule using BCH markers\n");
	}
	else
	{
		LOG_ACE_INFO("ot:ARToolkitModule using template markers\n");
	}


	// setting for the border width - default value is 0.250
	//
	if(attributes.get("border-width").length()>0)
	{
		float w = (float)atof(attributes.get("border-width").c_str());
		LOG_ACE_INFO("ot:ARToolkitModule using border-width of %.3f\n", w);
		tracker->setBorderWidth(w);
	}
	else
	{
		LOG_ACE_INFO("ot:ARToolkitModule using default border-width of 0.250\n");
	}


    // parsing camera config hints
	//
	if(cameraDeviceHint.length()>0)
		for(unsigned int i = 0; i <localTree->countChildren(); i++)
		{
			ConfigNode * child = (ConfigNode *)localTree->getChild(i);
			StringTable & childAttr = child->getAttributes();

			std::string devName = childAttr.get("device-name"),
						devFile = childAttr.get("camera-parameter");

			if(devName.length()>0 && devFile.length()>0 && devName==cameraDeviceHint)
			{
				LOG_ACE_INFO("ot:ARToolkitModule uses camera-hint for '%s'\n", devName.c_str());
				cameradata = devFile;
				break;
			}
		}

	LOG_ACE_INFO("ot:ARToolkitModule loads camera file %s\n", cameradata.c_str());


	// setting for template pattern folder
	//
    if( patternDirectory.compare("") != 0)
        context->addDirectoryFirst( patternDirectory );

    std::string fullname;
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


	// finally load the camera file
	//
	if(!tracker->loadCameraFile(cameradata.c_str(), trackerNear, trackerFar))
	{
		LOG_ACE_ERROR("ot:ARToolkitModule error loading camera parameters from %s\n", cameradata.c_str());
        initialized = 0;
        return;
	}


	initialized = 1;
	LOG_ACE_INFO("ot:ARToolkitModule initialization finished\n");
}

void ARToolKitPlusModule::update()
{
	updateARToolKit();
}

bool ARToolKitPlusModule::updateARToolKit()
{
	if(!initialized || maxMarkerId<0)
		return false;

	const ARToolKitPlus::ARUint8 * frameData = NULL;
    ARToolKitPlus::ARMarkerInfo * markerInfo;
    int markerNum;
    int j;
    ARFloat matrix[3][4];
	int newSizeX, newSizeY;
	ImageGrabber::FORMAT imgFormat0, imgFormat;

	if(!convertPixelFormat_ARToolKitPlus_to_ImageGrabber(tracker->getPixelFormat(), imgFormat0))
		return false;
	imgFormat = imgFormat0;

	if(!imageGrabber || !imageGrabber->grab(frameData, newSizeX, newSizeY, imgFormat))
		return false;

	// did grab() return another pixel format than ARToolKitPlus expects?
	if(imgFormat!=imgFormat0)
	{
		// ARToolKitPlus 2.1 and later can change pixel format at runtime!
		//
		ARToolKitPlus::PIXEL_FORMAT artkpFormat;
		if(!convertPixelFormat_ImageGrabber_to_ARToolKitPlus(imgFormat, artkpFormat))
			return false;
		tracker->setPixelFormat(artkpFormat);
	}

/*
	if(doBench)
	{
		if(tracker->arDetectMarker((ARToolKitPlus::ARUint8*)frameData, tracker->getThreshold(), &markerInfo, &markerNum ) < 0 )
			return false;

		ARFloat source_center[2], source_size;

		source_center[0] = 0.0f;
		source_center[1] = 0.0f;
		source_size = 80.0f;

		//tracker->arGetTransMat(&markerInfo[markerNum-1], source_center, source_size, matrix);

		for(j=0; j<markerNum; j++)
			tracker->arGetTransMat(&markerInfo[j], source_center, source_size, matrix);

		return true;
	}
*/

	// if the image size changed we have to reinitialize
	// some ARToolKit internal stuff...
	if(newSizeX!=sizeX || newSizeY!=sizeY)
	{
		sizeX = newSizeX;
		sizeY = newSizeY;
		tracker->changeCameraSize(newSizeX, newSizeY);
	}


	// reset all markers from last grab, then clear list
	//
    for(NodeVector::iterator it=visibleMarkers.begin(); it!=visibleMarkers.end(); it++)
	{
	    ARToolKitSource *source = (ARToolKitSource *)*it;

		State & state = source->buffer;
		if (state.confidence > 0.00000001f) 
		{
			state.confidence = 0.0f;
			state.timeStamp();
			source->modified = 1;
		}
	}
	visibleMarkers.clear();

	// try to find markers in the camera image
	//
	if(useMarkerDetectLite)
	{
		if(tracker->arDetectMarkerLite((ARToolKitPlus::ARUint8*)frameData, tracker->getThreshold(), &markerInfo, &markerNum ) < 0 )
		{
			return false;
		}
	}
	else
	{
		if(tracker->arDetectMarker((ARToolKitPlus::ARUint8*)frameData, tracker->getThreshold(), &markerInfo, &markerNum ) < 0 )
		{
			return false;
		}
	}

    if( markerNum < 1 )
	{
        return false;
	}

	// we use an array of best confidences to quickly find
	// the best markers for each id. a simple check against all
	// other visible markers would result in O(n²), now this method 
	// results roughly in 4*O(n) which is better than O(n²) for large n.
	//
	if(!bestCFs)
	{
		bestCFs = new float[maxMarkerId+1];
		for(j=0; j<maxMarkerId; j++)
			bestCFs[j] = 0.0f;
	}


	// store best confidences
	//
	for(j=0; j<markerNum; j++)
	{
		int id = markerInfo[j].id;
		if(id!=-1 && bestCFs[id]<markerInfo[j].cf)
			bestCFs[id] = markerInfo[j].cf;
	}


	// stores all multi.marker sources that have already been processed
	// in order to prevent multiple calculation of the same source due to multiple markers
	NodeVector processedSources;


	// walk through all markers in the image...
	//

	for(j=0; j<markerNum; j++)
	{
		int id = markerInfo[j].id;
		Node* source = NULL;

		if(id==-1)
			continue;

		// only use a marker if it has the best confidence for its id
		//
		if(markerInfo[j].cf<bestCFs[id])
			continue;

		MarkerIdMap::iterator it = sourcesMap.find(id);

		if(it==sourcesMap.end())
			continue;

		source = it->second;
		if(!source)
			continue;

		// we found a "best" marker and its source.
		// everything is fine to process it...

		// store that source for later usage
		//
		visibleMarkers.push_back(source);

		if(source->getType()=="ARToolKitPlusSource")
		{
	        ARToolKitSource *sourceA = (ARToolKitSource*)source;
			ARFloat source_center[2], source_size;
			
			source_center[0] = (ARFloat)sourceA->center[0];
			source_center[1] = (ARFloat)sourceA->center[1];
			source_size = (ARFloat)sourceA->size;

            if(tracker->arGetTransMat(&markerInfo[j], source_center, source_size, matrix)>=0)
				updateSingleMarkerSource(sourceA, markerInfo[j].cf, matrix);
		}
		else
		if(source->getType()=="ARToolKitPlusMultiMarkerSource")
		{
			bool alreadyProcessed = false;

			for(NodeVector::iterator it=processedSources.begin(); it!=processedSources.end(); it++)
				if(*it==source)
				{
					alreadyProcessed = true;
					break;
				}

			if(!alreadyProcessed)
			{
				ARToolKitMultiMarkerSource *sourceM = (ARToolKitMultiMarkerSource*)source;
				ARToolKitPlus::ARMultiMarkerInfoT* mmConfig = (ARToolKitPlus::ARMultiMarkerInfoT*)sourceM->mmConfig;

				if((tracker->arMultiGetTransMat(markerInfo, markerNum, mmConfig))>=0)
					updateMultiMarkerSource(sourceM, 1.0f, mmConfig->trans);

				processedSources.push_back(source);
			}
		}
	}


//	if(doBench)
//	{
//		ARFloat source_center[2] = { 0.0f, 0.0f }, source_size = 80.0f;
//		tracker->arGetTransMat(&markerInfo[0], source_center, source_size, matrix);
//		return true;
//	}


	// reset array of best confidences
	//
	for(j=0; j<markerNum; j++)
		bestCFs[markerInfo[j].id] = 0.0f;

	return true;
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

void ARToolKitPlusModule::updateState(State &state, float matrix[3][4])
{
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
}

void ARToolKitPlusModule::updateSingleMarkerSource(Node *node, float cf, ARFloat matrix[3][4])
{
	ARToolKitSource* source = (ARToolKitSource*)node;

	State & state = source->buffer;
	state.confidence = cf;
	updateState(state,matrix);
	source->modified = 1;
}

void ARToolKitPlusModule::updateMultiMarkerSource(Node *node, float cf, ARFloat matrix[3][4])
{
	ARToolKitMultiMarkerSource* source = (ARToolKitMultiMarkerSource*)node;

	State & state = source->buffer;
	state.confidence = cf;
	updateState(state,matrix);
	source->modified = 1;
}


const char*
ARToolKitPlusModule::getARToolKitPlusDescription() const
{
	const char* descr = tracker->getDescription();
	return descr;
}

} //namespace ot

#endif //USE_ARTOOLKITPLUS
