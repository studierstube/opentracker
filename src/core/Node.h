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
/** The header file for the basic Node class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Node.h,v 1.12 2001/04/29 16:34:44 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _NODE_H
#define _NODE_H

#include "../dllinclude.h"

/**
 * @defgroup core Core Classes
 * This group contains the basic interfaces and functionality to parse
 * a configuration file, build the datastructures and drive the event model.
 * Typically these classes are subclassed to provide new nodes or modules 
 * with additional functionality. Any changed to these classes may result
 * in widespread changes throughout the library.
 */

#include <string>
#include <vector>

using namespace std;

class ConfigurationParser;
class RefNode;
class DOM_Element;
class Node;

/**
 * a Vector of Node pointers. Very useful to implement a simple
 * container of Nodes such as a parent node, or to keep pointers
 * to several nodes around.
 */
typedef vector<Node *> NodeVector;

#include "State.h"

/**
 * The basic superclass for all nodes. Moreover a name
 * attribute for a unique ID is there but not used yet.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API Node
{

protected:
	/** Pointer to the parent XML element. This is untyped to avoid
	 * dependencies on XML parser library in this include file. */
	DOM_Element * parent;

	/**  A Vector of pointers to reference nodes referencing this node. */
	NodeVector references;

    /// the unique ID given in the XML configuration file.
    string name;

protected:
   /**
    * sets the value of name.
    * @param rName reference to a string object containing the new name
    */
    void setName(const string& rName)
    {
        name = rName;
    }			 

	virtual void setParent( DOM_Element & parElement );

	/** 
	 * adds a reference node to the list of references. Parents of 
	 * references need to be updated in case of an event generated.
	 * @param reference the reference node to add
	 */
	void addReference( Node * reference );
	
	/**
	 * removes a reference from the list.
	 * @param reference the reference node to remove
	 */
	void removeReference( Node * reference );

	// tree navigation interface
	
    /**
     * returns the number of children that are not wrapped, nor wrapper nodes.
     * That is the direct children a node may work with.
     * @returns unsigned number of children */
	unsigned int countChildren();

    /**
     * returns a child indicated by the index. This only returns children
     * that are not wrapped or Wrapper nodes themselves.
     * @param index unsigned number => 0 and < countChildren()
     * @returns pointer to the child node or NULL if index is out of range.
     */
	Node * getChild( unsigned int index );

    /**
     * returns the number of children contained by a certain wrapper node.
     * @param name the element name of the wrapper element
     * @returns unsigned number of children */
    unsigned int countWrappedChildren( const string & name );

    /**
     * returns a wrapped child by index.
     * @param name the element name of the wrapper element
     * @param index unsigned number => 0 and < countWrappedChildren
     * @returns pointer to the child node or NULL if index is out of range.
     */
	Node * getWrappedChild( const string & name, unsigned int index );

	// end tree navigation interface

public:
   /**
    * empty basic constructor.
    */
    Node();
    
   /**
    * basic destructor.
    */
    virtual ~Node();
	
	// begin EventGenerator & EventObserver interface

	/**
     * this method notifies the object that a new event was generated.
     * It is called by an EventGenerator.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event, Node& generator)
	{
	}

    /**
     * Updates the parent node and any reference nodes whenever a new
     * state is generated by the Event interface. This method should be
     * used to propagate new events, as it implements correctly the
     * event behaviour.
     * @param data reference to the event value */
	void updateObservers( State &data );	

    /**
     * tests for EventGenerator interface being implemented. This has to
     * be overriden in classes that subclass EventGenerator. Due to
     * inheritance raints it cannot be done automatically.
     * @returns a pointer to the EventGenerator interface, or NULL if it
     *   is not implemented
     */
    virtual int isEventGenerator()
    {
        return 0;
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
        return 0;
    }
	/**
     * returns a stored event by index. The index starts with 0 for the
     * latest event and goes back in time with increasing index.
     * @param index index of the desired event, if no index is given the
     *        latest is returned.
     * @return reference to the value of the event. */
    virtual State& getEvent(unsigned int index = 0)
	{
		return State::null;
	}
    /**
     * returns the event closest to a given point in time.
     * @param time the point in time in milliseconds since 1.1.1970
     * @return reference to the value of the event. */
    virtual State& getEventNearTime(double time)
	{
		return State::null;
	}
    /**
     * returns the number of stored events.
     * @return number of stored events. */
    virtual unsigned int getSize()
	{
		return 0;
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
        return 0;
    }

	/**
     * returns the state of the time dependend function at a given
     * point in time.
     * @param time point in time in milliseconds since 1.1.1970
     * @return reference to the state value */
    virtual State& getStateAtTime(double time)
	{
		return State::null;
	}

	// end TimeDependend interface

    /**
     * tests whether the node is a wrapper node. This method is only
     * there to distinguish wrapper nodes from other nodes.
     * @returns a pointer to the WrapperNode interface or NULL otherwise.
     * @warning Do not override this method ! This will result in your
     *          node being ignored by the parser.
     */
    virtual int isWrapperNode()
    {
        return 0;
    }

   /**
    * returns the value of member name.
    * @return reference to string object containing the name
    */
     string & getName() {
        return name;
    }

	friend class ConfigurationParser;
	friend class RefNode;
};

#endif
