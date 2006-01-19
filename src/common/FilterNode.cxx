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
/** source file for Filter Node.
  *
  * @author Gerhard Reitmayr
  * @todo check implementation of quaternion interpolation and document
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "FilterNode.h"
#include <math.h>

//using namespace std;

// constructor method.

namespace ot {

FilterNode::FilterNode( const std::vector<float> & weights_, const Type & type_ )
    : Node(), weights( weights_ ), type( type_ )
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
    if( queue != NULL && queue->getSize() == weights.size() )
    {
        double w,sum = 0;
        double pos[3] = {0, 0, 0 }, orient[3] = { 0, 0, 0 };
		double conf = 0;

        // referencerot for quaternion interpolation
        float * referencerot= queue->getEvent( 0 ).orientation;

        std::vector<float>::iterator it;
        for( it = weights.begin(); it != weights.end(); it++ )
        {
            State & state = queue->getEvent( it - weights.begin());
            w = (*it);

			if( type != ORIENTATION )
			{
			/*  The position is computed as the weighted average of the
				positions in the queue. The average is not normalized, to
				yield a more general filter.*/
				pos[0] += state.position[0] * w;
				pos[1] += state.position[1] * w;
				pos[2] += state.position[2] * w;
			}

			if( type != POSITION )
			{
			/* The orientation is computed as the normalized weighted average of the
				orientations in the queue in log space. That is, they are transformed
				to 3D vectors inside the unit hemisphere and averaged in this linear space.
				The resulting vector is transformed back to a unit quaternion. */

                if( MathUtils::dot(referencerot, state.orientation, 4) < 0 )
                {
                    state.orientation[0] = -state.orientation[0];
                    state.orientation[1] = -state.orientation[1];
                    state.orientation[2] = -state.orientation[2];
                    state.orientation[3] = -state.orientation[3];
                }

				double angle = acos( state.orientation[3] );
				double as = sin( angle );
				if( as != 0 )
					as = angle * w / as;
				else
					as = 0;					// lim x/(sin(x/2)) = 2 for x -> 0 ???
				orient[0] += state.orientation[0] * as;
				orient[1] += state.orientation[1] * as;
				orient[2] += state.orientation[2] * as;
			}

			/* confidence is also averaged */
			conf += state.confidence * w;

            sum += w;

        }
		if( type != POSITION )
		{
			// calculate the length of the summed vector in log space
			// this is the angle of the result quaternion
			w = sqrt((orient[0]*orient[0] + orient[1]*orient[1] + orient[2]*orient[2])/(sum*sum));
			double as = 0;
			if( w != 0)
				as = sin( w ) / w;
			localState.orientation[0] = (float)(orient[0] * as);
			localState.orientation[1] = (float)(orient[1] * as);
			localState.orientation[2] = (float)(orient[2] * as);
			localState.orientation[3] = (float)cos( w );
			MathUtils::normalizeQuaternion( localState.orientation );
		}
		else 
		{
			localState.orientation[0] = event.orientation[0];
			localState.orientation[1] = event.orientation[1];
			localState.orientation[2] = event.orientation[2];
			localState.orientation[3] = event.orientation[3];
		}
        
		if( type != ORIENTATION )
		{
			// copy pos to state.position
			localState.position[0] = (float)pos[0];
			localState.position[1] = (float)pos[1];
			localState.position[2] = (float)pos[2];
		}
		else
		{
			localState.position[0] = event.position[0];
			localState.position[1] = event.position[1];
			localState.position[2] = event.position[2];			
		}
        
		localState.confidence = (float)conf;

        localState.time = event.time;
		localState.button = event.button;
        updateObservers( localState );
    }
    else
    {
        updateObservers( event );
    }
}

} // namespace ot
