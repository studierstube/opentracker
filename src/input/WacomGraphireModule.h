 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for WacomGraphireModule module. 
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/WacomGraphireModule.h,v 1.3 2001/03/27 05:35:18 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section wacomgraphiremodule WacomGraphireModule
 * The WacomGraphireModule provides and drives WacomGraphireSource nodes that 
 * generate standard events in certain intervals. It does not use a 
 * configuration element but reserves the name 'WacomGraphireConfig'. 
 */

#ifndef _WACOMGRAPHIREMODULE_H
#define _WACOMGRAPHIREMODULE_H

#include "../OpenTracker.h"

#ifdef USE_WACOMGRAPHIRE

/**
 * The module and factory to drive the wacomgraphire source nodes. It ructs
 * WacomGraphireSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Ivan Viola, Matej Mlejnek
 */
class OPENTRACKER_API WacomGraphireModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of WacomGraphireSource nodes in the tree
    NodeVector nodes;
// Methods
public:
    /** constructor method. */
    WacomGraphireModule() : Module(), NodeFactory()
    {};
    /** Destructor method, clears nodes member. */
    virtual ~WacomGraphireModule();
    /** This method is called to ruct a new Node. It compares
     * name to the WacomGraphireSource element name, and if it matches
     * creates a new WacomGraphireSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes);
	/**
     * closes WacomGraphire static library */
    virtual void close();
    /**
	  * opens WacomGraphire static library (WINTAB32.LIB)
      */
    virtual void start();
    /**
     * pushes events into the tracker tree. Checks all WacomGraphireSources and
     * pushes new events, if a WacomGraphireSource with particular device fires.
	 * The events store position, pressure and status of the buttons of the 
	 * Wacom pointing device.
     */
    virtual void pushState();
};

#endif

#endif