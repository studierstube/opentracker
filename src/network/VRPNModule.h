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
/** header file for VRPN module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section vrpnmodule VRPNModule
 * The VRPN module provides the @ref vrpnsource and @ref vrpnsink nodes. It 
 * provides a bi-directional link between VRPN (http://www.vrpn.org/) and 
 * OpenTracker because it can act as both a server and a client. This also
 * enables the use of OpenTracker as a configurable transformation server 
 * within a VRPN network.
 *
 * @note Under Windows, the default server port is already occupied. Therefore
 * use the configuration element to set the port number differently, for example
 * to 3883 the official IANA number for VRPN!
 *
 * The configuration element is @c VRPNConfig and has the following attributes :
 * @li @c port port number to listen for new connections
 * @li @c interface ip address of network interface to use for incoming connections
 *
 * An example configuration element looks like this :
 * @verbatim<VRPNConfig port="3883" interface="localhost"/>@endverbatim
 */

#ifndef _VRPNMODULE_H
#define _VRPNMODULE_H

#include "../OpenTracker.h"

#ifdef USE_VRPN

class vrpn_Connection;
class VRPNSource;
class VRPNSink;

typedef std::vector<VRPNSource *> VRPNSourceVector;
typedef std::vector<VRPNSink *> VRPNSinkVector;

/**
 * The module and factory to drive the VRPN nodes. It constructs
 * VRPNNode nodes via the NodeFactory interface and registers
 * VRPNs with them after parsing
 * @author Gerhard Reitmayr
 * @ingroup network
 */
class OPENTRACKER_API VRPNModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of sink nodes
    VRPNSinkVector sinks;
    /// list of source nodes
    VRPNSourceVector sources;
    /// interface for VRPNSink servers to use
    std::string ip;
    /// port for VRPNSink servers to use
    int port;
    /// Connection object for VRPNSink servers
    vrpn_Connection * connection;

// Methods
public:
    /** constructor method. */
    VRPNModule();

    /** Destructor method, clears nodes member. */
    virtual ~VRPNModule();    

    virtual void init(StringTable& attributes,  ConfigNode * localTree);

    /** This method is called to construct a new Node. It compares
     * name to the VRPNSink and VRPNSource element name, and if it matches
     * creates a new VRPNSink or VRPNSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);

    virtual void start();

    virtual void pullState();

    virtual void pushState();

};

#endif

#endif
