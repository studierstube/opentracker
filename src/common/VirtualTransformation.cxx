 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for VirtualTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/VirtualTransformation.cxx,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "VirtualTransformation.h"

// transforms a state.

State* VirtualTransformation::transformState( State* state)
{
  // transform the position of the state
    MathUtils::rotateVector( state->orientation,
                             translation,
                             localState.position );
    localState.position[0] += state->position[0];
    localState.position[1] += state->position[1];
    localState.position[2] += state->position[2];
    // transform the orientation of the state
    MathUtils::multiplyQuaternion( state->orientation,
                                   rotation,
                                   localState.orientation );
    // copy other state fields
    localState.button = state->button;
    localState.confidence = state->confidence;
	localState.time = state->time;
    return &localState;
}
