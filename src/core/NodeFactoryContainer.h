 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for NodeFactoryContainer class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/NodeFactoryContainer.h,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _NODEFACTORYCONTAINER_H
#define _NODEFACTORYCONTAINER_H

#include "../dllinclude.h"

#include "NodeFactory.h"

/**
 * used to keep a list of NodeFactories.
 */
typedef std::vector<NodeFactory*> NodeFactoryVector;

/**
 * An implementation of NodeFactory that contains several NodeFactories and
 * acts like the one providing all node types of the different factories
 * together. When createNode is called it loops through the known
 * NodeFactories and calls createNode on them until it receives a non NULL
 * result. It then returns this node.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API NodeFactoryContainer : public NodeFactory
{

protected:
    /// the list of known NodeFactories
    NodeFactoryVector factories;

public:
    /** Destructor clears the factories list. */
    virtual ~NodeFactoryContainer();
    /**
     * adds a NodeFactory to the container making nodes provided by this
     * factory accessible.
     * @param factory reference to the NodeFactory object */
    void addFactory(NodeFactory& factory);
    /**
     * removes a NodeFactory from the container.
     * @param factory reference to the NodeFactory object */
    void removeFactory(NodeFactory& factory);
    /**
     * implements abstract method from NodeFactory. it loops through the known
     * NodeFactories and calls createNode on them until it receives a non NULL
     * result. It then returns this node. */
    virtual Node * createNode( string& name,  StringMap& attributes);
};

#endif

