 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for InterTraxModule module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/Attic/InterTraxModule.h,v 1.7 2001/03/06 18:08:59 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section intertraxmodule InterTraxModule
 * The InterTraxModule provides and drives InterTraxSource nodes that 
 * generate standard events in certain intervals. It does not use a 
 * configuration elements but reserves the name 'InterTraxConfig'. 
 */

#ifndef _INTERTRAXMODULE_H
#define _INTERTRAXMODULE_H

#include "../OpenTracker.h"

#ifdef USE_INTERTRAX

#include "isense.h"

/**
 * The module and factory to drive the intertrax source nodes. It ructs
 * InterTraxSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Ivan Viola, Matej Mlejnek
 */
class InterTraxModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of InterTraxSource nodes in the tree
    NodeVector nodes;
    /// InterTrax handle
    ISD_TRACKER_HANDLE handle;
    /// InterTrax Tracker data structure
    ISD_DATA_TYPE data;
    /// backup data to compare with new data
    float backup[3];

// Methods
public:
    /** constructor method. */
    InterTraxModule() : 
        Module(), 
        NodeFactory()
    {
        backup[0] = 0;
        backup[1] = 0;
        backup[2] = 0;
    };
    /** Destructor method, clears nodes member. */
    virtual ~InterTraxModule();
    /** This method is called to ruct a new Node. It compares
     * name to the InterTraxSource element name, and if it matches
     * creates a new InterTraxSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes);
	/**
     * closes InterTrax dynamic library */
    virtual void close();
    /**
      * opens InterTrax dynamic library (ISENSE.DLL)
      */
    virtual void start();
    /**
     * pushes events into the tracker tree. Checks all InterTraxSources and
     * pushes new events, if a InterTraxSource fires. The events store
	 * orientation of the InterTrax in quaternion.
     */
    virtual void pushState();
};

#endif

#endif
