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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Node.h,v 1.18 2002/09/17 17:59:40 reitmayr Exp $
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

// using namespace std;

class ConfigurationParser;
class RefNode;
class DOMElement;
class Node;
class NodePort;

/**
 * a Vector of Node pointers. Very useful to implement a simple
 * container of Nodes such as a parent node, or to keep pointers
 * to several nodes around.
 */
typedef std::vector<Node *> NodeVector;

#include "State.h"

/**
 * The basic superclass for all nodes. It implements several interfaces
 * necessary to deal with nodes. Moreover it provides the link to the
 * underlying XML Document tree and stores information such as the
 * type (i.e. element name) and a name (i.e. unique ID) of the node.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API Node
{

protected:    
	/** Pointer to the parent XML element. */
	DOMElement * parent;

	/**  A Vector of pointers to reference nodes referencing this node. */
	NodeVector references;

    /// the unique ID given in the XML configuration file.
    std::string name;
    
    /// the type of the node, equals the name of the configuration element
    std::string type;
    
public:
    enum error { OK=0, GRAPH_CONSTRAINT, READONLY, CONTEXT_ERROR, NOT_FOUND };

protected:

	virtual void setParent( DOMElement * parElement );

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

    /**
     * empty basic constructor. The constructors of any nodes should be
     * protected to avoid application code to directly construct new
     * instances. Any new nodes should be generated via valid API
     * calls such as Context::createNode() to ensure that internal
     * data is correct. Any NodeFactories needing access should be
     * declared friend classes of the nodes.
     */
    Node();
    
    /** @name Attributes Control Interface
     * This set of methods allows to manipulate the attributes of a node
     * in a safe and abstract manner. These methods operate on the
     * attributes of the underlying XML element. Whenever an attribute changes
     * the implementation of the node should take care to update the
     * XML elements attributes as well.*/
    //@{
    
    /** returns a value stored in the map
     * @param key the key the value is for 
     * @returns the string value or an empty string, if the key is not found */
    std::string get( const std::string & key );
    /** stores a key value pair in the table, overwritting a possible prior 
     * value
     * @param key the key to store it under
     * @param value the value to store */
    void put( const std::string &key, const std::string &value );
    /**
     * removes a key value pair from the table
     * @param key the key to the pair */
    void remove( const std::string &key );
 
    /** 
     * sets an int value, the value is converted to a string and stored
     * under the given key.
     * @param key key string of the entry
     * @param value the integer value to store
     */
    void put(const std::string & key, const int value);
    /** 
     * sets a float value, the value is converted to a string and stored
     * under the given key.
     * @param key key string of the entry
     * @param value the floating point value to store
     */
    void put(const std::string & key, const float value);
    /** 
     * sets a double value, the value is converted to a string and stored
     * under the given key.
     * @param key key string of the entry
     * @param value the double precision value to store
     */
    void put(const std::string & key, const double value);

    /** 
     * sets an array of int values. It is converted to a string where the
     * values are separated by spaces and stored
     * under the given key.
     * @param key key string of the entry
     * @param value pointer to the array of integer values to store
     * @param len the length of the array
     */    
    void put(const std::string & key, const int * value, int len);
    /** 
     * sets an array of float values. It is converted to a string where the
     * values are separated by spaces and stored
     * under the given key.
     * @param key key string of the entry
     * @param value pointer to the array of floating point values to store
     * @param len the length of the array
     */
    void put(const std::string & key, const float * value, int len);
    /** 
     * sets an array of double values. It is converted to a string where the
     * values are separated by spaces and stored
     * under the given key.
     * @param key key string of the entry
     * @param value pointer to the array of double values to store
     * @param len the length of the array
     */
    void put(const std::string & key, const double * value, int len);

    /**
     * parses a stored entry into an array of integers. It assumes that the
     * integers are separated by spaces. It returns the number of actually
     * converted integers.
     * @param key the key of the entry to parse
     * @param value pointer to the array of integers to store the parsed values 
     * @param len the length of the array, default 1 to use it for a single int only
     * @return number of actually parsed values
     */
    int get(const std::string & key, int * value, int len = 1 );
    /**
     * parses a stored entry into an array of floats. It assumes that the
     * floats are separated by spaces. It returns the number of actually
     * converted floats.
     * @param key the key of the entry to parse
     * @param value pointer to the array of integers to store the parsed values 
     * @param len the length of the array, default 1 to use it for a single float only
     * @return number of actually parsed values
     */
    int get(const std::string & key, float * value, int len = 1 );
    /**
     * parses a stored entry into an array of doubles. It assumes that the
     * doubles are separated by spaces. It returns the number of actually
     * converted doubles.
     * @param key the key of the entry to parse
     * @param value pointer to the array of doubles to store the parsed values 
     * @param len the length of the array, default 1 to use it for a single double only
     * @return number of actually parsed values
     */
    int get(const std::string & key, double * value, int len = 1 );

    //@}

public:

   /**
    * basic destructor.
    */
    virtual ~Node();

    /**
     * returns the type of the node, that is the element name of the underlying
     * configuration element. An implementation can then use this information 
     * to down cast a node reference or pointer to the correct class. 
     * @return string containing the node type
     */
    const std::string & getType() const
    {
        return type;
    }

    /**
     * returns the value of a unique ID set on the node. If no
     * ID is set, this method returns an empty string
     * @return string containing the unique ID
     */
    const std::string & getName() const
    {
       return name;
    }

    /** @name Graph Navigation Interface
     * This set of methods allows to manipulate the data flow graph in a
     * rather safe manner.*/
    //@{

    /**
     * returns a pointer to the parent node of the current node. This can
     * be a wrapper node to mark a certain input port of a real node. In this
     * case, getting the wrapper nodes parent will yield the true parent. The
     * root node will return NULL, because it has no parent.
     * @return pointer to parent node
     */
     Node * getParent();

    /**
     * returns the number of children that are not wrapped, nor wrapper nodes.
     * That is the direct children a node may work with.
     * @return unsigned number of children */
	unsigned int countChildren();

    /**
     * returns a child indicated by the index. This only returns children
     * that are not wrapped or Wrapper nodes themselves.
     * @param index unsigned number => 0 and < countChildren()
     * @return pointer to the child node or NULL if index is out of range.
     */
	Node * getChild( unsigned int index );

    /** 
     * adds a new child to the direct children of the node. This method
     * will only work, if it does not violate any rules for the graph.
     * @todo add detailed rules and error code documentation
     * @param child the new child node to add
     * @return error code describing outcome of the operation
     */
    error addChild(Node & child);

    /**
     * removes a child from the direct children of a node. This method
     * will only work, if the passed node is actually a child of the
     * node.     
     * @param child the child node to remove
     * @return error code describing outcome of the operation 
     */
    error removeChild(Node & child);

    /**
     * returns the number of NodePorts present on this Node. This is the
     * number of NodePorts actually used, not the total number possible 
     * by the content definition.
     * @returns unsigned number of NodePorts */
    unsigned int countPorts();

    /**
     * returns a NodePort child object indexed by Name. If the NodePort is
     * not present, NULL is returned.
     * @param name the element name of the NodePort
     * @returns pointer to the child NodePort or NULL if none of this name is present.
     */
	NodePort * getPort( const std::string & name );
    
    /**
     * returns a NodePort child object by index. The order of the NodePorts is not fixed
     * but may depend on the configuration file used.
     * @param index unsigned number => 0 and < countWrappedChildren
     * @returns pointer to the child NodePort or NULL if index is out of range.
     */    
    NodePort * getPort( unsigned int index );

    /**
     * creates and adds a new child NodePort object of the given name.
     * It will also only work, if it does not violate any rules for the graph.
     * @param name the name of the NodePort
     * @return error code describing outcome of the operation
     */
    error addPort( const std::string & name );

    /**
     * removes a child NodePort object. Again similar to removeChild(), but
     * for NodePorts. This method will only work, 
     * if a NodePort of the passed name is present on the node.
     * @param name the name of the wrapper element
     * @return error code describing outcome of the operation
     */ 
    error removePort( const std::string & name );
    
    error removePort( NodePort & port);
    
    error removePort( unsigned int index );

    //@}
	
    /** @name EventGenerator & EventObserver Interface
     * This is the classic event passing interface, working from children nodes
     * up to parent nodes. A child node creates a new event and its parent node 
     * is automatically notified of the event. 
     * The type of an event is of class State. 
     */
    //@{

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

	//@}

	/** @name EventQueue Interface
     * The EventQueue interface allows access to a queue of events, ordered
     * by their timestamps, with the latest event coming first, at slot 0. 
     * This works from parents to children, i.e. parents query their children 
     * for a certain event in the queue.
     */
     
    //@{

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

	//@}

	/** @name TimeDependend Interface
     * The TimeDependend Interface allows access to a continous function of 
     * states in time. It also works from parents to children, i.e. a parent 
     * queries a child for the state at a given point in time.
     */
    //@{

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

	//@}

    /**
     * tests whether the node is a wrapper node. This method is only
     * there to distinguish wrapper nodes from other nodes.
     * @returns a pointer to the WrapperNode interface or NULL otherwise.
     * @warning Do not override this method ! This will result in your
     *          node being ignored by the parser.
     */
    virtual int isNodePort()
    {
        return 0;
    }

    friend class Context;
	friend class ConfigurationParser;
	friend class RefNode;
    friend class XMLWriter;
};

#endif
