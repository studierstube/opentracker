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
/** header file for DynamicTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * @todo think about using other node types than only the EventGenerator
  * as the base child. What semantics do make sense ??
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page transform_nodes Transformation Node Reference
 * This page describes the nodes implementing geometric transformations 
 * available in OpenTracker. The currently implemented transformations are
 * a specific subset of all affine transformations possible.
 *
 * All transformations receive the event from a single child node, transform
 * it and pass it on. To work with all interface types each transformation has
 * three different configuration elements whose outputs and inputs are typed
 * with the same interface. The configuration elements are distinguished by
 * the following prefixes :
 * @li @c Event - the child node and the output interface are of type EventGenerator
 * @li @c Queue - the child node and the output interface are of type EventQueue
 * @li @c Time  - the child node and the output interface are of type Timedependend
 * The transformation nodes act transparently and actually exhibit the same type
 * as their child node.
 * 
 * The following sections describe the different transformation nodes in detail.
 * 
 * @section dynamictransform DynamicTransformation
 *
 * This section describes the DynamicTransformation node and configuration element. 
 * A DynamicTransformation transforms the data child's state value by the base
 * child's state value. The base childs position and orientation define
 * the affine base in which the data childs values are interpreted. Then the data childs values
 * are transformed to world coordinates. The basic name of the configuration element
 * is @c DynamicTransform , by prepending the prefixes above, one receives the
 * actual element names. The @c TransformBase child must always be an EventGenerator.
 *
 * The confidence value of the result event is the product of the confidence values
 * of the two input events. Button state is only passed from the child, never from the 
 * @c TransformBase child. If you want to merge the button state you must use an additional
 * @ref buttonopnode to do that. The timestamp is always of the last event that triggers an
 * output (see @c baseevent).
 *
 * The elements may have the following attribute.
 *
 * @li @c baseevent (true|false) flag to define whether a change in the base generates an event.
 *                  If set to true, a new event is also generated whenever the @c TransformBase 
 *                  child passes an event on.
 * 
 * An example element looks like this :
 * @verbatim
 <EventDynamicTransform baseevent="true">
    <Any EventGenerator element type>
    <TransformBase>
        <Any EventGenerator element type>
    </TransformBase>
 </EventDynamicTransform>@endverbatim
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
 * The confidence value of the result event is the product of the confidence values
 * of the two input events.
 * @author Gerhard Reitmayr
 * @ingroup common
 */

namespace ot {

class OPENTRACKER_API DynamicTransformation : public StaticTransformation
{
protected:

    /** stores the original event data to compute new values
     * on base changes.*/
    State store;
	/** flag, whether a change in the base should generate an event or not */
	int baseEvent;

    /** constructor method. It sets default values on the
     * underlying StaticTransformation. */
    DynamicTransformation( int baseEvent_ = 1, bool usePos_ = true, bool useOrient_ = true);

public:
    
    /**
     * this method is called by the EventGenerator to update it's observers.
     * This class computes a transformed state, stores it in its local variable
     * and notifies its observers in turn, propagating the change.
     * If the EventGenerator equals the baseChild, its data is stored in the
     * underlying StaticTransformation fields, to change the transformation itself.
     */
    virtual void onEventGenerated( State& event, Node& generator);

    friend class CommonNodeFactory;

};

} // namespace ot

#endif
