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
/** header file for ZoomSinkSource Node.
 *
 * @author Markus Sareika
 *
 * $Id: ZoomSinkSource.h
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/input/ZoomSinkSource.h>
#ifdef USE_ZOOM
#include <cmath>

namespace ot {

    // Process Data received from incomming Events
    void ZoomSinkSource::onEventGenerated( Event& event, Node& generator) 
    {
        static int i=0;

        if (generator.getName().compare("SetZoomFactor") == 0) {
			float zoomFactor;
			zoomFactor = event.getAttribute("zoomFactor", (float)0);
			lanc->adjustToZoomFactor(zoomFactor);
            return;
        }
        // relative input which defines the moving speed of the ptu axis and the zoom
        if (generator.getName().compare("RelativeInput") == 0) {
			lanc->relativeZoom(event.getPosition()[2]);
        }
    }

    void ZoomSinkSource::pushEvent()
    {
        lock();
        if (lanc->emitEvent)
        {
            lanc->emitEvent = false;
            push();
            unlock();
            updateObservers(event);
        }
        else
        {
            unlock();
        }
		
    }
    
    void ZoomSinkSource::pullEvent()
    {
    }

    void ZoomSinkSource::push()
    {
        // put extra attributes into event
        float fov = lanc->getFieldOfView();
        event.setAttribute<float>("fieldOfView", fov);
        float tp = (float)lanc->timePos;
        event.setAttribute<float>("timePos", tp );
        float zf = (float)lanc->zoomFactor;
		//if(!lanc->isShutteling()) 
		event.setAttribute<float>("zoomFactor", zf );
        event.timeStamp();
    }

	
    ZoomSinkSource::~ZoomSinkSource()
    {
       
    }
	
}

#endif //USE_ZOOM
/* 
 * ------------------------------------------------------------
 *   End of ZoomSinkSource.cxx
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
