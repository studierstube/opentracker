 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** The header file for the EventObserver interface.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/EventObserver.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _EVENTOBSERVER_H
#define _EVENTOBSERVER_H

#include "State.h"

// forward class declaration to be able to declare interface
class EventGenerator;

/**
 * Interface of a class that observes EventGenerators. A node listening
 * to EventGenerators must derive from this class and implement the
 * onEventGenerated method.
 * @author Gerhard Reitmayr
 */
class EventObserver
{
// Methods
public:
    /**
     * this method notifies the object that a new event was generated.
     * It is called by an EventGenerator with which the object
     * registered using addEventObserver.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event,
                                   EventGenerator& generator) = 0;
};

#endif
