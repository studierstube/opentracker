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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/VirtualTransformation.h,v 1.8 2001/04/30 10:10:15 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 *
 * @section virtualtransform VirtualTransformation
 *
 * This section describes the VirtualTransformation configuration element. It 
 * implements an offset in the child's affine space. That is the configured
 * translation and rotation are post transformed with the child's position
 * and orientation values. This effectively offsets the tracked point with
 * respect to the tracked affine base. Note that there is no scale, as a 
 * scaled base only makes sense, if there is a translational offset and in
 * that case the scale could be precomputed on the translational offset anyway.
 *
 * There are three variants of this transformation. The @c VirtualPositionTransform
 * element configures only a translational offset and accordingly only the
 * translation attribute can be specified. 
 * @c VirtualOrientationTransform configures a rotational offset and again only
 * the rotation attributes can be specified. 
 * Finally the simple @c VirtualTransform acts on both and
 * again a full transformation can be specified.
 * 
 * The configuration elements are obtained by using one of the base names
 * @c VirtualPositionTransform|VirtualOrientationTransform|VirtualTransform 
 * and prepending it with one of the prefixes described before.
 * They have the following attributes:
 * @li @c translation   0 0 0    (T)
 * @li @c rotationtype (quaternion|matrix|euler) quaternion
 * @li @c rotation        0 0 0 1 (R)
 * 
 *
 * An example element looks like this :
 * @verbatim
 <EventVirtualTransform rotationtype="euler" rotation="1 2 3" translation="0 0 0">
    <Any EventGenerator element type>
 </EventVirtualTransform>@endverbatim
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
