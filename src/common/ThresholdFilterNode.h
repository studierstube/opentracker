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
/** header file for ThresholdFilter Node.
  *
  * @author Gerhard Reitmayr
  * 
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/ThresholdFilterNode.h,v 1.1 2001/11/22 16:52:24 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section thresholdfilternode ThresholdFilter
 * The ThresholdFilter implements a filter that allows only events that differ sufficiently 
 * from the last event passed through the filter. It computes the 2-norm distance between
 * the positions of the new event and the last stored and the angle between the two quaternions
 * representing the rotations. The angle is computed in radiants. Then they are compared against
 * the two intervals given in the elements attributes. If one of them lies within the given 
 * interval, the event passes and is used as the new reference value.
 *
 * The element has the following attributes :
 * @li @c positionmin @c 0 minimal distance between the two positions
 * @li @c positionmax @c inf maximal distance between the two positions, 
 *                        @c inf means largest possible value
 * @li @c rotationmin @c 0 minimal angle between the two quaternions, a very general measure
 * @li @c rotationmax @c 3.141592654 maximal angle, default is pi the maximal possible angle.
 * Any values will be clamped to these intervals and it is ensured that max is >= min, by
 * setting max = min, if necessary.
 *
 * An example element looks like this :
 * @verbatim
<Filter weight="0.5 0.5 0.5">
    <EventQueue>...</EventQueue>
</Filter>@endverbatim
 */

#ifndef _THRESHOLDFILTERNODE_H
#define _THRESHOLDFILTERNODE_H

#include "../OpenTracker.h"

/**
 * The ThresholdFilterNode implements a filter that allows only events that
 * differ in a certain intervall from the last passed event. It computes the
 * 2-norm of the position difference and the dot product of the quaternions
 * representing the orientations and compares them with fixed boundary values.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API ThresholdFilterNode
    : public Node
{
// Members
protected:
    /// last saved state variable
    State lastState;
    /// min and max position values, must be in [0,inf)
    float positionMin, positionMax;
    /// min and max rotation values, must be in [0,pi]
    float rotationMin, rotationMax;

// Methods
protected:

    /** constructor method
     */
    ThresholdFilterNode( float posmin, float posmax, float rotmin, float rotmax );

public:

    /** tests for EventGenerator interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventGenerator, 0 otherwise */
    virtual int isEventGenerator() ;

    /**
     * this method is called by the EventGenerator to update it's observers.
     */
    virtual void onEventGenerated( State& event, Node& generator);

    friend class CommonNodeFactory;
};

#endif
