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
/** header file for WacomGraphireModule module. 
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/WacomGraphireModule.h,v 1.8 2001/07/23 14:08:01 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section wacomgraphiremodule WacomGraphireModule
 * The WacomGraphireModule provides and drives @ref wacomgraphiresource nodes that 
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
 * @ingroup input
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
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
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
