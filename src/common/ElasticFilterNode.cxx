 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
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

#include "ElasticFilterNode.h"

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
