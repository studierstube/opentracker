 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** The header file for the basic Node class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Node.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _NODE_H
#define _NODE_H

#include <string>

using namespace std;

#include "EventGenerator.h"
#include "EventQueue.h"
#include "TimeDependend.h"

// forward declaration to avoid cyclic definition problems
class WrapperNode;

/**
 * The basic superclass for all nodes. It defines the the basic interfaces
 * to set children and to query its type at runtime. Moreover a name
 * attribute for a unique ID is there but not used yet.
 * @author Gerhard Reitmayr
 */
class Node
{

// Members
public:
    /// the unique ID given in the XML configuration file. Not used yet.
    string name;

protected:
   /**
    * sets the value of name.
    * @param rName reference to a string object containing the new name
    */
    void setName( string& rName)
    {
        name = rName;
    }

public:
   /**
    * empty basic ructor.
    */
    Node()
    {}
   /**
    * empty basic ructor.
    */
    virtual ~Node()
    {}
    /**
     * adds a child to the Node. This is just an empty method,
     * the derived class has to implement what to do with the child.
     * @param node reference to the child node
     */
    virtual void addChild(Node& node)
    {}
    /**
     * adds a wrapped node to the node. That is a child that is tagged by
     * a name. In the XML file this is represented by a wrapper tag around
     * a single or group of children. Again this is just an empty method.
     * @param node reference to the child node
     * @param wrapper reference to a string object containing wrapper name
     */
    virtual void addWrappedChild(Node& node, string& wrapper)
    {}
    /**
     * tests for EventGenerator interface being implemented. This has to
     * be overriden in classes that subclass EventGenerator. Due to
     * inheritance raints it cannot be done automatically.
     * @returns a pointer to the EventGenerator interface, or NULL if it
     *   is not implemented
     */
    virtual EventGenerator * isEventGenerator()
    {
        return NULL;
    }
    /**
     * tests for EventQueue interface being present. This has to be overriden in
     * classes that subclass EventQueue. See isEventGenerator for details.
     * @returns a pointer to the EventQueue interface, or NULL if it is not
     *       implemented.
     */
    virtual EventQueue * isEventQueue()
    {
        return NULL;
    }
    /**
     * tests for TimeDependend interface being present. This has to be overriden
     * in classes that subclass TimeDependend. See isEventGenerator for details.
     * @returns a pointer to the TimeDependend interface, or NULL if it is not
     *      implemented.
     */
    virtual TimeDependend * isTimeDependend()
    {
        return NULL;
    }
    /**
     * tests whether the node is a wrapper node. This method is only
     * there to distinguish wrapper nodes from other nodes.
     * @returns a pointer to the WrapperNode interface or NULL otherwise.
     * @warning Do not override this method ! This will result in your
     *          node being ignored by the parser.
     */
    virtual WrapperNode * isWrapperNode()
    {
        return NULL;
    }
   /**
    * returns the value of member name.
    * @return reference to string object containing the name
    */
     string & getName() {
        return name;
    }
};

#endif
