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
/** source file for Filter Node.
  *
  * @author Gerhard Reitmayr
  * @todo check implementation of quaternion interpolation and document
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FilterNode.cxx,v 1.1 2001/10/20 17:20:11 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "FilterNode.h"
#include <math.h>

using namespace std;

// constructor method.

FilterNode::FilterNode( const vector<float> & weights_ )
    : Node(), weights( weights_ )
{
}

int FilterNode::isEventGenerator()
{
	return 1;
}

// this method is called by the EventGenerator to update it's observers.

void FilterNode::onEventGenerated( State& event, Node& generator)
{
    Node * queue = getChild( 0 );
    if( queue != NULL )
    {
        if( queue->getSize() != weights.size())
            return;
        float w,sum = 0;
        vector<float>::iterator it = weights.begin();
        localState.position[0] = 0;
        localState.position[1] = 0;
        localState.position[2] = 0;
        localState.orientation[0] = 0;
        localState.orientation[1] = 0;
        localState.orientation[2] = 0;
        localState.orientation[3] = 0;
        for( ; it != weights.end(); it++ )
        {
            State & state = queue->getEvent( it - weights.begin());
            w = (*it);
            /* The position is computed as the weighted average of the
               positions in the queue. */
            localState.position[0] += state.position[0] * w;
            localState.position[1] += state.position[1] * w;
            localState.position[2] += state.position[2] * w;

            /* The orientation is computed as the normalized weighted average of the
               orientations in the queue in log space. That is they are transformed
               to 3D vectors inside the unit hemisphere and averaged in this linear space.
               The resulting vector is transformed back to a unit quaternion. */
            localState.orientation[0] += state.orientation[0] * state.orientation[3] * w;
            localState.orientation[1] += state.orientation[1] * state.orientation[3] * w;
            localState.orientation[2] += state.orientation[2] * state.orientation[3] * w;
            sum += w;
        }
        w = sqrt((localState.orientation[0]*localState.orientation[0] + localState.orientation[1]*localState.orientation[1] +
            localState.orientation[2]*localState.orientation[2])/(sum*sum));
        localState.orientation[0] *= w;
        localState.orientation[1] *= w;
        localState.orientation[2] *= w;
        localState.orientation[3] = w;
        MathUtils::normalizeQuaternion(  localState.orientation );
        localState.time = event.time;
        updateObservers( localState );
    }
}
