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
/** header file for VirtualTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/VirtualTransformation.h,v 1.7 2001/04/18 16:38:18 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 *
 * @section virtualtransform VirtualTransformation
 * This section describes the VirtualTransformation configuration element. 
 * There arethree different Transformation elements to provide a strong structure
 * in the configuration file syntax. It works similar to the Transformation element. 
 * They have the following attributes:
 * @li @c translation   0 0 0    (T)
 * @li @c scale         1 1 1    (S)
 * @li @c rotationtype (quaternion|matrix|euler) quaternion
 * @li @c rotation        0 0 0 1 (R)
 * 
 * The Transformation is specified by T*S*R.
 *
 * An example element looks like this :
 * @verbatim
 <EventVirtualTransformation rotationtype="euler" rotation="1 2 3" translation="0 0 0">
    <Any EventGenerator element type>
 </EventVirtualTransformation>@endverbatim
 */

#ifndef _VIRTUALTRANSFORMATION_H
#define _VIRTUALTRANSFORMATION_H

#include "StaticTransformation.h"

/**
 * A VirtualTransformation acts differently from a common Transformation.
 * It is a local transformation of the states coordinate system.
 * Mathematically it is a pre-transformation as opposed to a
 * post-transformation like the VirtualTransformation.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API VirtualTransformation : public StaticTransformation
{
// Methods
protected:
    /**
     * transforms a state. Overrides the Transformation implementation
     * to implement a different one.
     */
    virtual State* transformState( State* state) ;

public:
    /** default constructor method sets transformation to identity */
    VirtualTransformation() : StaticTransformation()
    {}

    /** constructor method. */
    VirtualTransformation(float translation_[3], float scale_[3],
                          float rotation_[4], int usePos_, int useOrient_ )
        : StaticTransformation(translation_, scale_, rotation_, usePos_, useOrient_ )
    {}
};

#endif
