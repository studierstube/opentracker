 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Transformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/Transformation.h,v 1.1 2001/01/29 17:16:44 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section transform Transformation
 * This section describes the Transformation configuration element. There are
 * three different Transformation elements to provide a strong structure
 * in the configuration file syntax. They have the following attributes :
 * @li @c translation   0 0 0    (T)
 * @li @c scale         1 1 1    (S)
 * @li @c rotationtype (quaternion|matrix|euler) quaternion
 * @li @c rotation        0 0 0 1 (R)
 * The Transformation is specified by T*S*R.
 *
 * An example element looks like this :
 * @verbatim
 <EventTransformation rotationtype="euler" rotation="1 2 3" translation="0 0 0">
    <Any EventGenerator element type>
 </EventTransformation>@endverbatim
 */

#ifndef _TRANSFORMATION_H
#define _TRANSFORMATION_H

#include "../OpenTracker.h"
/*
#include "Node.h"
#include "EventGenerator.h"
#include "EventObserver.h"
#include "EventQueue.h"
#include "TimeDependend.h"
#include "State.h"
#include "MathUtils.h"
*/

/**
 * A Transform is the superclass for all transform nodes. It provides the
 * transformation for all three interfaces. By relying on its child to provide
 * the interfaces implementation it also automatically mirrors its child
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
    /// stores the translation of the transformation
    float translation[3];
    /// stores scale part of the transformation
    float scale[3];
    /// stores rotational part of the transformation
    float rotation[4];
    /// State variable to put transformed state into
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
    virtual State* transformState( State* state);

public:
    /** ructor method
     * @param translation_ sets translation
     * @param scale_ sets scale
     * @param rotation_ sets rotation
     */
    Transformation(float translation_[3], float scale_[3], float rotation_[4]);
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
    /** returns the rotational part of the Transformation */
    float* getRotation()
    {
        return (float*)rotation;
    }
    /** returns the scaling part of the Transformation */
    float* getScale()
    {
        return (float *)scale;
    }
    /** returns the size of the queue. Uses the childs
     * implementation, if possible.
     * @return size of queue */
    virtual int getSize();
    /** the function evaluation method.
     * @param time the point in time at which function is evaluated
     * @return reference to state value */
    virtual State& getStateAtTime(double time);
    /** returns the translational part of this Transformation */
    float* getTranslation()
    {
        return (float *)translation;
    }
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

