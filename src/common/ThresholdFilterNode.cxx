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
/** source file for ThresholdFilter Node.
  *
  * @author Gerhard Reitmayr
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ThresholdFilterNode.cxx,v 1.2 2002/02/05 11:11:53 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "ThresholdFilterNode.h"
#include <math.h>

using namespace std;

// constructor method.

ThresholdFilterNode::ThresholdFilterNode( const float posmin, const float posmax, 
                                          const float rotmin, const float rotmax )
    : Node(), positionMin( posmin ), positionMax( posmax ), 
      rotationMin( rotmin ), rotationMax( rotmax )
{
}

int ThresholdFilterNode::isEventGenerator()
{
	return 1;
}

// this method is called by the EventGenerator to update it's observers.

void ThresholdFilterNode::onEventGenerated( State& event, Node& generator)
{
    float deltaPos = sqrt(
        (event.position[0]-lastState.position[0])*(event.position[0]-lastState.position[0]) +
        (event.position[1]-lastState.position[1])*(event.position[1]-lastState.position[1]) +
        (event.position[2]-lastState.position[2])*(event.position[2]-lastState.position[2]));
    float deltaRot = acos( event.orientation[0]*lastState.orientation[0]+
                     event.orientation[1]*lastState.orientation[1]+
                     event.orientation[2]*lastState.orientation[2]+
                     event.orientation[3]*lastState.orientation[3] );
    if((( positionMin <= deltaPos ) && ( deltaPos <= positionMax )) || 
       (( rotationMin <= deltaRot ) && ( deltaRot <= rotationMax )))
    {
        lastState = event;
        updateObservers( lastState );
    }
}
