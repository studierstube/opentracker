 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Transformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/Transformation.h,v 1.4 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _TRANSFORMATION_H
#define _TRANSFORMATION_H

#include "../OpenTracker.h"

/**
 * A Transform is the abstract superclass for all transform nodes. It provides 
 * the transformation for all three interfaces. By relying on its child to 
 * provide the interfaces implementation it also automatically mirrors its child
 * interfaces. This makes it not typesafe in that the interfaces of its child
 * and parent have to match but this has to be provided by the configuration
 * file DTD.
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API Transformation
    : public Node
{
// Members
protected:
    /// local state variable
    State localState;

// Methods
protected:
    /** transforms a state. Simplifies implementing other Transformations
     * as it is only needed to override this method. It stores the
     * transformed state in the localState member variable and returns
     * a pointer to it.
     * @param state pointer to original state
     * @return pointer to localState member
     */
    virtual State* transformState( State* state) = 0;

public:
    /** constructor method
     */
    Transformation();

    /**
     * returns the event number n back in time starting with the
     * newest for n = 0
     */
    virtual State& getEvent(unsigned int number = 0);

    virtual State& getEventNearTime(double time);   

    /** returns the size of the queue. Uses the childs
     * implementation, if possible.
     * @return size of queue */
    virtual unsigned int getSize();

    /** the function evaluation method.
     * @param time the point in time at which function is evaluated
     * @return reference to state value */
    virtual State& getStateAtTime(double time);

    /** tests for EventGenerator interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventGenerator, 0 otherwise */
    virtual int isEventGenerator() ;

    /** tests for EventQueue interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventQueue, 0 otherwise */
    virtual int isEventQueue() ;

    /** tests for TimeDependend interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements TimeDependend, 0 otherwise */
    virtual int isTimeDependend() ;

    /**
     * this method is called by the EventGenerator to update it's observers.
     * This class computes a transformed state, stores it in its local variable
     * and notifies its observers in turn, propagating the change.
     */
    virtual void onEventGenerated( State& event, Node& generator);
};

#endif
