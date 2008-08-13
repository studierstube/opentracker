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
 * @section VideoLocationSource
 * @li @c ovSink: name of the video sink which provides the tracking image
 * An example configuration element looks like this :
 * @verbatim
 <VideoLocationConfig /> 
 <VideoLocationSource ovSink="VideoStream"/> @endverbatim
 */


#ifndef _VIDEOLOCATIONSOURCE_H
#define _VIDEOLOCATIONSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_VIDEOLOCATION

namespace ot {
 
    class OPENTRACKER_API VideoLocationSource : public Node 
    {
    public:        

	/// the new event
        Event event;
        bool modified;

    
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator()
        {
            return 1;
        }
    
        void pushEvent(){ updateObservers(event); }
        void pullEvent(){}

    protected:
	/// protected constructor so it is only accessible by the module
	VideoLocationSource() : Node()
            { modified = true; }


        friend class VideoLocationModule;
    };


}  // namespace ot


#endif //USE_VIDEOLOCATION

#endif // _VIDEOLOCATIONSOURCE_H

