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
/** header file for class EventQueueNode.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section EventQueue EventQueue
 * The EventQueueNode stores a queue of events of fixed size. The size is
 * set by the attribute length. It stores the last events it received from
 * its EventGenerator child and also passes on events via the EventGenerator
 * interface. It has the following attributes :
 * @li @c length 1 the maximal number of last events to store
 *
 * An example element looks like this :
 * @verbatim
 <EventQueue length="10">
    <Any EventGenerator element type>
 </EventQueue>@endverbatim
 */

#ifndef _EVENTQUEUENODE_H
#define _EVENTQUEUENODE_H

#include "../OpenTracker.h"
#include "../core/EventQueueImplementation.h"

/**
 * EventQueueNode stores a queue of size length of the last received events.
 * It implements the EventGenerator as well as the EventQueue interface,
 * because it also notifies parents of events it receives.
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API EventQueueNode : public Node, protected EventQueueImplementation
{
// Members
protected:
    /// maximal length of the queue
    int length;

// Methods
protected:
    /** constructor method.
     * @param maximum length of the queue */
    EventQueueNode(int length_) : Node(), EventQueueImplementation()
    {
        length = length_;
    }
public:    
    /** tests for EventGenerator interface being present. Overriden in this
     * node to return this.
     * @return always this */
    virtual int isEventGenerator()
    {
        return 1;
    };
    /** tests for EventQueue interface being present. Overriden in this
     * node to return this.
     * @return always this */
    virtual int isEventQueue()
    {
        return 1;
    };

    /** this method is called by the EventGenerator to update it's observers
     * @param event new event
     * @param generator the calling EventGenerator */
    virtual void onEventGenerated( State& event, Node& generator);

    /** returns the event number n back in time starting with the
     * newest event for n = 0.
     * @param number the number of the event to be retrieved
     * @return reference to the State */
    virtual State& getEvent(unsigned int number = 0)
    {
        return EventQueueImplementation::getEvent( number );
    }
 
    /** returns the event closes to the given point in time
     * @param time point in the the event should be closest to
     * @return reference to the found event */
    virtual State& getEventNearTime(double time)
    {
        return EventQueueImplementation::getEventNearTime( time );
    }

    /** returns the size of the queue */
    virtual unsigned int getSize()
    {
        return EventQueueImplementation::getSize();
    }

    friend class CommonNodeFactory;
};

#endif
