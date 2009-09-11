/* ========================================================================
 * Copyright (c) 2008,
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
/** source file for VideoLocationModule.
 *
 * @author Markus Sareika
 *
 * $Id$
 * @file                                                                   */
/* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section NFTrackerSource
 * @li @c ovSink: name of the video sink which provides the tracking image
 * An example configuration element looks like this :
 * @verbatim
 <NFTrackerConfig camera-calib="calibfile.xyz" /> 
 <NFTrackerSource ovSink="VideoStream" target="multiset/target_*.var" configDb="multiset/multiset.var"/> @endverbatim
 */


#ifndef _NFTRACKERSOURCE_H
#define _NFTRACKERSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_NFTRACKER

#include <ace/OS.h>
#include <NFT3/NFT3_Tracker.h>
#include <StbCore/Image.h>
#include <StbCore/OS.h>
#include <StbCore/Logger.h>
#include <StbCore/FileSystem.h>
#include <StbCore/InFile.h>

#include <StbCore/Window.h>

#include <conio.h>

#if (defined(_DEBUG))
#      pragma comment(lib, "NFT3d.lib")
#    else
#      pragma comment(lib, "NFT3.lib")
#    endif

namespace ot {
 
    class OPENTRACKER_API NFTrackerSource : public Node, public VideoUser
    {
    public:        

		/// the new event
        Event event;
 
		std::string featureset;
		std::string cameraCalib;

    
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator(){ return 1; }
    
        void pushEvent()
		{ 
			updateObservers(event); 
		}
        void pullEvent(){}

		void newVideoFrame(const unsigned char* frameData, int newSizeX, int newSizeY, PIXEL_FORMAT imgFormat, void* trackingData)
		{
			if( !initialized ) init();
			coreImageBuffer->setPixels((void*)frameData, newSizeX, newSizeY, StbCore::PIXEL_FORMAT_BGR);
			process();
			return;
		}

    protected:
	/// protected constructor so it is only accessible by the module
	friend class NFTrackerModule;

	bool initialized;

	NFTrackerSource() : Node()
    { 
		coreImageBuffer = StbCore::Image::create();
		initialized = false;
		noTracking = false;
	}

	bool createTracker()
	{
		StbCV::Camera camera;

		StbCore::StringW fullCamPath = cameraCalib.c_str();
		if(!camera.load(fullCamPath))
		{
			StbCore::Logger::getInstance()->logError("Failed loading camera setting. Can't initialize tracking.");
			return false;
		}
		

		StbCore::StringW featureName = featureset.c_str();
		if(StbCore::InFileAutoPtr file = StbCore::FileSystem::getInstance()->openFileForReading(featureName))
		{
			tracker = new NFT3::Tracker;

			if(!tracker->loadSetup(file))
			{
				StbCore::Logger::getInstance()->logError("Failed loading NFT3 tracking setup.");
				delete tracker;
				tracker = NULL;
				return false;
			}
			tracker->setCamera(camera);
		}
		else
		{
			StbCore::Logger::getInstance()->logError("Application parameter 'featureset' missing. Can't initialize NFT3 tracking.");
			return false;
		}
#ifdef _DEBUG
		//debugWindow.create(640,480);
#endif
		return true;
	}



	void init()
    {
		createTracker();
		initialized = true;
    }

	void process()
    {
		int key(0);
		while( _kbhit() )
        {
            key = _getch();
            if( key == 0 || key == 0xE0 )
            {
                key = 0xE0 << 8;
                key |= _getch();
            }
		}
		if( key == 0xe03f ) //f5
		{
			if(noTracking){
				noTracking=false;
				tracker->clearFlags(NFT3::Tracker::NO_TRACKING);
			}else{
				noTracking=true;
				tracker->setFlags(NFT3::Tracker::NO_TRACKING);
			}
		}

		// convert to lum image

		cameraGray.allocPixels(coreImageBuffer->getWidth(), coreImageBuffer->getHeight());
		const unsigned char* srcPixels = (const unsigned char*)coreImageBuffer->getPixels();
		unsigned char* dstPixels = cameraGray.getPixels();
		int numPixels = coreImageBuffer->getWidth()*coreImageBuffer->getHeight();
		while(--numPixels)
		{
			unsigned int grey = srcPixels[0] + (srcPixels[1]<<1) + srcPixels[2];
			*dstPixels++ = (unsigned char)(grey>>2);
			srcPixels += 3;
		}

		//FILE* fp = fopen("camera.raw", "wb");
		//fwrite(lumBuffer, 1, coreImageBuffer->getWidth()*coreImageBuffer->getHeight(), fp);
		//fclose(fp);


		tracker->update(cameraGray);

		const NFT3::TargetVector& targets = tracker->getTargets();

		if(targets.size()>0)
		{
			if(targets[0].getStatus()!=NFT3::Target::INACTIVE)
			{
				const StbCV::PoseF& pose = targets[0].getPose();

				for(int i(0);i<3;i++)
					event.getPosition()[i] = pose.translation()(i);

				MathUtils::Matrix4x4 mOri =  { {pose.rotation().matrix()(0), pose.rotation().matrix()(1), pose.rotation().matrix()(2), 0},
				{pose.rotation().matrix()(3), pose.rotation().matrix()(4), pose.rotation().matrix()(5), 0},
				{pose.rotation().matrix()(6), pose.rotation().matrix()(7), pose.rotation().matrix()(8), 0},
				{0, 0, 0, 1} };
				double qOriRes[4];
				MathUtils::matrixToQuaternion(mOri, qOriRes);
				for (int i(0);i<4;i++) event.getOrientation()[i]=(float)qOriRes[i];
				event.timeStamp();
			}
		}
		if(noTracking)
		{
			StbCore::String str;
			str = tracker->createDetectionReport();
			logPrintS("%s\n", str.c_str());
		}else{
			StbCore::String str;
			str = tracker->createTrackingReport();
			logPrintS("%s\n", str.c_str());
		}
    }

	private:

	NFT3::Tracker	*tracker;
	StbCore::Image *coreImageBuffer;
	//unsigned char* lumBuffer;
	StbCV::Image cameraGray;
	bool noTracking;

	//StbCV::Window debugWindow;

    };
}  // namespace ot


#endif //USE_NFTRACKER

#endif // _NFTRACKERSOURCE_H

