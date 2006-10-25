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
/** header file for PanTiltUnitSource Node.
  *
  * @author Markus Sareika
  *
  * $Id: PanTiltUnitSource.h
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section PanTiltUnitSource PanTiltUnitSource
 * The PanTiltUnitSource node is a simple EventGenerator that outputs the
 * current position and button state of the SpaceMouse. It is driven by
 * the @ref PanTiltUnitModule. 
 *
 * An example element looks like this :
 * @verbatim
<PanTiltUnitSource/>@endverbatim
 */

#ifndef _PANTILTUNITSOURCE_H
#define _PANTILTUNITSOURCE_H

#include "../OpenTracker.h"
#include "../common/ConsoleSource.h"

#ifdef USE_PANTILTUNIT

#include <PTU.H>
#include <W32SERIA.H>

#pragma comment(lib,"ptu")

//#pragma comment(lib,"lanc32")



namespace ot {

/**
 * This class implements a source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Markus Sareika
 * @ingroup input
 */
class OPENTRACKER_API PanTiltUnitSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    Event event;

    /** simple constructor, sets members to initial values */
	PanTiltUnitSource() : Node(),
		publishEvent(false)
	{
		// default values
	}
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    /** pushes event down the line. */
    void push()
    {
        event.timeStamp();
        updateObservers( event );
    }
	

virtual void onEventGenerated( Event& event, Node& generator);

protected:

	// This Event holds the desired ptu Location
	Event ptuGoto;
	// This Event holds the relativeInput
    Event relativeInput;
	// This Event holds the newEvent
	Event newEvent;

    // A flag to indicate whether relativeInput or ptuGoto was changed during processing
    bool publishEvent;
	
	// The COM port where the PTU is connected
	bool initComPort(int comPort);

	void closeComPort();

	friend class PanTiltUnitModule;

private:

	portstream_fd COMstream;

};

}  // namespace ot

#endif //USE_PANTILTUNIT

#endif //_PANTILTUNITSOURCE_H
