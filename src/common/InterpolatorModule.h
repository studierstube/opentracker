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
/** header file for TestModule module.
  *
  * @author Flo Ledermann
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section interpolatormodule InterpolatorModule
 * Module for filters that interpolate tracking data between updates and therefore need
 * a module to drive the updates. Currently, only the @ref ElasticFilterNode is in this module.
 * It doesn't have a configuration element
 * but reserves the name 'InterpolatorConfig' for it.
 */

#ifndef _INTERPOLATORMODULE_H
#define _INTERPOLATORMODULE_H

#include "../dllinclude.h"

#include <vector>

#include "../core/Module.h"
#include "../core/NodeFactory.h"

namespace ot {

typedef std::vector<Node*> NodeVector;

/**
 * Module for filters that interpolate tracking data between updates and therefore need
 * a module to drive the updates. Currently, only the @ref ElasticFilterNode is in this module.
 * @author Flo Ledermann
 * @ingroup common
 */
class OPENTRACKER_API InterpolatorModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of interpolator nodes in the tree
    NodeVector nodes;
    /// current cycle count.
    int cycle;

// Methods
public:
    /** constructor method. */
    InterpolatorModule() : Module(), NodeFactory()
    {
        cycle = 0;
    };
    /** Destructor method, clears nodes member. */
    virtual ~InterpolatorModule();
    /** This method is called to build a new Node. 
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
    /**
     * pushes events into the tracker tree. Checks all sources and
     * pushes new events.
     */
    virtual void pushState();
};

} // namespace ot

#endif //_INTERPOLATORMODULE_H
