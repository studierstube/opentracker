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
/** source file for ElasticFilter node.
  *
  * @author Flo Ledermann
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "ElasticFilterNode.h"

namespace ot {

// constructor method
ElasticFilterNode::ElasticFilterNode( float force_, float damp_, int frequency_, int offset_ )
   : currentState(), vState()
{
    frequency = frequency_;
    offset = offset_;

    force = force_;
    damp = damp_;

    init = false;
} 

// tests the confidence value and only forwards passing events
void ElasticFilterNode::onEventGenerated( State& event, Node & generator )
{
    //printf("ElasticFilterNode::onEventGenerated() \n");
    if (!init){
        currentState = event;
        init = true;

    }
    targetState = event;
    // we have to pass on the other information as well
    currentState.button = event.button;
    currentState.confidence = event.confidence;
}

void ElasticFilterNode::push() {
    // maybe we should us absolute timing, but for now we try without
    // double curTime = OSUtils::currentTime();
    // double dt = lastTime - curTime;
    if (init) {
        //printf("ElasticFilterNode::push() \n");

        State dState;

        // calculate current offset from target
        dState.position[0] = targetState.position[0] - currentState.position[0];
        dState.position[1] = targetState.position[1] - currentState.position[1];
        dState.position[2] = targetState.position[2] - currentState.position[2];

        // calculate velocity
        vState.position[0] += (float)(dState.position[0] * force * 0.1);
        vState.position[1] += (float)(dState.position[1] * force * 0.1);
        vState.position[2] += (float)(dState.position[2] * force * 0.1);
		
        // damp velocity
        vState.position[0] *= 1.0f - damp;
        vState.position[1] *= 1.0f - damp;
        vState.position[2] *= 1.0f - damp;
     
        // add velocity to current state
        currentState.position[0] += vState.position[0];
        currentState.position[1] += vState.position[1];
        currentState.position[2] += vState.position[2];
				
		MathUtils::slerp(currentState.orientation, targetState.orientation, force*(1-damp)*0.1f, dState.orientation);
		
        for (int i=0; i< 4; i++)
			currentState.orientation[i] = dState.orientation[i]; // copy ??

        currentState.timeStamp();
        updateObservers(currentState);
    }
}

} // namespace ot
