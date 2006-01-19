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
