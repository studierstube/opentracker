 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for StaticTransform Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/StaticTransformation.h,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section transform Transformation
 * This section describes the Transformation configuration element. 
 * There are three different Transformation elements to provide a strong structure
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
 <EventTransformation rotationtype="euler" rotation="1 2 3" translation="0 0 0">
    <Any EventGenerator element type>
 </EventTransformation>@endverbatim
 */

#ifndef _STATICTRANSFORMATION_H
#define _STATICTRANSFORMATION_H

#include "Transformation.h"

/**
 * The StaticTransformation class implements a static post transformation on
 * the state of its child node. It also is the base for other static implementations
 * such as the VirtualTransformation as it implements the same fields.
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API StaticTransformation : public Transformation
{
protected:

    /// stores the translation of the transformation
    float translation[3];
    /// stores scale part of the transformation
    float scale[3];
    /// stores rotational part of the transformation
    float rotation[4];
    
    /**
     * transforms a state. Overrides the Transformation implementation
     * to implement a different one.
     */
    virtual State* transformState( State* state) ;

public:

    /** default constructor method sets values to implement identity 
     * transformation */
    StaticTransformation();

    /** constructor method
     * @param translation_ sets translation
     * @param scale_ sets scale
     * @param rotation_ sets rotation
     */
    StaticTransformation(float translation_[3], float scale_[3], float rotation_[4]);

    /** returns the rotational part of the Transformation */
    float* getRotation()
    {
        return (float*)rotation;
    }
    /** returns the scaling part of the Transformation */
    float* getScale()
    {
        return (float *)scale;
    }
    /** returns the translational part of this Transformation */
    float* getTranslation()
    {
        return (float *)translation;
    }
};

#endif
