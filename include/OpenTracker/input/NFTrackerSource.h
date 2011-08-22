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
/** source file for NFT Tracker Module.
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
 <NFTrackerSource ovSink="VideoStream" featureset="target.xml" cameraCalib="calibFile"/> @endverbatim
 */


#ifndef _NFTRACKERSOURCE_H
#define _NFTRACKERSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_NFTRACKER

#include <ace/OS.h>


namespace ot {
 
    class OPENTRACKER_API NFTrackerSource : public Node
    {
    public:        

		/// the new event
        Event event,  ///< the event pushed into the tree
            buffer; ///< buffer variable for grabbing thread
        int modified;  ///< is a new update ready in the buffer ?

    
		std::string targetName;
        /** tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
        virtual int isEventGenerator(){ return 1; }

    protected:
	/// protected constructor so it is only accessible by the module
	friend class NFTrackerModule;

	NFTrackerSource() :  Node(), modified(0)
    { 
	}

    };
}  // namespace ot


#endif //USE_NFTRACKER

#endif // _NFTRACKERSOURCE_H

