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
/** source file for MatrixTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "MatrixTransformation.h"

namespace ot {

// default constructor method.

MatrixTransformation::MatrixTransformation()
{
    for( int i = 0; i < 3; i++ )
        for( int j = 0; j < 4; j++ )
            if( i == j )
                matrix[i][j] = 1;
            else
                matrix[i][j] = 0;
}

// constructor methods.

MatrixTransformation::MatrixTransformation( float * matrix_[4] )
    : Transformation()
{
    for( int i = 0; i < 3; i ++ )
    {
        for( int j = 0; j < 4; j ++ )
            matrix[i][j] = matrix_[i][j];
    }
}

MatrixTransformation::MatrixTransformation( float * matrix_)
    : Transformation()
{
    for( int i = 0; i < 3; i ++ )
    {
        for( int j = 0; j < 4; j ++ )
            matrix[i][j] = matrix_[4*i+j];
    }
}
// transforms a state.

State* MatrixTransformation::transformState( State* state )
{

    // transform the position of the state
    float * pos = state->position;
    localState.position[0] = matrix[0][0]*pos[0] + matrix[0][1]*pos[1] + matrix[0][2]*pos[2] + matrix[0][3];
    localState.position[1] = matrix[1][0]*pos[0] + matrix[1][1]*pos[1] + matrix[1][2]*pos[2] + matrix[1][3];
    localState.position[2] = matrix[2][0]*pos[0] + matrix[2][1]*pos[1] + matrix[2][2]*pos[2] + matrix[2][3];

    localState.orientation[0] = state->orientation[0];
    localState.orientation[1] = state->orientation[1];
    localState.orientation[2] = state->orientation[2];
    localState.orientation[3] = state->orientation[3];
 
    // copy other state fields
    localState.button = state->button;
    localState.confidence = state->confidence;
    localState.time = state->time;
    return &localState;
}

} // namespace ot
