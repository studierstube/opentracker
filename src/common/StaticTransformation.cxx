 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for StaticTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/StaticTransformation.cxx,v 1.1 2001/02/19 07:31:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "StaticTransformation.h"

// constructor method.

StaticTransformation::StaticTransformation(float translation_[3], float scale_[3],
                               float rotation_[4])
    : Transformation()
{
    for( int i = 0; i < 3; i ++ )
    {
        this->translation[i] = translation_[i];
        this->scale[i] = scale_[i];
        this->rotation[i] = rotation_[i];
    }
    this->rotation[3] = rotation_[3];
}

// transforms a state.

State* StaticTransformation::transformState( State* state)
{
    // transform the position of the state
    MathUtils::rotateVector( rotation,  state->position, localState.position );
    localState.position[0] = localState.position[0]*scale[0] + translation[0];
    localState.position[1] = localState.position[1]*scale[1] + translation[1];
    localState.position[2] = localState.position[2]*scale[2] + translation[2];
    // transform the orientation of the state
    MathUtils::multiplyQuaternion( rotation,
                                   state->orientation,
                                   localState.orientation );
    // copy other state fields
    localState.button = state->button;
    localState.confidence = state->confidence;
    localState.time = state->time;
    return &localState;
}
