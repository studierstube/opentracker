 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for DynamicTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * @todo think about using other node types than only the EventGenerator
  * as the base child. What semantics do make sense ??
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/DynamicTransformation.h,v 1.2 2001/03/06 18:06:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section dynamictransform DynamicTransformation
 * This section describes the DynamicTransformation configuration element. 
 * A DynamicTransformation transforms the data child's state value by the base
 * child's state value. The base childs position and orientation are assumed
 * to be the affine base of the data childs values. Then the data childs values
 * are transformed to world coordinates. Note that there is no complement inverse
 * element, because te 
 *
 * There are three different Transformation elements to provide a strong structure
 * in the configuration file syntax. They have no attributes but use a single
 * wrapper tag to define the base child.
 * 
 * An example element looks like this :
 * @verbatim
 <EventDynamicTransformation>
    <Any EventGenerator element type>
    <TransformBase>
        <Any EventGenerator element type>
    </TransformBase>
 </EventDynamicTransformation>@endverbatim
 */

#ifndef _DYNAMICTRANSFORMATION_H
#define _DYNAMICTRANSFORMATION_H

#include "StaticTransformation.h"

/**
 * The DynamicTransformation class implements a transformation that puts
 * the data childs data into the affine space defined by the base childs
 * data. If no data was received by the child sofar, it uses the
 * identity transformation. It builds upon the StaticTransformation by
 * setting the members of the StaticTransformation to the base childs
 * data. 
 * @author Gerhard Reitmayr
 */
class DynamicTransformation : public StaticTransformation
{
protected:

    /// the data of this child defines the base affine space
    EventGenerator * baseChild;
    /** stores the original event data to compute new values
     * on base changes.*/
    State store;

public:
    /** constructor method. It sets default values on the
     * underlying StaticTransformation. */
    DynamicTransformation();

    /**
     * adds a child to the Node. If the child is a TransformBase 
     * WrapperNode, its child is used to define the baseChild.
     * and 
     * @param node reference to the new child node.
     */
    virtual void addChild( Node& node );

    /**
     * this method is called by the EventGenerator to update it's observers.
     * This class computes a transformed state, stores it in its local variable
     * and notifies its observers in turn, propagating the change.
     * If the EventGenerator equals the baseChild, its data is stored in the
     * underlying StaticTransformation fields, to change the transformation itself.
     */
    virtual void onEventGenerated( State& event,
                                   EventGenerator& generator);

};

#endif
