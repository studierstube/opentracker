 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** The header file for the EventQueue interface
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/EventQueue.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _EVENTQUEUE_H
#define _EVENTQUEUE_H

#include "State.h"

/**
 * A class that holds a queue of events and that allows accessing them in
 * some ways.
 *
 * @author Gerhard Reitmayr
 */
class EventQueue
{
public:
    /**
     * returns a stored event by index. The index starts with 0 for the
     * latest event and goes back in time with increasing index.
     * @param index index of the desired event, if no index is given the
     *        latest is returned.
     * @return reference to the value of the event. */
    virtual State& getEvent(int index = 0)  = 0;
    /**
     * returns the event closest to a given point in time.
     * @param time the point in time in milliseconds since 1.1.1970
     * @return reference to the value of the event. */
    virtual State& getEventNearTime(double time)  = 0;
    /**
     * returns the number of stored events.
     * @return number of stored events. */
    virtual int getSize()  = 0;
};
#endif
