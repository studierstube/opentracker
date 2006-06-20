/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institut for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** header file for TestModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: TestModule.h 760 2004-11-14 18:20:34Z daniel $
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

#ifndef _CORBAMODULE_H
#define _CORBAMODULE_H

#ifdef USE_CORBA

#include "../dllinclude.h"

#include <vector>

#include <OT_CORBA.hh>
#include "../core/Module.h"
#include "../core/NodeFactory.h"
#include "CORBASink.h"
#include "CORBASource.h"

namespace ot {

typedef std::vector<CORBASink*> CORBASinkVector;
//typedef std::vector<OT_CORBA::OTSource_var> SourceNodeVector;
 typedef std::map<CORBASource*, OT_CORBA::OTSource_var> SourceNodeMap;

/**
 * The module and factory to drive the test source nodes. It constructs
 * OTSource and OTSink nodes via the NodeFactory interface and pushes
 * events into the tracker tree according to the nodes configuration.
 * @author Joseph Newman
 * @ingroup network
 */

class OPENTRACKER_API CORBAModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of CORBASource nodes in the tree
    CORBASinkVector sinks;
    SourceNodeMap sources;
    bool persistent;

// Methods
public:
    /** constructor method. */
 CORBAModule() : Module(), NodeFactory()
    {
      // Constructor is currently blank
    };
    /** Destructor method, clears nodes member. */
    virtual ~CORBAModule();

    virtual void init(StringTable& attributes,  ConfigNode * localTree);

    /** initializes CORBA orb */
    void initializeORB(int argc, char **argv);

    /** block waiting for orb to be destroyed */
    void runORB();

    /** initializes CORBA orb */
    void destroyORB();

    PortableServer::POAManager_var pman;
    PortableServer::POA_var getPOA() { return poa; };
    
    /** Clears nodes */
    virtual void clear();
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
    virtual void pushEvent();
    
private:
    CORBA::ORB_var orb;
    CORBA::Object_var objref;
    PortableServer::POA_var poa;
    PortableServer::POA_var root_poa;

};

} // namespace ot

#endif //USE_CORBA

#endif
