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
  * @author Gerhard Reitmayr, Christian Sandor, Martin Bauer
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/DwarfModule.h,v 1.4 2003/07/27 10:31:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section dwarfmodule DwarfModule
 * The DwarfModule 
 */

#ifndef _DWARFMODULE_H
#define _DWARFMODULE_H

#include "../OpenTracker.h"

#ifdef USE_DWARF

#include <vector>
#include <map>

class DwarfSink;
class DwarfSource;
class CorbaInit;
namespace DWARF {
    class PoseSenderService;
}

/**
 * The module and factory to drive the test source nodes. It constructs
 * TestSource nodes via the NodeFactory interface and pushes events into
 * the tracker tree according to the nodes configuration.
 * @author Gerhard Reitmayr, Christian Sandor, Martin Bauer
 * @ingroup network
 */
class OPENTRACKER_API DwarfModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of DwarfSinks in the OpenTracker graph
    std::vector<DwarfSink *> sinks;
    /// list of DwarfSources in the OpenTracker graph
    std::vector<DwarfSource *> sources;

    /** maps ability name to ThingId to store information.
     * Will be obsolete, if ThingId becomes an attribute. 
     */
    std::map<std::string, std::string> thingIdMap;

    /** maps ability name to ThingType to store information.
     * Will be obsolete, if ThingType becomes an attribute. 
     */
    std::map<std::string, std::string> thingTypeMap;

    /// encapsulates all PoseData abilities of the service
    DWARF::PoseSenderService * service;

    /// corba initialization, should be a singleton
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

#endif // USE_DWARF

#endif
