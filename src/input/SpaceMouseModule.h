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
/** header file for Spacemouse module.
  *
  * @author Michael Woegerbauer
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpaceMouseModule.h,v 1.2 2002/11/07 17:02:02 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section spacemousemodule SpaceMouseModule
 * The SpaceMouseModule provides and drives @ref spacemousesource nodes that 
 * generate standard events in certain intervals. It does not use a
 * configuration element, but reserves the name 'SpaceMouseConfig'.
 */

#ifndef _SPACEMOUSEMODULE_H
#define _SPACEMOUSEMODULE_H

#include "../OpenTracker.h"

#ifdef USE_SPACEMOUSE

#include <Windows.h>

/**
 * The module and factory to drive the SpaceMouseSource nodes. It constructs
 * SpaceMouseSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * The min/max values for each axis are:
 * -1 / +1 for position and
 * -90 / +90 degrees for orientation,
 * when the SpaceMouse sensitivity is set to factory defaults.
 * @author Michael Woegerbauer
 * @ingroup input
 */
class OPENTRACKER_API SpaceMouseModule : public ThreadModule, public NodeFactory
{
// Members
protected:
    /// list of SpaceMouseSource nodes in the tree
    NodeVector nodes;

// Methods
public:
    /** constructor method. */
    SpaceMouseModule() : ThreadModule(), NodeFactory(), stop(0)
    {};
    /** Destructor method, clears nodes member. */
    virtual ~SpaceMouseModule();
    /** This method is called to construct a new Node. It compares
     * name to the SpaceMouseSource element name, and if it matches
     * creates a new SpaceMouseSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
	/**
     * closes SpaceMouse dynamic library */
    virtual void close();
    /**
	  * opens SpaceMouse dynamic library (SIAPPDLL.DLL)
      */
    virtual void start();


	virtual
		void run();

    /// flag to stop the thread
    int stop;
    /**
     * pushes events into the tracker tree. Checks all SpaceMouseSources and
     * pushes new events, if a SpaceMouseSource fires. The events store
	 * structure with position and status of the buttons.
     */
    virtual void pushState();
	static HWND		hWndSpaceMouse;

private:
	void processMessages();

};

#endif

#endif


