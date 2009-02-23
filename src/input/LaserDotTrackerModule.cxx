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
/** source file for LaserDotTrackerModule module.
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
#include <OpenTracker/tool/OT_ACE_Log.h>

#include <OpenTracker/input/LaserDotTrackerModule.h>
#include <OpenTracker/input/LaserDotTrackerSource.h>


#ifdef USE_LASERDOTTRACKER

#include <OpenTracker/core/Context.h>



namespace ot {
		OT_MODULE_REGISTER_FUNC(LaserDotTrackerModule){
        LaserDotTrackerModule *laserDotTrack = new LaserDotTrackerModule;
        context->addFactory( * laserDotTrack );
        context->addModule( "LaserDotTrackerConfig", *laserDotTrack );
        context->registerVideoUser(laserDotTrack);
    }

    LaserDotTrackerModule::LaserDotTrackerModule() : NodeFactory()
    {
    }

	void LaserDotTrackerModule::init(StringTable& attributes, ConfigNode * localTree)
	{
		//cameraCalib = attributes.get("camera-calib");
	}

    LaserDotTrackerModule::~LaserDotTrackerModule()
    {
        sources.clear();
    }

    // This method is called to construct a new Node.
    Node* 
    LaserDotTrackerModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if(name.compare("LaserDotTrackerSource") == 0 )
        {
			LaserDotTrackerSource* source = new LaserDotTrackerSource();

			if ( !attributes.get("info").empty() )source->info = atoi(attributes.get("info").c_str());
            if ( !attributes.get("ovSink").empty() )
                openVideoSinkName = attributes.get("ovSink").c_str();
			if ( !attributes.get("filter").empty() ) 
			{
				if( attributes.get("filter").compare("R") == 0 ) source->filter = 1;
				if( attributes.get("filter").compare("G") == 0 ) source->filter = 2;
				if( attributes.get("filter").compare("B") == 0 ) source->filter = 3;
				if( attributes.get("filter").compare("TRGB") == 0 ) source->filter = 4;
			}
            if ( !attributes.get("threshold").empty() ) 
                source->threshold = atof(attributes.get("threshold").c_str());
			if ( !attributes.get("thresholdR").empty() ) 
                source->thresholdR = atof(attributes.get("thresholdR").c_str());
			if ( !attributes.get("thresholdG").empty() ) 
                source->thresholdG = atof(attributes.get("thresholdG").c_str());
			if ( !attributes.get("thresholdB").empty() ) 
                source->thresholdB = atof(attributes.get("thresholdB").c_str());
			
            sources.push_back( source );
            return source;
        }

        return NULL;
    }

    void
    LaserDotTrackerModule::newVideoFrame(const unsigned char* frameData, int newSizeX, int newSizeY, PIXEL_FORMAT imgFormat, void* trackingData)
    {
	    if(sources.size() <= 0) return;
        
        for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
        {
            LaserDotTrackerSource * source = (LaserDotTrackerSource *)((Node *)*it);
			if( !source->initialized ) source->init(newSizeX, newSizeY);
			memcpy(source->imgBuffer, frameData, sizeof(char)*newSizeX*newSizeY*3);
			source->process();
        }
	    return;
    }


	void LaserDotTrackerModule::start()
    {
        for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
        {
            LaserDotTrackerSource * source = (LaserDotTrackerSource *)((Node *)*it);
        }
    }

 
    void 
	LaserDotTrackerModule::pushEvent()
    {
    //    for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
    //    {
    //        NFTrackerSource * source = (NFTrackerSource *)((Node *)*it);

    //        if( source->modified == 1 )
    //        {
				//source->lockLoop();
				//source->modified = 0;

				//source->updateObservers( source->event );
				//source->unlockLoop();
    //        }
    //    }
    }
} //namespace ot


#else
#ifdef WIN32
#pragma message(">>> no USE_LASERDOTTRACKER support")
#endif
#endif //USE_LASERDOTTRACKER

/* 
 * ------------------------------------------------------------
 *   End of LaserDotTrackerModule.cxx
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
