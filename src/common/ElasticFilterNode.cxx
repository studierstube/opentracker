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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ElasticFilterNode.cxx,v 1.3 2003/06/23 08:51:32 tomp Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ElasticFilterNode.h"

// constructor method
ElasticFilterNode::ElasticFilterNode( float force_, float damp_, int frequency_, int offset_ )
{
    frequency = frequency_;
    offset = offset_;

    force = force_;
    damp = damp_;

    init = false;

    State vState;
    State currentState;
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

		// dState.orientation[0] = targetState.orientation[0] - currentState.orientation[0];
        // dState.orientation[1] = targetState.orientation[1] - currentState.orientation[1];
        // dState.orientation[2] = targetState.orientation[2] - currentState.orientation[2];
        // dState.orientation[3] = targetState.orientation[3] - currentState.orientation[3];
		{
        float  currInvQuat[4];   
        MathUtils::invertQuaternion(currentState.orientation, currInvQuat);
		MathUtils::multiplyQuaternion(targetState.orientation, currInvQuat, dState.orientation);
        }

        // calculate velocity
        vState.position[0] += dState.position[0] * force * 0.1;
        vState.position[1] += dState.position[1] * force * 0.1;
        vState.position[2] += dState.position[2] * force * 0.1;
        
        //vState.orientation[0] += dState.orientation[0] * force * 0.1;
        //vState.orientation[1] += dState.orientation[1] * force * 0.1;
        //vState.orientation[2] += dState.orientation[2] * force * 0.1;
        //vState.orientation[3] += dState.orientation[3] * force * 0.1;
        //vQuat = vQuat + dQuat *force*.01;
		
		// TODO could be replaced by scale(force*.01);
		{
		float axisa[4];
		MathUtils::quaternionToAxisAngle(dState.orientation, axisa);
		axisa[3] *= force*.01;
		MathUtils::axisAngleToQuaternion( axisa, dState.orientation);
		}	
		
		MathUtils::multiplyQuaternion( vState.orientation, dState.orientation, vState.orientation);
		
        // damp velocity
        vState.position[0] *= 1.0 - damp;
        vState.position[1] *= 1.0 - damp;
        vState.position[2] *= 1.0 - damp;
        
		//vState.orientation[0] *= 1.0 - damp;
        //vState.orientation[1] *= 1.0 - damp;
        //vState.orientation[2] *= 1.0 - damp;
        //vState.orientation[3] *= 1.0 - damp;
        // vQuat = vQuat* 1.0-damp;
		{
		float axisa[4];
		MathUtils::quaternionToAxisAngle(vState.orientation, axisa);
		axisa[3] *= 1.0-damp;
		MathUtils::axisAngleToQuaternion( axisa, vState.orientation);
		}

        // add velocity to current state
        currentState.position[0] += vState.position[0];
        currentState.position[1] += vState.position[1];
        currentState.position[2] += vState.position[2];
        
        //currentState.orientation[0] += vState.orientation[0];
        //currentState.orientation[1] += vState.orientation[1];
        //currentState.orientation[2] += vState.orientation[2];
        //currentState.orientation[3] += vState.orientation[3];
		// cQuat = cQuat + vQuat;
		
		MathUtils::multiplyQuaternion(currentState.orientation, vState.orientation, currentState.orientation);
		
        MathUtils::normalizeQuaternion( currentState.orientation );

        currentState.timeStamp();
        updateObservers(currentState);
   
    }
}

