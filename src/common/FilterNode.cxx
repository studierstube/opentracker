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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/FilterNode.cxx,v 1.2 2001/10/21 22:10:56 reitmayr Exp $
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
        double w,sum = 0;
        double pos[3] = {0, 0, 0 }, orient[3] = { 0, 0, 0 };
        vector<float>::iterator it = weights.begin();
  /*      localState.position[0] = 0;
        localState.position[1] = 0;
        localState.position[2] = 0;
        localState.orientation[0] = 0;
        localState.orientation[1] = 0;
        localState.orientation[2] = 0;
        localState.orientation[3] = 0;*/
        for( ; it != weights.end(); it++ )
        {
            State & state = queue->getEvent( it - weights.begin());
            w = (*it);

            /* The position is computed as the weighted average of the
               positions in the queue. The average is not normalized, to
               yield a more general filter.*/
            pos[0] += state.position[0] * w;
            pos[1] += state.position[1] * w;
            pos[2] += state.position[2] * w;

            /* The orientation is computed as the normalized weighted average of the
               orientations in the queue in log space. That is, they are transformed
               to 3D vectors inside the unit hemisphere and averaged in this linear space.
               The resulting vector is transformed back to a unit quaternion. */
            double angle = acos( state.orientation[3] ) * 2;
            double as = angle * w / sin( angle / 2 );
            orient[0] += state.orientation[0] * as;
            orient[1] += state.orientation[1] * as;
            orient[2] += state.orientation[2] * as;
            sum += w;
        }
        // calculate the length of the summed vector in log space
        // this is the angle of the result quaternion
        w = sqrt((orient[0]*orient[0] + orient[1]*orient[1] + orient[2]*orient[2])/(sum*sum));
        double as = sin( w / 2 ) / w;
        localState.orientation[0] = orient[0] * as;
        localState.orientation[1] = orient[1] * as;
        localState.orientation[2] = orient[2] * as;
        localState.orientation[3] = cos( w / 2 );
        MathUtils::normalizeQuaternion( localState.orientation );
        // copy pos to state.position
        localState.position[0] = pos[0];
        localState.position[1] = pos[1];
        localState.position[2] = pos[2];
        localState.time = event.time;
        updateObservers( localState );
    }
}
