 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for class EventQueueImplementation
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/EventQueueImplementation.cxx,v 1.2 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "EventQueueImplementation.h"

#include <algorithm>

// Destructor method.

EventQueueImplementation::~EventQueueImplementation()
{
    queue.clear();
}

// returns the event number n back in time starting with the newest for n = 0

State& EventQueueImplementation::getEvent(unsigned int number)
{
    if( number >=0 && number < queue.size())
    {
        return *queue[number];
    }
    return State::null;
}

// returns the event closes to the given point in time

State& EventQueueImplementation::getEventNearTime(double time)
{
    StateQueue::iterator index = queue.begin();
    while( index != queue.end())
    {
        if((*index)->time >= time )
            break;
        index++;
    }
    if( index != queue.end())
    {
        if( index == queue.begin() )
        {
            return *(*index);
        }
        StateQueue::iterator pre = index - 1;
        if( (*pre)->time - time < time - (*index)->time )
        {
            return *(*pre);
        } else
        {
            return *(*index);
        }
    }
    return *queue[queue.size()-1];
}

// returns the size of the queue

unsigned int EventQueueImplementation::getSize()
{
    return queue.size();
}

// inserts an event so that it is in order in time with the other events

void EventQueueImplementation::insertAtTime(State& event)
{
    StateQueue::iterator index = queue.begin();
    while( index != queue.end())
    {
        if((*index)->time >= event.time )
            break;
        index++;
    }
    if( index != queue.end())
    {
        if( (*index)->time == event.time )
        {
            (*index) = &event;
        }
        else
        {
            queue.insert( index, &event );
        }
    }
    else
    {
        queue.push_back( &event );
    }
}
