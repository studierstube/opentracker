 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Transformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/Transformation.h,v 1.3 2001/02/20 18:02:49 reitmayr Exp $
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
class Transformation
    : public Node
    , public EventGenerator
    , public EventObserver
    , public EventQueue
    , public TimeDependend
{
// Members
protected:
    /// pointer to the one possible child
    Node* child;
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
     * adds a child to the Node. Sets the child member to the node.
     * If the node implements the EventGenerator interface, it registers
     * as an EventObserver with it.
     * @param node reference to the new child node.
     */
    virtual void addChild( Node& node);

    /**
     * returns the event number n back in time starting with the
     * newest for n = 0
     */
    virtual State& getEvent(int number = 0);

    virtual State& getEventNearTime(double time);   

    /** returns the size of the queue. Uses the childs
     * implementation, if possible.
     * @return size of queue */
    virtual int getSize();

    /** the function evaluation method.
     * @param time the point in time at which function is evaluated
     * @return reference to state value */
    virtual State& getStateAtTime(double time);

    /** tests for EventGenerator interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventGenerator, 0 otherwise */
    virtual EventGenerator * isEventGenerator() ;

    /** tests for EventQueue interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements EventQueue, 0 otherwise */
    virtual EventQueue * isEventQueue() ;

    /** tests for TimeDependend interface being present. Returns the
     * result of the childs implementation of this method.
     * @return 1 if child implements TimeDependend, 0 otherwise */
    virtual TimeDependend * isTimeDependend() ;

    /**
     * this method is called by the EventGenerator to update it's observers.
     * This class computes a transformed state, stores it in its local variable
     * and notifies its observers in turn, propagating the change.
     */
    virtual void onEventGenerated( State& event,
                                   EventGenerator& generator);
};

#endif
