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
/** header file for Merge Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section merge Merge
 * A Merge node is an EventGenerator node that listens to several other
 * EventGenerator nodes and merges data from these. It has several inputs
 * that are marked with different wrapper tags. It stores an internal state
 * and updates it with parts of the data depending on the type of input. Then
 * it generates an event of its own. Timestamps are treated specially. If no
 * child node is connected to the MergeTime input, then the timestamp of the
 * new event equals the timestamp of the last received event. Otherwise it
 * behaves like other inputs. The following list shows the possible
 * inputs :
 * 
 * @li @c MergePosition only the position data is taken from events received
 *        from children of this wrapper element
 * @li @c MergeOrientation only the orientation data is taken
 * @li @c MergeButton only the button data is taken
 * @li @c MergeConfidence only the confidence data is taken
 * @li @c MergeTime only the time stamp data is taken
 * @li @c MergeDefault any data that is not set by a child of another
 *        wrapper element is used.
 * 
 * An example element using all wrapper elements looks like this :
 * @verbatim
 <Merge>
    <MergeDefault>
        <One or more of any EventGenerator element type>
    </MergeDefault>
    <MergePosition>
        <One or more of any EventGenerator element type>
    </MergePosition>
    <MergeOrientation>
        <One or more of any EventGenerator element type>
    </MergeOrientation>
    <MergeButton>
        <One or more of any EventGenerator element type>
    </MergeButton>
    <MergeConfidence>
        <One or more of any EventGenerator element type>
    </MergeConfidence>
    <MergeTime>
        <One or more of any EventGenerator element type>
    </MergeTime>
 </Merge>@endverbatim
 */

#ifndef _MERGENODE_H
#define _MERGENODE_H

#include "../OpenTracker.h"

/**
 * A MergeNode is an EventGenerator node that observes several marked input
 * nodes and merges the data provided by the input nodes. The mark decides
 * which part of the data is used in the merge. It is implemented using
 * wrapper nodes and apropriate DTD definitions.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API MergeNode : public Node   
{

protected:
    /// State variable to put transformed state into
    State localState;
    
    /** constructor method
     */
    MergeNode()
    {}

public: 

    /** tests for EventGenerator interface being present and returns
     * 1, if present.
     * @return always 1 */
    virtual int isEventGenerator()
    {
    	return 1;
    }

    /**
     * This method is called by any child node. It updates the local
	 * state with the right part of the passed event and notifies
	 * it's parent and references in turn.
	 * @param event the event value passed
	 * @param generator the node generating the event
     */
    virtual void onEventGenerated( State& event, Node & generator);                                

    friend class CommonNodeFactory;
};

} // namespace ot

#endif
