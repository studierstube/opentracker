 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for TestModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/TestModule.h,v 1.3 2001/01/31 14:49:57 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section testmodule TestModule
 * The TestModule provides and drives TestSource nodes that generate standard
 * events in certain intervals. It doesn't have a configuration element
 * but reserves the name 'Test' for it. It does something useful as soon as
 * at least one TestSource node is created.
 */

#ifndef _TESTMODULE_H
#define _TESTMODULE_H

#include <vector>

#include "Module.h"
#include "NodeFactory.h"

/**
 * a Vector of Node pointers. Very useful to implement a simple
 * container of Nodes such as a parent node, or to keep pointers
 * to several nodes around.
 */
typedef vector<Node*> NodeVector;

/**
 * The module and factory to drive the test source nodes. It ructs
 * TestSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Gerhard Reitmayr
 */
class TestModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of TestSource nodes in the tree
    NodeVector nodes;
    /// current cycle count, for computing when to fire a TestSource
    int cycle;

// Methods
public:
    /** ructor method. */
    TestModule() : Module(), NodeFactory()
    {
        cycle = 0;
    };
    /** Destructor method, clears nodes member. */
    virtual ~TestModule();
    /** This method is called to ruct a new Node. It compares
     * name to the TestSource element name, and if it matches
     * creates a new TestSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes);
    /**
     * pushes events into the tracker tree. Checks all TestSources and
     * pushes new events, if a TestSource fires.
     */
    virtual void pushState();
};

#endif
