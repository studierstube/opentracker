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
/** header file for MatrixTransform Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section matrixtransform MatrixTransformation
 *
 * This section describes the MatrixTransformation configuration element. It 
 * transforms the data by applying an affine transformation to the 
 * the child's position data as post transformations. The transformation itself is
 * fixed and described by a 3x4 matrix which is set with the elements attributes. 
 * Note that this node only acts on position information !!
 *
 * The configuration elements are obtained by using the base name
 * @c MatrixTransformation and prepending it with one of the prefixes described before.
 * They have the following attribute:
 * @li @c matrix   a 3x4 matrix specifying an affine transformation    
 *
 * An example element looks like this :
 * @verbatim
 <EventMatrixTransform matrix="1 0 1 1 0 0 1 1 1 1 1 2">
    <Any EventGenerator element type>
 </EventMatrixTransform>@endverbatim
 */

#ifndef _MATRIXTRANSFORMATION_H
#define _MATRIXTRANSFORMATION_H

#include "Transformation.h"

/**
 * The MatrixTransformation class implements a static matrix post transformation on
 * the state of its child node. It implements the @ref matrixtransform nodes.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API MatrixTransformation : public Transformation
{
protected:

    /// stores the affine part of the transformation
    float matrix[3][4];

    /**
     * transforms a state. Overrides the Transformation implementation
     * to implement a different one.
     */
    virtual State* transformState( State* state) ;

    /** default constructor method sets values to implement identity 
     * transformation */
    MatrixTransformation();

    /** constructor method
     * @param matrix_ contains 3x4 matrix for affine transformation
     */
    MatrixTransformation( float * matrix_[4] );

    /** constructor method
     * @param matrix_ contains 3x4 matrix for affine transformation
     */
    MatrixTransformation( float * matrix_ );

public:


    /** returns the affine part of the Transformation 
     * @return float pointer to 3x4 floats 
    */
    float ** getMatrix()
    {
        return (float**)matrix;
    }


    /** sets the scale of the transformation. It is only
     * executed, if the transformation is of type to 
     * change the position of any events. It copies the
     * passed float array into an internal structure.
     * @param data a 3x4 float array containing the scale
     */
    void setMatrix(float* data[4])
    {        
        memcpy( matrix, data, sizeof(float)*12 );
    }

    friend class CommonNodeFactory;
};

} // namespace ot

#endif
