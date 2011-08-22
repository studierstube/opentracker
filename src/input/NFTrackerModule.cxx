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
/** source file for NFTrackerModule module.
 *
 * @author Markus Sareika
 *
 * $Header$
 * @file                                                                   */
/* ======================================================================= */


#include <OpenTracker/tool/disable4786.h>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cctype>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/OtLogger.h>

#ifdef USE_NFTRACKER

#include <OpenTracker/input/NFTrackerModule.h>
#include <OpenTracker/input/NFTrackerSource.h>


#include <OpenTracker/core/Context.h>




namespace ot {
    OT_MODULE_REGISTER_FUNC(NFTrackerModule){
        NFTrackerModule *nftTrack = new NFTrackerModule;
        context->addFactory( * nftTrack );
        context->addModule( "NFTrackerConfig", *nftTrack );
		context->registerVideoUser(nftTrack);
    }

    NFTrackerModule::NFTrackerModule() : NodeFactory()
    {
		coreImageBuffer = StbCore::Image::create();
		initialized = false;
		noTracking = false;
    }

	void NFTrackerModule::init(StringTable& attributes, ConfigNode * localTree)
	{
		featureset = attributes.get("featureset");
		cameraCalib = attributes.get("cameraCalib");
		openVideoSinkName = attributes.get("ovSink");
	}

    NFTrackerModule::~NFTrackerModule()
    {
        sources.clear();
    }

    // This method is called to construct a new Node.
    Node* 
	NFTrackerModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if(name.compare("NFTrackerSource") == 0 )
        {
			NFTrackerSource* source = new NFTrackerSource();

            if ( !attributes.get("targetName").empty() )
                source->targetName = attributes.get("targetName").c_str();
  
            sources.push_back( source );
            return source;
        }

        return NULL;
    }

	void NFTrackerModule::createTracker()
	{
		StbCV::Camera camera;

		StbCore::StringW fullCamPath = cameraCalib.c_str();
		if(!camera.load(fullCamPath))
		{
			StbCore::Logger::getInstance()->logError("Failed loading camera setting. Can't initialize tracking.");
			return;
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
				return;
			}
			tracker->setCamera(camera);
		}
		else
		{
			StbCore::Logger::getInstance()->logError("Application parameter 'featureset' missing. Can't initialize NFT3 tracking.");
			return;
		}
#ifdef _DEBUG
		//debugWindow.create(640,480);
#endif
		initialized = true;
	}

	void NFTrackerModule::newVideoFrame(const unsigned char* frameData, int newSizeX, int newSizeY, PIXEL_FORMAT imgFormat, void* trackingData)
	{
		if( !initialized ) createTracker();
		coreImageBuffer->setPixels((void*)frameData, newSizeX, newSizeY, StbCore::PIXEL_FORMAT_BGR);
		process();
		return;
	}

	void NFTrackerModule::process()
    {
  //  	int key(0);
		//while( _kbhit() )
  //      {
  //          key = _getch();
  //          if( key == 0 || key == 0xE0 )
  //          {
  //              key = 0xE0 << 8;
  //              key |= _getch();
  //          }
		//}
		//if( key == 0xe03f ) //f5
		//{
		//	if(noTracking){
		//		noTracking=false;
		//		tracker->clearFlags(NFT3::Tracker::NO_TRACKING);
		//	}else{
		//		noTracking=true;
		//		tracker->setFlags(NFT3::Tracker::NO_TRACKING);
		//	}
		//}

		// convert to lum image


        static bool initAlloc = false;
        if(!initAlloc)
        {
		    cameraGray.allocPixels(coreImageBuffer->getWidth(), coreImageBuffer->getHeight());
            initAlloc = true;
        }
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

        // reset all markers from last grab, then clear list
        //
        for(NodeVector::iterator it=visibleMarkers.begin(); it!=visibleMarkers.end(); it++)
        {
            Node* source = ((Node *)*it);

            //if(source->getType()=="ARToolKitPlusSource")
            //{
            NFTrackerSource *smSource = (NFTrackerSource *)source;

            Event & event = smSource->buffer;
            /*if (event.getConfidence() > 0.00000001f) 
            {*/
                event.getConfidence() = 0.0f;
                event.timeStamp();
                smSource->modified = 1;
            //}
        }
        visibleMarkers.clear();



		const NFT3::TargetVector& targets = tracker->getTargets();

		if(targets.size()>0)
		{
			for(int i(0); i<targets.size(); i++)
			{
				if(targets[i].getStatus()!=NFT3::Target::INACTIVE)
				{
					const StbCV::PoseF& pose1 = targets[i].getPose();
                    StbCV::PoseF pose = pose1;
                    pose.translation()(0) = pose1.translation()(0);
                    pose.translation()(1) = -1.0f * pose1.translation()(1);
                    pose.translation()(2) = -1.0f * pose1.translation()(2); 

          

					for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
					{
						NFTrackerSource * source = (NFTrackerSource *)((Node *)*it);
					
						if( !(source->targetName).compare(targets[i].getName().c_str()) )
						{
                            Event &event = source->buffer;
                            
              
							for(int i(0);i<3;i++)
								event.getPosition()[i] = pose.translation()(i);
							MathUtils::Matrix4x4 mOri =  { {pose.rotation().matrix()(0), pose.rotation().matrix()(1), pose.rotation().matrix()(2), 0},
							{-pose.rotation().matrix()(3),-pose.rotation().matrix()(4),-pose.rotation().matrix()(5), 0},
							{-pose.rotation().matrix()(6),-pose.rotation().matrix()(7),-pose.rotation().matrix()(8), 0},
							{0, 0, 0, 1} };

         
              
							double qOriRes1[4];
                            double qOriRes[4] = {0,0,0,1};

							MathUtils::matrixToQuaternion(mOri, qOriRes1);
              
              
							for (int i(0);i<4;i++) event.getOrientation()[i]=(float)qOriRes1[i];

                            event.timeStamp();
                            source->modified = 1;
                            
							//source->event.timeStamp();
							//source->newEvent=true;
						}
					}
				}
			}
		}


		//if(noTracking)
		//{
		//	StbCore::String str;
		//	str = tracker->createDetectionReport();
		//	logPrintS("%s\n", str.c_str());
		//}else{
		//	StbCore::String str;
		//	str = tracker->createTrackingReport();
		//	logPrintS("%s\n", str.c_str());
		//}
    }


	void NFTrackerModule::start()
    {
        for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
        {
            NFTrackerSource * source = (NFTrackerSource *)((Node *)*it);
        }
    }

    void NFTrackerModule::pushEvent()
    {
        for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
        {
            NFTrackerSource * source = (NFTrackerSource *)((Node *)*it);
            //if( TRUE ) 
            if( source->modified == 1 )
            {
                source->event = source->buffer;
                source->modified = 0;
                source->updateObservers( source->event );
            }
        }
    }
 
 //   void 
	//NFTrackerModule::pushEvent()
 //   {
 //       for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
 //       {
 //           NFTrackerSource * source = (NFTrackerSource *)((Node *)*it);

 //           if( source->modified == 1 )
 //           {
	//			source->lockLoop();
	//			source->modified = 0;

	//			source->updateObservers( source->event );
	//			source->unlockLoop();
 //           }
 //       }

 //     
 //   }
} //namespace ot


#else
#ifdef WIN32
#pragma message(">>> no NFTracker support")
#endif
#endif //USE_NFTRACKER

/* 
 * ------------------------------------------------------------
 *   End of NFTrackerModule.cxx
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
