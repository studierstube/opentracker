 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for class EventQueueNode.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/EventQueueNode.cxx,v 1.2 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "EventQueueNode.h"

#ifdef WIN32
#include <iostream>    // new IOStream for VisualC++
#else
#include <iostream.h>
#endif

// this method is called by the EventGenerator to update it's observers

void EventQueueNode::onEventGenerated( State& event, Node& generator)
{
    if( queue.size() == length )
    {
        queue.pop_back();
    }
    insertAtTime( event );
    updateObservers( event );
}
