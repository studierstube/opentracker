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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for VirtualTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/VirtualTransformation.cxx,v 1.4 2001/03/27 06:08:50 reitmayr Exp $
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
