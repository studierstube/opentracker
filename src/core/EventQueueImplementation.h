 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for class EventQueueImplementation
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/EventQueueImplementation.h,v 1.2 2001/01/29 17:16:44 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _EVENTQUEUEIMPLEMENTATION_H
#define _EVENTQUEUEIMPLEMENTATION_H

#include <deque>

#include "EventQueue.h"
#include "State.h"

/** A front and back insert Vector like container, for simple and
 * fast queueing of timestamped events. */
typedef std::deque<State *> StateQueue;

/**
 * Helper class that implements the EventQueue interface using a STL deque
 * container. Some classes do not need the implementation of the EventQeue
 * interfaces while others depend on it. The later should derive from this
 * class.
 * @author Gerhard Reitmayr
 * @todo research passing iterators between methods, then implement
 * time base search as a method of its own.
 * @ingroup core
 */
class EventQueueImplementation : public EventQueue
{
// Members
protected:
    /// queue of events
    StateQueue queue;
// Methods
protected:
    /** inserts an event so that it is in order in time with the other events.
     * @param event timestamped event that is inserted */
    void insertAtTime(State& event);

public:
    /// Destructor method clears queue
    virtual ~EventQueueImplementation();
    /** returns the event number n back in time starting with the
     * newest event for n = 0.
     * @param number the number of the event to be retrieved
     * @return reference to the State */
    virtual State& getEvent(int number = 0) ;
    /** returns the event closes to the given point in time
     * @param time point in the the event should be closest to
     * @return reference to the found event */
    virtual State& getEventNearTime(double time) ;
    /** returns the size of the queue */
    virtual int getSize() ;
};

#endif
