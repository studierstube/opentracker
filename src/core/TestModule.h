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
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/TestModule.h,v 1.7 2001/04/18 16:38:18 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section testmodule TestModule
 * The TestModule provides and drives @ref testsource nodes that generate standard
 * events in certain intervals. It doesn't have a configuration element
 * but reserves the name 'TestConfig' for it. It does something useful as soon as
 * at least one @ref testsource node is created.
 */

#ifndef _TESTMODULE_H
#define _TESTMODULE_H

#include "../dllinclude.h"

#include <vector>

#include "Module.h"
#include "NodeFactory.h"

typedef vector<Node*> NodeVector;

/**
 * The module and factory to drive the test source nodes. It constructs
 * TestSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API TestModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of TestSource nodes in the tree
    NodeVector nodes;
    /// current cycle count, for computing when to fire a TestSource
    int cycle;

// Methods
public:
    /** constructor method. */
    TestModule() : Module(), NodeFactory()
    {
        cycle = 0;
    };
    /** Destructor method, clears nodes member. */
    virtual ~TestModule();
    /** This method is called to ruct a new Node. It compares
     * name to the TestSource element name, and if it matches
     * creates a new TestSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringTable& attributes);
    /**
     * pushes events into the tracker tree. Checks all TestSources and
     * pushes new events, if a TestSource fires.
     */
    virtual void pushState();
};

#endif
