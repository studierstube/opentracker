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
/** header file for RangeFilter Node.
  *
  * @author Gerhard Reitmayr
  * 
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section RangeFilterNode RangeFilter
 * The RangeFilter node filters updates according to the length of the position
 * value. It is configured with a @c min and @c max attribute that define
 * the range of lengths of events to be passed through. Other events are dropped
 *
 * The element has the following attributes :
 * @li @c min 0, minimal length in meters
 * @li @c max large value, maximal length in meters
 *
 * An example element looks like this :
 * @verbatim
<RangeFilter min="0.1" max="2.5">
    <Any EventGenerator element type>
</RangeFilter>@endverbatim
 */

#ifndef _RANGEFILTERNODE_H
#define _RANGEFILTERNODE_H

#include "../OpenTracker.h"

/**
 * The RangeFilterNode implements a filter that allows only events the position
 * value of which has a length within a given range. It is a simple EventGenerator
 * node.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API RangeFilterNode
    : public Node
{
// Members
protected:

    /// minimum length, stores as the square of the length not the actual length !
    float min;
    /// maximum length stored as the square of the length not the actual length !
    float max;

// Methods
protected:

    /** constructor method
     */
    RangeFilterNode( float min, float max );

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
