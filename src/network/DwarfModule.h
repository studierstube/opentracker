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
/** header file for DWARFMODULE module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/DwarfModule.h,v 1.3 2003/07/24 15:37:47 anonymous Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section DWARFMODULE DWARFMODULE
 * The DWARFMODULE provides and drives @ref testsource nodes that generate standard
 * events in certain intervals. It doesn't have a configuration element
 * but reserves the name 'TestConfig' for it. It does something useful as soon as
 * at least one @ref testsource node is created.
 */

#ifndef _DWARFMODULE_H
#define _DWARFMODULE_H

#include "../dllinclude.h"

#include <vector>
#include <map>

#include "../core/Module.h"
#include "../core/NodeFactory.h"

class DwarfSink;
class DwarfSource;
namespace DWARF{class PoseSenderService;}
class CorbaInit;

/**
 * The module and factory to drive the test source nodes. It constructs
 * TestSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API DwarfModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of TestSource nodes in the tree
    std::vector<DwarfSink *> sinks;

    std::vector<DwarfSource *> sources;

    std::map<std::string, std::string> thingIdMap;

    std::map<std::string, std::string> thingTypeMap;

    DWARF::PoseSenderService * service;

    CorbaInit * myOrbConnection;

// Methods
public:
    /** constructor method. */
    DwarfModule() : Module(), NodeFactory()
    {};
    /** Destructor method, clears nodes member. */
    virtual ~DwarfModule();

    virtual void init(StringTable & attributes, ConfigNode * localTree);

    /** This method is called to ruct a new Node. It compares
     * name to the TestSource element name, and if it matches
     * creates a new TestSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
    /**
     * pushes events into the tracker tree. Checks all TestSources and
     * pushes new events, if a TestSource fires.
     */
    virtual void pushState();

    virtual void pullState();
};

#endif
