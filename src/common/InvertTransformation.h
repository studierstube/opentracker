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
/** header file for InvertTransform Nodes.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/InvertTransformation.h,v 1.1 2001/05/22 08:36:13 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page transform_nodes Transform Node Reference
 * @section inverttransform InvertTransform
 *
 * An InvertTransform inverts the affine base described by the tracking data.
 * Put another way, if you use the output of an InvertTransform and combine
 * it with the input using a DynamicTransform, you will get the canonical
 * base, i.e. position in the origin and orientation equal to identity.
 *
 * There are again three flavours, one for each interface type.
 * The configuration elements are obtained by using the base name @c InvertTransform
 * and prepending it with one of the prefixes described before.
 *
 * An example element looks like this :
 * @verbatim
 <EventInvertTransform>
    <Any EventGenerator element type>
 </EventInvertTransform>@endverbatim
 */

#ifndef _INVERTTRANSFORMATION_H
#define _INVERTTRANSFORMATION_H

#include "Transformation.h"

/**
 * The InverTransform class implements a transformation on the child node, that
 * inverts the affine base described by the tracking data. It has no additional
 * fields.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API InvertTransformation : public Transformation
{
protected:

    /**
     * transforms a state. Overrides the Transformation implementation
     * to implement the inversion
     */
    virtual State* transformState( State* state) ;

public:

    /** default constructor method sets values to implement identity 
     * transformation */
    InvertTransformation()
    {}
};

#endif
