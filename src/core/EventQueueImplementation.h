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
/** header file for class EventQueueImplementation
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/EventQueueImplementation.h,v 1.5 2001/04/08 19:31:09 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _EVENTQUEUEIMPLEMENTATION_H
#define _EVENTQUEUEIMPLEMENTATION_H

#include "../dllinclude.h"

#include <deque>

using namespace std;

#include "State.h"

/** A front and back insert Vector like container, for simple and
 * fast queueing of timestamped events. */
typedef deque<State *> StateQueue;

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
class OPENTRACKER_API EventQueueImplementation
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
    virtual State& getEvent(unsigned int number = 0) ;
    /** returns the event closes to the given point in time
     * @param time point in the the event should be closest to
     * @return reference to the found event */
    virtual State& getEventNearTime(double time) ;
    /** returns the size of the queue */
    virtual unsigned int getSize() ;
};

#endif
