 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** The source file for the EventGenerator interface
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/EventGenerator.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "EventGenerator.h"
#include <algorithm>

/// Destructor method.

EventGenerator::~EventGenerator()
{
    observers.clear();
}

/// registers an EventObserver with the EventGenerator

void EventGenerator::addEventObserver(EventObserver& observer)
{
    EventObserverVector::iterator result = find( observers.begin(),
        observers.end(), &observer );
    if( result == observers.end())
    {
        observers.push_back( &observer );
    }
}

/// unregisteres an EventObserver with the EventGenerator

void EventGenerator::removeEventObserver(EventObserver& observer)
{
    EventObserverVector::iterator result = find( observers.begin(),
        observers.end(), &observer );
    if( result != observers.end())
    {
        observers.erase( result );
    }
}

/// updates the observers of the generator

void EventGenerator::updateObservers(State& event)
{
    for( EventObserverVector::iterator index = observers.begin();
        index != observers.end(); index++ )
    {
        (*index)->onEventGenerated( event, *this );
    }
}
