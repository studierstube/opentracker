 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Transformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/Transformation.cxx,v 1.4 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "Transformation.h"

// constructor method.

Transformation::Transformation()
    : Node()
    , localState()
{
}

// returns the event number n back in time starting with the newest for n = 0

State& Transformation::getEvent(unsigned int number)
{
	Node * child = getChild( 0 );
	if( child != NULL )
	{
		if( child->isEventQueue() == 1 )
		{    
			return *transformState( & child->getEvent( number ));
		}
    }
    return State::null;
}

// returns the event closes to the given point in time

State& Transformation::getEventNearTime(double time)
{
	Node * child = getChild( 0 );
	if( child != NULL )
	{
		if( child->isEventQueue() == 1 )
		{    
			return *transformState( & child->getEventNearTime( time ));
		}
    }   
    return State::null;
}

// returns the size of the queue

unsigned int Transformation::getSize()
{
	Node * child = getChild( 0 );
	if( child != NULL )
	{
		if( child->isEventQueue() == 1 )
		{    
			return child->getSize();
		}
    }
    return 0;
}

// the function evaluation method

State& Transformation::getStateAtTime(double time)
{
	Node * child = getChild( 0 );
	if( child != NULL )
	{
		if( child->isTimeDependend() == 1 )
		{    
			return *transformState(& child->getStateAtTime( time ));
		}
    }
    return State::null;
}

// tests for EventGenerator interface being present.

int Transformation::isEventGenerator()
{
    Node * child = getChild( 0 );
	if( child != NULL )
	{
		return child->isEventGenerator();
    }
	return 0;
}

// tests for EventQueue interface being present.

int Transformation::isEventQueue()
{
    Node * child = getChild( 0 );
	if( child != NULL )
	{
		return child->isEventQueue();
    }
	return 0;
}

// tests for TimeDependend interface being present.

int Transformation::isTimeDependend()
{
    Node * child = getChild( 0 );
	if( child != NULL )
	{
		return child->isTimeDependend();
    }
	return 0;
}

// this method is called by the EventGenerator to update it's observers.

void Transformation::onEventGenerated( State& event, Node& generator)
{
    updateObservers( *transformState( &event ));
}