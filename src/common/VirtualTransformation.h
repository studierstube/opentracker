 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for VirtualTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/VirtualTransformation.h,v 1.3 2001/02/20 18:02:49 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
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
 */
class VirtualTransformation : public StaticTransformation
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
                          float rotation_[4])
        : StaticTransformation(translation_, scale_, rotation_ )
    {}
};

#endif
