 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** The header file for the RefNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/RefNode.h,v 1.2 2001/03/26 22:45:47 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _REFNODE_H
#define _REFNODE_H

#include "../dllinclude.h"

#include "Node.h"

/**
 * A Reference Node contains the reference to another node and its subtree.
 * It can only occur as a leaf node. However it reflects the interface of
 * the referenced node and also updates it's parents, whenever the referenced
 * node generates an event.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API RefNode : public Node
{

protected:
	/** pointer to the referenced node */
	Node * reference;
 
public:
   /**
    * basic constructor.
    */
    RefNode( Node * ref ) :
	  Node(),
      reference( ref )		
    {
		reference->addReference( this );  
	}
   /**
    * basic destructor.
    */
    virtual ~RefNode()
    {
		reference->removeReference( this );
	}
	
	// begin EventGenerator & EventObserver interface

	/**
     * this method notifies the object that a new event was generated.
     * It is called by an EventGenerator.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event,
                                   Node& generator)
	{
		updateObservers( event );
	}

    /**
     * tests for EventGenerator interface being implemented. This has to
     * be overriden in classes that subclass EventGenerator. Due to
     * inheritance raints it cannot be done automatically.
     * @returns a pointer to the EventGenerator interface, or NULL if it
     *   is not implemented
     */
    virtual int isEventGenerator()
    {
        return reference->isEventGenerator();
    }

	// end EventGenerator interface

	// begin EventQueue interface

    /**
     * tests for EventQueue interface being present. This has to be overriden in
     * classes that subclass EventQueue. See isEventGenerator for details.
     * @returns a pointer to the EventQueue interface, or NULL if it is not
     *       implemented.
     */
    virtual int isEventQueue()
    {
        return reference->isEventQueue();
    }
	/**
     * returns a stored event by index. The index starts with 0 for the
     * latest event and goes back in time with increasing index.
     * @param index index of the desired event, if no index is given the
     *        latest is returned.
     * @return reference to the value of the event. */
    virtual State& getEvent(unsigned int index = 0)
	{
		return reference->getEvent( index );
	}
    /**
     * returns the event closest to a given point in time.
     * @param time the point in time in milliseconds since 1.1.1970
     * @return reference to the value of the event. */
    virtual State& getEventNearTime(double time)
	{
		return reference->getEventNearTime( time );
	}
    /**
     * returns the number of stored events.
     * @return number of stored events. */
    virtual unsigned int getSize()
	{
		return reference->getSize();
	}

	// end EventQueue interface

	// begin TimeDependend interface

    /**
     * tests for TimeDependend interface being present. This has to be overriden
     * in classes that subclass TimeDependend. See isEventGenerator for details.
     * @returns a pointer to the TimeDependend interface, or NULL if it is not
     *      implemented.
     */
    virtual int isTimeDependend()
    {
        return reference->isTimeDependend();
    }

	/**
     * returns the state of the time dependend function at a given
     * point in time.
     * @param time point in time in milliseconds since 1.1.1970
     * @return reference to the state value */
    virtual State& getStateAtTime(double time)
	{
		return reference->getStateAtTime( time );
	}

	// end TimeDependend interface
};

#endif
