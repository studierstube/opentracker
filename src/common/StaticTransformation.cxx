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
/** source file for StaticTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/StaticTransformation.cxx,v 1.6 2001/04/16 15:43:11 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "StaticTransformation.h"

// default constructor method.

StaticTransformation::StaticTransformation()
{
    translation[0] = 0;
    translation[1] = 0;
    translation[2] = 0;
    rotation[0] = 0;
    rotation[1] = 0;
    rotation[2] = 0;
    rotation[3] = 1;
    scale[0] = 1;
    scale[1] = 1;
    scale[2] = 1;
    usePos = 0;
    useOrient = 0;
}

// constructor method.

StaticTransformation::StaticTransformation(float translation_[3], float scale_[3],
                               float rotation_[4], int usePos_, int useOrient_ )
    : Transformation(), usePos( usePos_ ), useOrient( useOrient_ )
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

State* StaticTransformation::transformState( State* state )
{
    // transform the position of the state
    if( usePos )
    {
        MathUtils::rotateVector( rotation,  state->position, localState.position );
        localState.position[0] = localState.position[0]*scale[0] + translation[0];
        localState.position[1] = localState.position[1]*scale[1] + translation[1];
        localState.position[2] = localState.position[2]*scale[2] + translation[2];
    }
    else {
        localState.position[0] = state->position[0];
        localState.position[1] = state->position[1];
        localState.position[2] = state->position[2];
    }
    // transform the orientation of the state
    if( useOrient )
    {
        MathUtils::multiplyQuaternion( rotation, state->orientation,
                                       localState.orientation );
    } 
    else {
        localState.orientation[0] = state->orientation[0];
        localState.orientation[1] = state->orientation[1];
        localState.orientation[2] = state->orientation[2];
        localState.orientation[3] = state->orientation[3];
    }
    // copy other state fields
    localState.button = state->button;
    localState.confidence = state->confidence;
    localState.time = state->time;
    return &localState;
}
