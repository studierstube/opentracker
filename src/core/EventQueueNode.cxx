 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for class EventQueueNode.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/EventQueueNode.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "EventQueueNode.h"

#ifdef WIN32
#include <iostream>    // new IOStream for VisualC++
#else
#include <iostream.h>
#endif

/// adds a child to the Node.

void EventQueueNode::addChild(Node& node)
{
    if( node.isEventGenerator() != NULL )
    {
        node.isEventGenerator()->addEventObserver( *this );
        TreeNode::addChild( node );
    }
    else
    {
        cerr << "EventQueueNode : child is not an EventGenerator !" << endl;
    }
}

/// this method is called by the EventGenerator to update it's observers

void EventQueueNode::onEventGenerated( State& event,
                                       EventGenerator& generator)
{
    if( queue.size() == length )
    {
        queue.pop_back();
    }
    insertAtTime( event );
    updateObservers( event );
}
