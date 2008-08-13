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
/** source file for VideoLocationModule module.
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

#include <OpenTracker/input/VideoLocationModule.h>
#include <OpenTracker/input/VideoLocationSource.h>


#ifdef USE_VIDEOLOCATION

#include <OpenTracker/core/Context.h>

namespace ot {
    OT_MODULE_REGISTER_FUNC(VideoLocationModule){
        VideoLocationModule *videoLoc = new VideoLocationModule;
        context->addFactory( * videoLoc );
        context->addModule( "VideoLocationConfig", *videoLoc );
        context->registerVideoUser(videoLoc);
    }

    VideoLocationModule::VideoLocationModule() : NodeFactory()
    {
    }

    VideoLocationModule::~VideoLocationModule()
    {
        sources.clear();
    }

    // This method is called to construct a new Node.
    Node* 
    VideoLocationModule::createNode( const std::string& name, const StringTable& attributes)
    {
        if(name.compare("VideoLocationSource") == 0 )
        {
            if ( !attributes.get("ovSink").empty() )
                openVideoSinkName = attributes.get("ovSink").c_str();
            
            VideoLocationSource* source = new VideoLocationSource();
            
            sources.push_back( source );
            return source;
        }

        return NULL;
    }

    void
    VideoLocationModule::newVideoFrame(const unsigned char* frameData, int newSizeX, int newSizeY, PIXEL_FORMAT imgFormat, void* trackingData)
    {
	    if(!initialized || trackingData==NULL) return;
        
        for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
        {
            VideoLocationSource * source = (VideoLocationSource *)((Node *)*it);
            
            for (int i(0);i<3;i++) source->event.getPosition()[i]=((float*)trackingData)[i];
            for (int i(0);i<4;i++) source->event.getOrientation()[i]=((float*)trackingData)[i+3];
            
            source->event.timeStamp();
            source->modified = 1;
        }

	    return;
    }

    // pushes events into the tracker tree.

    void VideoLocationModule::pushEvent()
    {
        for( NodeVector::iterator it = sources.begin(); it != sources.end(); it ++ )
        {
            VideoLocationSource * source = (VideoLocationSource *)((Node *)*it);

            if( source->modified == 1 )
            {
              source->modified = 0;
              source->updateObservers( source->event );
            }
        }
    }


} //namespace ot


#else
#ifdef WIN32
#pragma message(">>> no VideoLocation support")
#endif
#endif //USE_VIDEOLOCATION

/* 
 * ------------------------------------------------------------
 *   End of VideoLocationModule.cxx
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
