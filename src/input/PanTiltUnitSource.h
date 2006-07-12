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
/** header file for SpaceMouseSource Node.
  *
  * @author Michael Wögerbauer
  *
  * $Id: SpaceMouseSource.h 900 2006-01-19 16:47:43Z spiczak $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section spacemousesource SpaceMouseSource
 * The SpaceMouseSource node is a simple EventGenerator that outputs the
 * current position and button state of the SpaceMouse. It is driven by
 * the @ref spacemousemodule. 
 *
 * An example element looks like this :
 * @verbatim
<SpaceMouseSource/>@endverbatim
 */

#ifndef _SPACEMOUSESOURCE_H
#define _SPACEMOUSESOURCE_H

#include "../OpenTracker.h"

#include "../common/ConsoleSource.h"


#define OTCOM_NONE				0
#define OTCOM_RESETPOSITION		1
#define OTCOM_RESETROTATION		2
#define OTCOM_TOGGLE_ROTATECAMERAAXIS  3


#ifdef USE_SPACEMOUSE

namespace ot {

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Michael Woegerbauer
 * @ingroup input
 */
class OPENTRACKER_API SpaceMouseSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    Event event;

// Methods
public:
    /** simple constructor, sets members to initial values */
    SpaceMouseSource() : Node()
    {
		// default values
		tWeightDefault = 0.5f;
		rWeightDefault = 0.1f;

		tWeight = tWeightDefault;
		rWeight = rWeightDefault;

		nlDistance = 10;
		gogok = 1.0f;  //1.0f/6.0f;
		//scale = 1.0f;

		useAbsRotation = true;
	};
        
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
	// SpaceMouse Cursor-specific variables
	bool useAbsRotation;

	// User Position
	Event usrpos;
	// This Event holds the current cursor position and is sent to the observers
    Event tmpEvent;

	// Weights
	float tWeight;
	float rWeight;	// weights for translation and rotation
	float tWeightDefault;
	float rWeightDefault;

	bool extNode;

	// scaling factor (extNode)
	//float scale;
	//float cursorDistance;

	// parameters for GoGo Interaction
	float nlDistance;			// distance where non-linear movement starts
    float gogok;				// scaling factor for nonlinear movement acceleration
    
    // A flag to indicate whether tmpState was changed during processing
    int changed;

	friend class SpaceMouseModule;
};

}  // namespace ot

#endif //USE_SPACEMOUSE

#endif //_SPACEMOUSESOURCE_H
