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
/** header file for Position Filter Node.
  *
  * @author Thomas Psik
  * 
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section PositionFilterNode PositionFilter
 * The PositionFilterNode node filters updates according to the position information. @see RangeFilterNode
 *  It is configured with a @c min and @c max attribute that define
 * the minimum and maximum of position data to be passed through (like a BoundingBox). Other events are dropped
 *
 * The element has the following attributes :
 * @li @c min -1, -1, -1, minimal position in meters
 * @li @c max 1, 1, 1, maximal position in meters
 *
 * An example element looks like this :
 * @verbatim
<PositionFilter min="-1 -1 -1" max="1 1 1">
    <Any EventGenerator element type>
</PositionFilter>@endverbatim
 */
#ifndef _POSITIONFILTERNODE_H
#define _POSITIONFILTERNODE_H

#include "../OpenTracker.h"


namespace ot {


/**
 * The PositionFilterNode implements a filter that allows only events the position
 * value of which is within a given range. It is a simple EventGenerator
 * node.
 * @author Thomas Psik
 * @ingroup common
 */
class OPENTRACKER_API PositionFilterNode
    : public Node
{
// Members
protected:

   /// minimum position x,y,z
    float min[3];
    /// maximum position x,y,z
    float max[3];

// Methods
protected:

    /** constructor method
     */
    PositionFilterNode( const float min[3], const float max[3]);

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


}  // namespace ot


#endif 

