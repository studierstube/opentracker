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
/** header file for Cybermouse module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/CyberMouseModule.h,v 1.8 2001/07/16 21:43:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section cybermousemodule CyberMouseModule
 * The CyberMouseModule provides and drives @ref cybermousesource nodes that 
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
 * @ingroup input
 */
class OPENTRACKER_API CyberMouseModule : public Module, public NodeFactory
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
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const string& name,  StringTable& attributes);
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
