 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for WacomGraphireModule module. 
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/WacomGraphireModule.h,v 1.1 2001/03/05 17:53:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section wacomgraphiremodule WacomGraphireModule
 * The WacomGraphireModule provides and drives WacomGraphireSource nodes that 
 * generate standard events in certain intervals. It does not use a 
 * configuration element but reserves the name 'WacomGraphire'. 
 */

#ifndef _WACOMGRAPHIREMODULE_H
#define _WACOMGRAPHIREMODULE_H

#include "../OpenTracker.h"

#ifdef USE_WACOMGRAPHIRE

#include <wintab.h>
#include <pktdef.h>

/** What Wintab packet data items we want.  PK_CURSOR identifies which cursor 
 * (0-5) generated the packet.  PACKETDATA must be defined before including
 * pktdef.h 
 */

#define	PACKETDATA	( PK_X | PK_Y | PK_Z | PK_CURSOR | PK_BUTTONS | PK_NORMAL_PRESSURE )

/**
 * The module and factory to drive the wacomgraphire source nodes. It ructs
 * WacomGraphireSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Ivan Viola, Matej Mlejnek
 */
class WacomGraphireModule : public Module, public NodeFactory
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