 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for class EventQueueNode.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/EventQueueNode.h,v 1.2 2001/03/26 22:11:21 reitmayr Exp $
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
