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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Transformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
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
