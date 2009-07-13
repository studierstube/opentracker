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

/**
 * @page Nodes Node Reference
 * @section ZoomSinkSource ZoomSinkSource
 * The ZoomSinkSource node is a simple EventGenerator that outputs the
 * current position and button state of the SpaceMouse. It is driven by
 * the @ref ZoomSinkSource. 
 *
 * An example element looks like this :
 * @verbatim
<ZoomSinkSource/>@endverbatim
 */

#ifndef _ZoomSinkSource_H
#define _ZoomSinkSource_H

#include <stdio.h>
#include <queue>


#include "../OpenTracker.h"
#include "../common/ConsoleSource.h"

#ifdef USE_ZOOM

#include "Lanc.h"


namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Sareika
 * @ingroup input
 */
class OPENTRACKER_API ZoomSinkSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    Event event;
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    /** pushes event down the line. */
    void push();
	void pushEvent( );
    void pullEvent( );

	

	/** simple constructor, sets members to initial values */
	ZoomSinkSource() : Node(), 
		publishEvent(false),
		process(false)
	{
		lanc = new Lanc;
		isInit = false;
		lastEvent =0.f;
	}

	~ZoomSinkSource();
	

virtual void onEventGenerated( Event& event, Node& generator);

protected:

	int frequency, delayEvent, delay;

    // flags to indicate whether relativeInput or ptuGoto was changed during processing
    bool publishEvent, process;

	friend class ZoomModule;

private:

	// This Event defines the moving speed
	Event relativeInput;
	Event setZoomFactor;

	Lanc* lanc;

	float lastEvent;
	bool isInit;
};

}  // namespace ot

#endif //USE_ZOOM

#endif //_ZoomSinkSource_H
