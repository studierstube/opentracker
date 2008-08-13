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
/** source file for StbTrackerModule module.
 *
 * @author Erick Mendez
 * @author Daniel Wagner
 *
 * @file                                                                   */
/* ======================================================================= */

/* =======================================================================
	What is implemented already:
		Detectors:
			- SimpleID
		Pose Estimators:
			- Null
			- RPP
		Everything else needed for running
	Things left to implement to whoever needs them:
		Detectors:
			- Multimarker
			- BCH
			- Datamatrix
			- Template
			- Split
			- All the "Fixed" versions of the above mentioned
		Pose Estimators:
			- LM
			- RPP Multi
		Filters:
			- DESP
   ======================================================================= */


// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <cstdlib>
#include <algorithm>
#include <cctype>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/OtLogger.h>
#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <OpenTracker/input/StbTrackerModule.h>
#include <OpenTracker/input/StbTrackerMarkerSource.h>


#ifdef USE_STBTRACKER

#include <OpenTracker/core/Context.h>
namespace ot{
OT_MODULE_REGISTER_FUNC(StbTrackerModule){
	        // Create an StbTrackerModule instance
			StbTrackerModule *stbtracker = new StbTrackerModule;
			context->addFactory( * stbtracker );
			context->addModule( "StbTrackerConfig", *stbtracker );
			context->registerVideoUser(stbtracker);
	}
}

#ifdef STBTRACKER_DLL
#  include <StbTracker/TrackerSingleMarker.h>
#  pragma message("compiling against StbTracker DLL")
#  ifndef OT_DO_NOT_LINK_STBTRACKER
#    if (defined(DEBUG) || defined(_DEBUG)) && !defined(STBTRACKER_ALWAYS_RELEASE)
#      pragma comment(lib, "StbTrackerDllD.lib")
#      pragma message("StbTrackerModule: linking against StbTrackerDllD.lib")
#    else
#      pragma comment(lib, "StbTrackerDll.lib")
#      pragma message("StbTrackerModule: linking against StbTrackerDll.lib")
#    endif
#  endif
#else
#  pragma message("compiling against StbTracker Lib")
#  ifndef OT_DO_NOT_LINK_STBTRACKER
#    if (defined(DEBUG) || defined(_DEBUG)) &&  !defined(STBTRACKER_ALWAYS_RELEASE)
#      pragma comment(lib, "StbTrackerD.lib")
#      pragma message("StbTrackerModule: linking against StbTrackerD.lib")
#    else
#      pragma comment(lib, "StbTracker.lib")
#      pragma message("StbTrackerModule: linking against StbTracker.lib")
#    endif
#  endif
#endif

#include <StbTracker/Base/Logger.h>


class StbTrackerModuleLogger : public StbTracker::Logger
{
public:
	void StbTracker_Log(TYPE /*nType*/, const char* nMessage)
	{
		printf("%s\n", nMessage);

		FILE* fp = fopen("log.txt", "a");
		fprintf(fp, "%s\n", nMessage);
		fclose(fp);
	};
};

#include <cstdio>
#include <cstdlib>
#include <iostream>


namespace ot {


bool convertPixelFormat_OpenTracker_to_StbTracker(ot::PIXEL_FORMAT nSrcFormat, StbTracker::PIXEL_FORMAT &nDstFormat)
{
	switch(nSrcFormat)
	{
			case ot::FORMAT_X8B8G8R8:
		nDstFormat = StbTracker::PIXEL_FORMAT_ABGR;
		return true;
			case ot::FORMAT_B8G8R8X8:
		nDstFormat = StbTracker::PIXEL_FORMAT_BGRA;
		return true;
			case ot::FORMAT_B8G8R8:
		nDstFormat = StbTracker::PIXEL_FORMAT_BGR;
		return true;
			case ot::FORMAT_R8G8B8X8:
		nDstFormat = StbTracker::PIXEL_FORMAT_RGBA;
		return true;
			case ot::FORMAT_R8G8B8:
		nDstFormat = StbTracker::PIXEL_FORMAT_RGB;
		return true;
			case ot::FORMAT_R5G6B5:
		nDstFormat = StbTracker::PIXEL_FORMAT_RGB565;
		return true;
			case ot::FORMAT_L8:
		nDstFormat = StbTracker::PIXEL_FORMAT_LUM;
		return true;
			default:
		return false;
	}
}

StbTrackerModule::StbTrackerModule() : ThreadModule(), NodeFactory()
{
	sizeX = sizeY = -1;
	trackerNear = 1.0f;
	trackerFar = 1000.0f;
	stop = 0;

	logger = new StbTrackerModuleLogger;
	tracker = StbTracker::TrackerMain::create(logger);

	// ******* Register a Target Manager **********************
	targetManager = StbTracker::TargetManagerDefault::create();
	tracker->registerFeature(targetManager);

}

StbTrackerModule::~StbTrackerModule()
{
	sources.clear();
	sourcesMap.clear();
	delete tracker;
	delete logger;
}

// This method is called to construct a new Node.
Node* StbTrackerModule::createNode( const std::string& name, const StringTable& attributes)
{
    if(name.compare("StbTrackerMarkerSource")==0) 
    {
        double center[2], size;
        int num;

        if( (num = attributes.get("center", center, 2)) != 2 )
        {
			center[0]=0;
			center[1]=0;
			logPrintD("StbTracker: No 'center' given, assuming 0,0\n");
        }
        if( attributes.get("size", &size) != 1 )
        {
			size=1;
			logPrintD("StbTracker No 'size' given, assuming 1\n");
        }

        int id;
        std::string filename = attributes.get("tag-file");
        std::string markerid = attributes.get("tag-id");
        std::string border = attributes.get("border-width");
        std::string fullname;

        // see if we have a marker-id field
        if(markerid.length())
        {
            id = atoi(markerid.c_str());
			StbTracker::MarkerSimpleId* __marker = StbTracker::MarkerSimpleId::create(id, size);
			targetManager->registerSingleMarkerTarget(markerid.c_str(), __marker);
        }
        else 
		{
			// if(filename.length())
			// TODO: Include all other marker styles here. Mendez 20080311
		}

        StbTrackerMarkerSource* source = new StbTrackerMarkerSource( id, center, size );
        sources.push_back( source );
        sourcesMap.insert(std::make_pair(id, source));

        logPrintI("Built StbTrackerMarkerSource\n");
        return source;
    }

    return NULL;
}



#define MAKE_STRING_LOWER(STR)  std::transform(STR.begin(), STR.end(), STR.begin(), std::tolower);
// initializes the StbTracker module
void StbTrackerModule::init(StringTable& attributes, ConfigNode * localTree)
{
	cameradata = attributes.get("camera-parameter");

	std::string undistmode = attributes.get("undist-mode");
	std::string detectmode = attributes.get("detect-mode");
	std::string posemode = attributes.get("pose-estimator");
	std::string threshold = attributes.get("threshold");
	std::string markermode = attributes.get("marker-mode");
    openVideoSinkName = attributes.get("ovSink");

	MAKE_STRING_LOWER(undistmode);
	MAKE_STRING_LOWER(detectmode);
	MAKE_STRING_LOWER(posemode);
	MAKE_STRING_LOWER(threshold);
	MAKE_STRING_LOWER(markermode);

	// ******* Setting Pose Estimator **********************
	if(posemode=="rpp")
	{
		StbTracker::PoseEstimator* poseEstimator = StbTracker::PoseEstimatorRPPSingle::create();
		tracker->registerFeature(poseEstimator);
        logPrintI("StbTrackerModule using RPP estimator\n");
	}
	else
	{
		StbTracker::PoseEstimator* poseEstimator = StbTracker::PoseEstimatorNull::create();
		tracker->registerFeature(poseEstimator);
		logPrintI("StbTrackerModule using default pose estimator\n");
	}

	// ******* Setting Threshold **********************
	if(threshold == "non-uniform")
	{
		printf(">>>>>>>>>>>>>>Setting Non-uniform\n");
		StbTracker::Thresholder* thresholder = StbTracker::ThresholderNonUniform::create();
		tracker->registerFeature(thresholder);
        logPrintI("StbTrackerModule using Non-Uniform thresholding\n");
	}
	if(threshold == "auto")
	{
		StbTracker::Thresholder* thresholder = StbTracker::ThresholderAuto::create();
		thresholder->setAutoEnabled(true);
		tracker->registerFeature(thresholder);
        logPrintI("StbTrackerModule using auto-thresholding\n");
	}
	else
	{
		int tmpThreshold = atoi(threshold.c_str());
		if( tmpThreshold < 0 ) tmpThreshold = 0;
		else if( tmpThreshold > 255 ) tmpThreshold = 255;

		StbTracker::Thresholder* thresholder = StbTracker::ThresholderAuto::create();
		thresholder->setThreshold(tmpThreshold);
		tracker->registerFeature(thresholder);
        logPrintI("StbTrackerModule using manual thresholding at '%d'\n", tmpThreshold);
	}

	// ******* Setting Fiducial Detector **********************
	StbTracker::FiducialDetectorRect* fiducialDetector = StbTracker::FiducialDetectorRect::create();
	tracker->registerFeature(fiducialDetector);

	// ******* Setting Marker Style **********************
	if(markermode == "idbased")
	{
		StbTracker::MarkerDetectorSimpleId* markerDetector = StbTracker::MarkerDetectorSimpleId::create();
		if(attributes.get("border-width").length()>0)
		{
			float w = (float)atof(attributes.get("border-width").c_str());
			markerDetector->setBorderWidth(w);
			logPrintI("StbTrackerModule using border-width %.3f\n", w);
		}
		else
		{
			markerDetector->setBorderWidth(0.125);
			logPrintI("StbTrackerModule using default border-width 0.125\n");
		}
		tracker->registerFeature(markerDetector);

		logPrintI("StbTrackerModule using id-based markers\n");
	}
	else
	{
		// TODO: Fill here with other marker styles. Mendez 20080311
	}

	// ******* Load Camera File **********************
	camera = StbTracker::Camera::createFromFile(cameradata.c_str());
	if(camera==NULL)
	{
		logPrintE("StbTracker: Failed to load camera file\n");
        initialized = 0;
		return;
	}
	tracker->registerFeature(camera);

	image = StbTracker::Image::create();
	imageGray = NULL;
	initialized = 1;
	logPrintI("StbTrackerModule initialization finished\n");
}


void StbTrackerModule::newVideoFrame(const unsigned char* frameData, int newSizeX, int newSizeY, PIXEL_FORMAT imgFormat, void* /*usrData*/)
{
    if(!initialized) return;

	StbTracker::PIXEL_FORMAT stbtrackerImgFormat;

	// Check if there has been a change on the camera size
	camera->changeFrameSize(StbTracker::Vec2F((float)newSizeX, (float)newSizeY));

	if (!convertPixelFormat_OpenTracker_to_StbTracker(imgFormat,stbtrackerImgFormat))
	{
		logPrintE("Unknown pixel format\n");
		return;
	}

	if(imageGray==NULL)
		imageGray= new unsigned char[newSizeX*newSizeY];

	int numPixels = newSizeX*newSizeY;
	unsigned char* dst = imageGray;
	const unsigned char* src = const_cast<unsigned char*>(frameData);
	while(--numPixels)
	{
		unsigned int grey = src[0] + (src[1]<<1) + src[2];
		*dst++ = (unsigned char)(grey>>2);
		src += 3;
	}

	image->setPixels(const_cast<unsigned char*>(imageGray), newSizeX,newSizeY, StbTracker::PIXEL_FORMAT_LUM);
	tracker->update(image);

	StbTracker::TargetVector& targets = targetManager->getVisibleTargets();

    if( targets.size()< 1 ) return;


    // walk through all found targets
	for(size_t i=0; i<targets.size(); i++)
	{
		// Get the target (remember target!=marker)
		StbTracker::Target* target = targets[i];

		// Check if its a single marker
		if(StbTracker::TargetSingleMarker* _target = StbTracker::Base::cast<StbTracker::TargetSingleMarker>(target))
		{
			// Get the marker associated with the target
			StbTracker::Marker* marker = _target->getMarker();

			// Check if its simple id based
			if(StbTracker::MarkerSimpleId* _marker = StbTracker::Base::cast<StbTracker::MarkerSimpleId>(marker))
			{
				// Find out who is associated with this marker
				MarkerIdMap::iterator it = sourcesMap.find(_marker->getId());
				if(it==sourcesMap.end()) continue;
				Node* source = NULL;
				source = it->second;
				if(source==0) continue;

				// Update it
				StbTracker::Matrix34F matrix;
				_target->getPoseMatrix(matrix,StbTracker::Target::TYPE_RAW);
				float _matrix[3][4];
				memcpy(_matrix, matrix.get(), 12*sizeof(float));

				updateMarkerSource((StbTrackerMarkerSource *)source, _marker->getConfidence(), _matrix);
			}
		}

    }

    return;
}

// pushes events into the tracker tree.
void StbTrackerModule::pushEvent()
{
	for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
	{
		StbTrackerMarkerSource * source = (StbTrackerMarkerSource *)((Node *)*it);
		if( source->modified == 1 )
		{
			source->event = source->buffer;
			source->modified = 0;
			source->updateObservers( source->event );
		}
	}
}

void StbTrackerModule::updateEvent(Event &event, float matrix[3][4])
{
	float m[3][3];
	for( int r = 0; r < 3; r ++ )
	{
		for( int s = 0; s < 3; s ++ )
		{
			m[r][s] = (float)matrix[r][s];
		}
	}
	MathUtils::matrixToQuaternion( m, event.getOrientation() );
	event.getPosition()[0] = (float)matrix[0][3];
	event.getPosition()[1] = (float)matrix[1][3];
	event.getPosition()[2] = (float)matrix[2][3];
	event.timeStamp();
}

void StbTrackerModule::updateMarkerSource(StbTrackerMarkerSource *source, float cf, float matrix[3][4])
{
	Event & event = source->buffer;
	event.setConfidence(cf);
	updateEvent(event,matrix);
	source->modified = 1;
}


bool newVideoFrame(const unsigned char* image, int width, int height, PIXEL_FORMAT format, void *usrData=NULL);


} //namespace ot


#else
#pragma message(">>> no StbTracker support")
#endif //USE_STBTRACKER

/* 
 * ------------------------------------------------------------
 *   End of StbTrackerModule.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
