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
/** source file for InverTransform Nodes.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/InvertTransformation.cxx,v 1.1 2001/05/22 08:36:13 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "InvertTransformation.h"

// transforms a state.

State* InvertTransformation::transformState( State* state )
{
    // invert the data 
    MathUtils::invertQuaternion( state->orientation, localState.orientation );
    MathUtils::rotateVector( localState.orientation, state->position, localState.position );
    localState.position[0] *= -1.0;
    localState.position[1] *= -1.0;
    localState.position[2] *= -1.0;

    // copy other state fields
    localState.button = state->button;
    localState.confidence = state->confidence;
    localState.time = state->time;
    return &localState;
}
