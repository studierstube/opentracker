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
/** header file for Filter Node.
  *
  * @author Gerhard Reitmayr
  * 
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section filternode Filter
 * The Filter node implements a simple linear averaging over a number of events.
 * The events are stored in an EventQueueNode that is the required child of this
 * node. Everytime it receives a new event from the EventQueueNode it computes the
 * weighted average of the events stored in the EventQueueNode. It implements an
 * Event interface.
 *
 * The position is calculated as the weighted average of the last events positions.
 * No normalization is done, so one can use this filter to calculate gain != 1. The
 * rotation is calculated in exponential space of the unit sphere of quaternions. 
 * Here the average is normalized to stay within the result space.
 * Confidence values are also averaged and not normalized.
 *
 * The element has the following attributes :
 * @li @c  weight a number of float values giving the individual weights of the
 *         events averaged. The number of events used from the queue are equal
 *         to the number of weights specified here. The first weight relates to the
 *         newest event. If less events are stored in the queue, no new event is generated.
 * @li @c  type @c all @c (all|position|orientation) a mode attribute that specifies to 
 *         which part of the event data the filter should be applied to. @c position will
 *         apply it only to the position part, @c orientation to the orientation and
 *         @all to both.
 *
 * An example element looks like this :
 * @verbatim
<Filter weight="0.5 0.5 0.5" type="all">
    <EventQueue>...</EventQueue>
</Filter>@endverbatim
 */

#ifndef _FILTERNODE_H
#define _FILTERNODE_H

#include "../OpenTracker.h"

#include <vector>

/**
 * The Filter node generates a new state event by combining several past 
 * events in a linear fashion. The weights of the summands can be adjusted.
 * It needs a child that both implements the Event and the EventQueue interface
 * to have access to the required number of states and to be triggered to
 * generate a new one.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API FilterNode
    : public Node
{
// Members
protected:
    /// local state variable
    State localState;

    /// array of weights
    std::vector<float> weights;

	enum Type { ALL = 0, POSITION, ORIENTATION } type;

// Methods
protected:

    /** constructor method
     */
    FilterNode( const std::vector<float> & weights_, const Type & type_ );

public:

    /** tests for EventGenerator interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventGenerator, 0 otherwise */
    virtual int isEventGenerator() ;

    /**
     * this method is called by the EventGenerator to update it's observers.
     * This class computes a transformed state, stores it in its local variable
     * and notifies its observers in turn, propagating the change.
     */
    virtual void onEventGenerated( State& event, Node& generator);

    friend class CommonNodeFactory;
};

#endif
