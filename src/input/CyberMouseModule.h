 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Cybermouse module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/CyberMouseModule.h,v 1.2 2001/03/06 18:08:59 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section cybermousemodule CyberMouseModule
 * The CyberMouseModule provides and drives CyberMouseSource nodes that 
 * generate standard events in certain intervals. It does not use a
 * configuration element, but reserves the name 'CyberMouseConfig'.
 */

#ifndef _CYBERMOUSEMODULE_H
#define _CYBERMOUSEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_CYBERMOUSE

/**
 * The module and factory to drive the CyberMouseSource nodes. It constructs
 * CyberMouseSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Ivan Viola, Matej Mlejnek
 */
class CyberMouseModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of CyberMouseSource nodes in the tree
    NodeVector nodes;

// Methods
public:
    /** constructor method. */
    CyberMouseModule() : Module(), NodeFactory()
    {};
    /** Destructor method, clears nodes member. */
    virtual ~CyberMouseModule();
    /** This method is called to ruct a new Node. It compares
     * name to the CyberMouseSource element name, and if it matches
     * creates a new CyberMouseSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes);
	/**
     * closes CyberMouse dynamic library */
    virtual void close();
    /**
	  * opens CyberMouse dynamic library (FREEDDLL.DLL)
      */
    virtual void start();
    /**
     * pushes events into the tracker tree. Checks all CyberMouseSources and
     * pushes new events, if a CyberMouseSource fires. The events store
	 * structure with position and status of the buttons.
     */
    virtual void pushState();
};

#endif

#endif