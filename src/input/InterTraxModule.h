 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for InterTraxModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/Attic/InterTraxModule.h,v 1.3 2001/01/28 16:49:11 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page modules Module Reference
 * @section testmodule TestModule
 * The TestModule provides and drives TestSource nodes that generate standard
 * events in certain intervals. It doesn't have a configuration element
 * but reserves the name 'Test' for it. It does something useful as soon as
 * at least one TestSource node is created.
 */

#ifdef USE_INTERTRAX

#ifndef _INTERTRAXMODULE_H
#define _INTERTRAXMODULE_H

#include "../OpenTracker.h"
#include "isense.h"

/**
 * The module and factory to drive the test source nodes. It ructs
 * TestSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Gerhard Reitmayr
 */
class InterTraxModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of TestSource nodes in the tree
    NodeVector nodes;
	/// 
	ISD_TRACKER_HANDLE handle;
	///
	ISD_DATA_TYPE data;
 
// Methods
public:
    /** constructor method. */
    InterTraxModule() : Module(), NodeFactory()
    {};
    /** Destructor method, clears nodes member. */
    virtual ~InterTraxModule();
    /** This method is called to ruct a new Node. It compares
     * name to the TestSource element name, and if it matches
     * creates a new TestSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes);
	/**
     * closes intertrax library */
    virtual void close();
    /**
      */
    virtual void start();
    /**
     * pushes events into the tracker tree. Checks all TestSources and
     * pushes new events, if a TestSource fires.
     */
    virtual void pushState();
};

#endif

#endif
