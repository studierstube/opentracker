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
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section spacemousesource SpaceMouseSource
 * The SpaceMouseSource node is a simple EventGenerator that outputs the
 * current position and button state of the SpaceMouse. It is driven by
 * the @ref spacemousemodule. 
 * [Hints to be filled in...]
 *
 * An example element looks like this :
 * @verbatim
<SpaceMouseSource/>@endverbatim
 */

#ifndef _SPACEMOUSESOURCE_H
#define _SPACEMOUSESOURCE_H

#include "../OpenTracker.h"

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
    State state;

// Methods
public:
    /** simple constructor, sets members to initial values */
    SpaceMouseSource() : Node()
    {};
        
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
        state.timeStamp();
        updateObservers( state );
    }

protected:
    /// the state that is updated by the @see JostickModule polling thread.
    State tmpState;
    /// a flag to indicate whether tmpState was changed during processing
    int changed;

	friend class SpaceMouseModule;
};

}  // namespace ot

#endif

#endif
