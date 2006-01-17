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


// this will remove the warning 4786
#include "../tool/disable4786.h"
#include <stdlib.h>

#include "../OpenTracker.h"

#include <ace/Log_Msg.h>
#include "../tool/OT_ACE_Log.h"

#include "ARToolKitPlusModule.h"
#include "ARToolKitSource.h"
#include "ARToolKitMultiMarkerSource.h"


#ifdef USE_ARTOOLKITPLUS

#include <ARToolKitPlus/MemoryManager.h>
#include <ARToolKitPlus/MemoryManagerMemMap.h>


#ifdef ARTOOLKITPLUS_DLL
#  pragma message("compiling against ARToolKitPlus DLL")
#  include <ARToolKitPlus/TrackerSingleMarker.h>
//#  if defined(DEBUG) || defined(_DEBUG)
//#    pragma comment(lib, "ARToolKitPlusDllD.lib")
//#    pragma message("ARToolKitPlusModule: linking against ARToolKitPlusDllD.lib")
//#  else
//#    pragma comment(lib, "ARToolKitPlusDll.lib")
//#    pragma message("ARToolKitPlusModule: linking against ARToolKitPlusDll.lib")
//#  endif
#else
#  include <ARToolKitPlus/TrackerSingleMarkerImpl.h>
#  pragma message("compiling against ARToolKitPlus Lib")
#  if (defined(DEBUG) || defined(_DEBUG))
#    pragma comment(lib, "ARToolKitPlusD.lib")
#    pragma message("ARToolKitPlusModule: linking against ARToolKitPlusD.lib")
#  else
#    pragma comment(lib, "ARToolKitPlus.lib")
#    pragma message("ARToolKitPlusModule: linking against ARToolKitPlus.lib")
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


#ifdef ARTOOLKITPLUS_FOR_STB3

#  include <VidCapture.h>

  // we don't want to include opengl.h here...
#  ifndef GL_RGB
#    define GL_RGB 0x1907
#  endif

#  ifndef GL_RGBA
#    define GL_RGBA 0x1908
#  endif

#  ifndef GL_ABGR
#    define GL_BGRA 0x80e1
#  endif


namespace ot {
	bool capCallback(CVRES status, CVImage* imagePtr, void* userParam);
}

#endif //ARTOOLKITPLUS_FOR_STB3



#include <stdio.h>
#include <stdlib.h>
#include <iostream>


namespace ot {


const char* ImageGrabber::formatStrings[3] = {  "RGBX8888",  "RGB565",  "LUM8"  };



ARToolKitPlusModule::ARToolKitPlusModule() : imageGrabber(NULL), ThreadModule(), NodeFactory()
{
	doBench = false;

	//wparam = NULL;
	sizeX = sizeY = -1;
	flipX = flipY = false;
	idbasedMarkers = false;
	//treshhold = 100;

	trackerNear = 1.0f;
	trackerFar = 1000.0f;

	logger = new ARToolKitPlusModuleLogger;

#ifdef ARTOOLKITPLUS_DLL

#  ifdef ARTOOLKITPLUS_FOR_STB3
	tracker = ARToolKitPlus::createTrackerSingleMarker(320,240, 6,6,6, ARToolKitPlus::PIXEL_FORMAT_RGBA);
#  else
	tracker = ARToolKitPlus::createTrackerSingleMarker(320,240, 6,6,6, ARToolKitPlus::PIXEL_FORMAT_RGB565);
#  endif //ARTOOLKITPLUS_FOR_STB3

#else

#  ifdef ARTOOLKITPLUS_FOR_STB3
	tracker = new ARToolKitPlus::TrackerSingleMarkerImpl<6,6,6, ARToolKitPlus::PIXEL_FORMAT_RGBA>(320,240);
#  else
//	if(ARToolKitPlus::memManager && !ARToolKitPlus::memManager->didInit())
//		ARToolKitPlus::memManager->init(ARToolKitPlus::TrackerSingleMarkerImpl<6,6,6, ARToolKitPlus::PIXEL_FORMAT_RGBA>::getMemoryRequirements());

	tracker = new ARToolKitPlus::TrackerSingleMarkerImpl<6,6,6, ARToolKitPlus::PIXEL_FORMAT_RGB565>(320,240);
#  endif //ARTOOLKITPLUS_FOR_STB3

#endif //ARTOOLKITPLUS_DLL

	tracker->init(NULL, trackerNear, trackerFar, logger);
	tracker->setThreshold(100);
	//tracker.setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

	bestCFs = NULL;
	maxMarkerId = MAX_MARKERID;
	useMarkerDetectLite = false;

	stop = 0;

#ifdef ARTOOLKITPLUS_FOR_STB3
	rate = 30;
	vidCap = NULL;
	curCapImage = NULL;
	videoWidth = videoHeight = 0;
	didLockImage = false;
#endif //ARTOOLKITPLUS_FOR_STB3
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

void ARToolKitPlusModule::init(StringTable& attributes, ConfigNode * localTree)
{
    cameradata = attributes.get("camera-parameter");
    patternDirectory = attributes.get("pattern-dir");

	std::string undistmode = attributes.get("undist-mode");
	std::string detectmode = attributes.get("detect-mode");

	if(detectmode.length() && detectmode=="lite")
		useMarkerDetectLite = true;

	if(undistmode.length())
	{
		if(undistmode=="none" || undistmode=="NONE")
			tracker->setUndistortionMode(ARToolKitPlus::UNDIST_NONE);
		else
		if(undistmode=="lut" || undistmode=="LUT")
			tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
	}

	int tmpThreshold=100;
    
    if( attributes.get("treshhold", &tmpThreshold ) == 1 )
    {
        if( tmpThreshold < 0 )
            tmpThreshold = 0;
        else if( tmpThreshold > 255 )
            tmpThreshold = 255;
    }
	tracker->setThreshold(tmpThreshold);

#ifdef ARTOOLKITPLUS_FOR_STB3
	videomode = attributes.get("videomode");

    if( attributes.get("framerate", &rate) != 1 )
    {
        rate = 0.01;
    } else
    {
        rate = rate / 1000;
    }
#endif

	if( attributes.get("flipX").compare("true") == 0 )
		flipX = true;
	if( attributes.get("flipY").compare("true") == 0 )
		flipY = true;

	if( attributes.get("marker-mode").compare("idbased") == 0 )
		idbasedMarkers = true;

	if( attributes.get("border-width").length()>0 )
	{
		float w = (float)atof(attributes.get("border-width").c_str());
		LOG_ACE_INFO("ot:ARToolkitModule sets border width to %f\n", w);
		tracker->setBorderWidth(w);
	}


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
				LOG_ACE_INFO("ot:ARToolkitModule uses camera-hint for '%s'\n", devName.c_str());
				cameradata = devFile;
				break;
			}
		}

	LOG_ACE_INFO("ot:ARToolkitModule loads camera file %s\n", cameradata.c_str());

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

	if(!tracker->loadCameraFile(cameradata.c_str(), trackerNear, trackerFar))
	{
		LOG_ACE_ERROR("ot:ARToolkitModule error loading camera parameters from %s\n", cameradata.c_str());
        initialized = 0;
        return;
	}

#if defined(ARTOOLKITPLUS_VERSION_MAJOR) && (ARTOOLKITPLUS_VERSION_MAJOR==2)
	tracker->setMarkerMode(idbasedMarkers ? ARToolKitPlus::MARKER_ID_SIMPLE : ARToolKitPlus::MARKER_TEMPLATE);
#else
	tracker->activateIdBasedMarkers(idbasedMarkers);
#endif

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
	if(!initialized || maxMarkerId<0)
		return false;

#ifdef ARTOOLKITPLUS_FOR_STB3
	if(!curCapImage)
		return false;
#endif //ARTOOLKITPLUS_FOR_STB3

	const ARToolKitPlus::ARUint8 * frameData = NULL;
    ARToolKitPlus::ARMarkerInfo * markerInfo;
    int markerNum;
    int j;
    ARFloat matrix[3][4];
	int newSizeX, newSizeY;
	ImageGrabber::FORMAT imgFormat0 = tracker->getPixelFormat()==ARToolKitPlus::PIXEL_FORMAT_RGBA ? ImageGrabber::RGBX8888 : ImageGrabber::RGB565,
						 imgFormat = imgFormat0;

#ifdef ARTOOLKITPLUS_FOR_STB3
	lock();
	newSizeX = curCapImage->Width();
	newSizeY = curCapImage->Height();
	imgFormat = ImageGrabber::RGBX8888;
	frameData = curCapImage->GetRawDataPtr();
#else
	{
		if(!imageGrabber || !imageGrabber->grab(frameData, newSizeX, newSizeY, imgFormat))
			return false;
	}
#endif //ARTOOLKITPLUS_FOR_STB3

	if(imgFormat!=imgFormat0)
	{
		LOG_ACE_ERROR("ot:ARToolkitPlusModule got wrong image format\n");
		LOG_ACE_ERROR("       (%s instead of %s)\n", imgFormat==ImageGrabber::RGBX8888 ? "RGBX888" : "RGB565",
													 imgFormat0==ImageGrabber::RGBX8888 ? "RGBX888" : "RGB565");
#ifdef ARTOOLKITPLUS_FOR_STB3
		unlock();
#endif
		return false;
	}


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
#ifdef ARTOOLKITPLUS_FOR_STB3
		unlock();
#endif
			return false;
		}
	}
	else
	{
		if(tracker->arDetectMarker((ARToolKitPlus::ARUint8*)frameData, tracker->getThreshold(), &markerInfo, &markerNum ) < 0 )
		{
#ifdef ARTOOLKITPLUS_FOR_STB3
		unlock();
#endif
			return false;
		}
	}


    if( markerNum < 1 )
	{
#ifdef ARTOOLKITPLUS_FOR_STB3
		unlock();
#endif
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
				updateSource(sourceA, markerInfo[j].cf, matrix);
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
					updateSource(sourceM, 1.0f, mmConfig->trans);

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


#ifdef ARTOOLKITPLUS_FOR_STB3
	unlock();
#endif
	return true;
}


/*void
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
}*/


void
ARToolKitPlusModule::updateSource(Node *node, float cf, ARFloat matrix[3][4])
{
	ARToolKitSource* source = (ARToolKitSource*)node;

	State & state = source->buffer;
	state.confidence = cf;

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


const char*
ARToolKitPlusModule::getARToolKitPlusDescription() const
{
	return tracker->getDescription();
}



#ifdef ARTOOLKITPLUS_FOR_STB3


void ARToolKitPlusModule::run()
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
            updateARToolKit();
        }
        
        double s = count/rate - ( OSUtils::currentTime() - startTime );
        if( s >= 10 )
        {
            OSUtils::sleep( s );
        }
        count ++;
    }

	shutDownVidCapture();
    LOG_ACE_INFO("ot:ARToolKit Framerate %f\n", 1000 * count / ( OSUtils::currentTime() - startTime ));
}


void
ARToolKitPlusModule::start()
{
    // if we don't have any nodes or are not initialized, forget it
    if( isInitialized() == 0)
    {
        return;
    }

	vidCap = CVPlatform::GetPlatform()->AcquireVideoCapture();

	if(!CVSUCCESS(vidCap->Init()))
	{
		CVPlatform::GetPlatform()->Release(vidCap);
		LOG_ACE_ERROR("ERROR: initializing video capture object failed.\n");
		return;
	}

	LOG_ACE_ERROR("Initialized video capture object.\n");

	if(!CVSUCCESS(vidCap->Connect(0)))
	{
		vidCap->Uninit();
		CVPlatform::GetPlatform()->Release(vidCap);      
		LOG_ACE_ERROR("ERROR: connecting to camera failed.\n");
		return;
	}

	int numDev = 0, useDev = -1;;
	vidCap->GetNumDevices(numDev);

	const char* cfgCamName = "NO_CAMERA_NAME";

	for(int i=0; i<numDev; i++)
	{
		int devNameLen = 0;
		vidCap->GetDeviceName(0,devNameLen);
		devNameLen++;
		char* cameraName = new char[devNameLen];
		vidCap->GetDeviceName(cameraName,devNameLen);

		// TODO: get camera name from config file
		if(!strcmp(cameraName, cfgCamName))
			useDev = i;
		delete cameraName;

		if(i!=0)
			break;
	}

	if(useDev == -1)
	{
		LOG_ACE_ERROR("WARNING: camera '%s' not found. using default camera (0)\n", cfgCamName);
		useDev = 0;
	}


	// find the specified video mode
	//
	int firstSpace = videomode.find(" ");
	int lastSpace = videomode.rfind(" ");

	if(firstSpace!=-1 && lastSpace!=-1)
	{
		std::string widthStr = videomode.substr(0, firstSpace);
		std::string heightStr = videomode.substr(lastSpace, videomode.length());

		videoWidth = atoi(widthStr.c_str());
		videoHeight = atoi(heightStr.c_str());
	}

	if(videoWidth==0 || videoHeight==0)
	{
		videoWidth = 320;
		videoHeight = 240;
	}


	CVVidCapture::VIDCAP_MODE modeInfo;
	int numModes = 0, videoModeId = -1;
	vidCap->GetNumSupportedModes(numModes);

	// Check each mode 
	for(int curmode=0; curmode<numModes; curmode++)
		if(CVSUCCESS(vidCap->GetModeInfo(curmode, modeInfo)))
			if(modeInfo.XRes==videoWidth && modeInfo.YRes==videoHeight)
			{
				videoModeId = curmode;
				break;
			}

	if(videoModeId==-1)
	{
		vidCap->Uninit();
		CVPlatform::GetPlatform()->Release(vidCap);
		LOG_ACE_ERROR("ERROR: could not find specified video mode\n");
		return;
	}


	if(CVFAILED(vidCap->SetMode(videoModeId)))
	{
		vidCap->Uninit();
		CVPlatform::GetPlatform()->Release(vidCap);
		LOG_ACE_ERROR("ERROR: failed to set camera mode.\nIf the application crashes please restart with the next mode!\n");
		return;
	}

	didLockImage = false;

	if(!CVSUCCESS(vidCap->StartImageCap(CVImage::CVIMAGE_RGBX32, ot::capCallback, this)))
	{
		LOG_ACE_ERROR("ERROR: failed to start capturing.\nIf the application crashes please restart with the next mode!\n");
		vidCap->Uninit();
		CVPlatform::GetPlatform()->Release(vidCap);
		return;
	}

    ThreadModule::start();
    LOG_ACE_INFO("ot:ARToolKitPlusModule started\n");
}


void
ARToolKitPlusModule::close()
{
    // if we don't have any nodes or are not initialized, forget it
    if( isInitialized() == 0)
    {
        return;
    }

	shutDownVidCapture();

    lock();
    stop = 1;
    unlock();

    // join the thread to wait for proper cleanup
    ThreadModule::close();

    LOG_ACE_INFO("ot:ARToolkit stopped\n");
}


void
ARToolKitPlusModule::shutDownVidCapture()
{
	if(vidCap)
	{
		vidCap->Stop();
		vidCap->Disconnect();
		vidCap->Uninit();
		CVPlatform::GetPlatform()->Release(vidCap);
		vidCap = NULL;
	}
}


bool
ARToolKitPlusModule::isStereo()
{
	return false;
}


int
ARToolKitPlusModule::getSizeX(int stereo_buffer)
{
	lock();
	int w = curCapImage->Width();
	unlock();

	return w;
}



int 
ARToolKitPlusModule::getSizeY(int stereo_buffer)
{
	lock();
	int h = curCapImage->Height();
	unlock();

	return h;
}


void
ARToolKitPlusModule::getFlipping(bool* isFlippedH, bool* isFlippedV, int stereo_buffer)
{
	*isFlippedH = false;
	*isFlippedV = true;
}


unsigned char *
ARToolKitPlusModule::lockFrame(MemoryBufferHandle* pHandle, int stereo_buffer)
{
	if(!curCapImage)
		return NULL;

	if(curCapImage->Width()!=videoWidth || curCapImage->Height()!=videoHeight)
		return NULL;

	didLockImage = true;
	lock();
	pHandle->n++;
	return curCapImage->GetRawDataPtr();
}


void
ARToolKitPlusModule::unlockFrame(MemoryBufferHandle Handle, int stereo_buffer)
{
	if(didLockImage)
		unlock();
	didLockImage = false;
}


int
ARToolKitPlusModule::getImageFormat(int stereo_buffer)
{
	return GL_RGBA;
}


void
ARToolKitPlusModule::setCapturedImage(CVImage* nImage)
{
	lock();

		if(curCapImage)
			CVImage::ReleaseImage(curCapImage);

		if(nImage)
		{
			curCapImage = nImage;
			curCapImage->AddRef();
		}

	unlock();
}


bool
capCallback(CVRES status, CVImage* imagePtr, void* userParam)
{
	// Only try to work with the image pointer if the
	// status is successful!
	if(CVSUCCESS(status))
	{
		reinterpret_cast<ot::ARToolKitPlusModule*>(userParam)->setCapturedImage(imagePtr);
	}
	else
	{
		printf("ERROR: could not retrieve video image\n");
	}

	return true;
}


#endif //ARTOOLKITPLUS_FOR_STB3


} //namespace ot


#endif //USE_ARTOOLKITPLUS
