 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Transformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/Transformation.cxx,v 1.2 2001/02/19 07:31:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "Transformation.h"

// constructor method.

Transformation::Transformation()
    : Node()
    , EventGenerator()
    , localState()
{
}

// adds a child to the Node.

void Transformation::addChild( Node& node)
{
    child = (Node *) &node;
    if( node.isEventGenerator() != NULL )
    {
        node.isEventGenerator()->addEventObserver( *this );
    }
}

// returns the event number n back in time starting with the newest for n = 0

State& Transformation::getEvent(int number)
{
    if( isEventQueue() != NULL )
    {
        return *transformState( & child->isEventQueue()->getEvent( number ));
    }
    return State::null;
}

// returns the event closes to the given point in time

State& Transformation::getEventNearTime(double time)
{
    if( isEventQueue() != NULL )
    {
        return *( State *)transformState(& child->isEventQueue()->getEventNearTime( time ));
    }
    return State::null;
}

// returns the size of the queue

int Transformation::getSize()
{
    if( isEventQueue() != NULL )
    {
        return child->isEventQueue()->getSize();
    }
    return 0;
}

// the function evaluation method

State& Transformation::getStateAtTime(double time)
{
    if( isTimeDependend() != NULL )
    {
        return *transformState(& child->isTimeDependend()->getStateAtTime( time ));
    }
    return State::null;
}

// tests for EventGenerator interface being present.

EventGenerator * Transformation::isEventGenerator()
{
    if( child->isEventGenerator() != NULL )
    {
        return this;
    }
    return NULL;
}

// tests for EventQueue interface being present.

EventQueue * Transformation::isEventQueue()
{
    if( child->isEventQueue() != NULL )
    {
        return this;
    }
    return NULL;
}

// tests for TimeDependend interface being present.

TimeDependend * Transformation::isTimeDependend()
{
    if( child->isTimeDependend() != NULL )
    {
        return this;
    }
    return NULL;
}

// this method is called by the EventGenerator to update it's observers.

void Transformation::onEventGenerated( State& event,
                                       EventGenerator& generator)
{
    updateObservers( *transformState( &event ));
}
