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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for InterTraxModule module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/Attic/InterTraxModule.h,v 1.13 2001/07/16 21:43:52 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section intertraxmodule InterTraxModule
 *
 * @deprecated This module is deprecated and should not be used anymore. A
 * better alternative is the @ref intersensemodule that supports all 
 * InterSense devices.
 *
 * The InterTraxModule provides and drives @ref intertraxsource nodes that 
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
 * @ingroup input
 */
class OPENTRACKER_API InterTraxModule : public Module, public NodeFactory
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
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const string& name,  StringTable& attributes);
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
