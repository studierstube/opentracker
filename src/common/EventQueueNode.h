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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/EventQueueNode.h,v 1.3 2001/03/27 06:08:50 reitmayr Exp $
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
 */
class OPENTRACKER_API EventQueueNode : public Node, public EventQueueImplementation
{
// Members
protected:
    /// maximal length of the queue
    int length;

// Methods
public:
    /** constructor method.
     * @param maximum length of the queue */
    EventQueueNode(int length_) : Node(), EventQueueImplementation()
    {
        length = length_;
    }

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
};

#endif
