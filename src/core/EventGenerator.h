 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** The header file for the EventGenerator interface
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/EventGenerator.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _EVENTGENERATOR_H
#define _EVENTGENERATOR_H

#include <vector>
using namespace std;

#include "EventObserver.h"

/**
 * used to keep a list of EventObservers.
 */
typedef vector<EventObserver *> EventObserverVector;

/**
 * Interface of a class that generates single events. It also keeps
 * a vector of EventObservers that are registered to be notified
 * whenever a new event is generated.
 * @author Gerhard Reitmayr
 */
class EventGenerator
{
// Members
protected:
    /// the list of registered EventObservers
    EventObserverVector observers;

protected:
    /**
     * notifies the registered EventObservers by calling onEventGenerated.
     * @param event reference to the value of the event.
     */
    void updateObservers(State& event);

public:
    /** empty constructor */
    EventGenerator()
    {};
    /** destructor clears the list of observers. */
    virtual ~EventGenerator();
    /** registers an EventObserver with the EventGenerator.
     * @param observer reference to the observer */
    void addEventObserver(EventObserver& observer);
    /** unregisters an EventObserver with the EventGenerator.
     * @param observer reference to the observer */
    void removeEventObserver(EventObserver& observer);
};

#endif
