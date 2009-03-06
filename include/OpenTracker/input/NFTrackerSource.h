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
#include <OpenTracker/input/NFTracker.h>
#include <StbCV/Image.h>
#include <StbCore/Image.h>
#include <StbTracker/Util/ImageTool.h>

namespace ot {
 
    class OPENTRACKER_API NFTrackerSource : public Node, public VideoUser
    {
    public:        

		/// the new event
        Event event;
 
		std::string target;
		std::string configDb;
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
		tracker = new NFTracker();
		coreImageBuffer = StbCore::Image::create();
		lumBuffer=NULL;
		initialized = false;
	}

	void init()
    {
		initialized = true;
		tracker->init(target.c_str(), cameraCalib, configDb );
    }

	void process()
    {
		// convert to lum image
		StbTracker::ImageTool *imageTool = new StbTracker::ImageTool(NULL);
		if (lumBuffer == NULL)
			lumBuffer = new unsigned char[coreImageBuffer->getWidth()*coreImageBuffer->getHeight()];
		imageTool->convertImageToLum(coreImageBuffer, lumBuffer, NULL);
		
		StbCV::Image camImage, camImageHalf;
		camImage.setPixels(lumBuffer, coreImageBuffer->getWidth(), coreImageBuffer->getHeight());
		camImageHalf.downSampleToHalfFrom(camImage);
		//camImage.saveRaw("camera_image.raw");
		tracker->update(camImageHalf);
		//////// convert to lum image
		//////StbTracker::ImageTool *imageTool = new StbTracker::ImageTool(NULL);
		//////if (lumBuffer == NULL)
		//////	lumBuffer = new unsigned char[coreImageBuffer->getWidth()*coreImageBuffer->getHeight()];
		//////imageTool->convertImageToLum(coreImageBuffer, lumBuffer, NULL);
		//////
		//////StbCV::Image camImage;
		//////camImage.setPixels(lumBuffer, coreImageBuffer->getWidth(), coreImageBuffer->getHeight());
		////////camImage.saveRaw("camera_image.raw");
		//////tracker->update(camImage);

		const StbCV::PoseF& pose = tracker->getPose();
    
		if( pose.translation()(2)!=0.f )
		{
			for (int i(0);i<3;i++) event.getPosition()[i] = pose.translation()(i);
			
			MathUtils::Matrix4x4 mOri =  {{pose.rotation().matrix()(0), pose.rotation().matrix()(1), pose.rotation().matrix()(2), 0},
										  {pose.rotation().matrix()(3), pose.rotation().matrix()(4), pose.rotation().matrix()(5), 0},
										  {pose.rotation().matrix()(6), pose.rotation().matrix()(7), pose.rotation().matrix()(8), 0},
										  {0, 0, 0, 1}};
			double qOriRes[4];
			MathUtils::matrixToQuaternion(mOri, qOriRes);
			for (int i(0);i<4;i++) event.getOrientation()[i]=(float)qOriRes[i];
			event.timeStamp();
		}
    }

	private:

	NFTracker	*tracker;
	StbCore::Image *coreImageBuffer;
	unsigned char* lumBuffer;

    };
}  // namespace ot


#endif //USE_NFTRACKER

#endif // _NFTRACKERSOURCE_H

