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
/** source file for class EventQueueImplementation
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/EventQueueImplementation.cxx,v 1.5 2001/10/21 22:12:03 reitmayr Exp $
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
    if( number < queue.size())
    {
        return queue[number];
    }
    return State::null;
}

// returns the event closes to the given point in time

State& EventQueueImplementation::getEventNearTime(double time)
{
    StateQueue::iterator index = queue.begin();
    while( index != queue.end())
    {
        if((*index).time >= time )
            break;
        index++;
    }
    if( index != queue.end())
    {
        if( index == queue.begin() )
        {
            return (*index);
        }
        StateQueue::iterator pre = index - 1;
        if( (*pre).time - time < time - (*index).time )
        {
            return (*pre);
        } else
        {
            return (*index);
        }
    }
    return queue[queue.size()-1];
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
        if((*index).time >= event.time )
            break;
        index++;
    }
    if( index != queue.end())
    {
        if( (*index).time == event.time )
        {
            (*index) = event;
        }
        else
        {
            queue.insert( index, event );
        }
    }
    else
    {
        queue.push_back( event );
    }
}
