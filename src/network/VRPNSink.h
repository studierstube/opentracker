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
/** header file for VRPNSink node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section vrpnsink VRPNSink
 *
 * The VRPNSink node acts as a single tracking server device using VRPN. It 
 * provides its data via a single connection setup by the @ref vrpnmodule.
 * Therefore a single VRPNSink node is identifyable by the @c name attribute
 * that specifies the device name of the node. The @c type field selects the type
 * of device to be, either a tracker for 6DOF information or a button with up to 
 * 8 buttons. The values correspond to the OpenTracker event type. See 
 * http://www.vrpn.org/ for details on VRPN.
 * 
 * It has the following attributes :
 * @li @c name the VRPN device name to distinguish it from other devices on the server
 * @li @type @c (tracker|button) to configure the type of device to be
 * @li @station the station number to report on for a tracker device
 *
 * An example element looks like this :
 * @verbatim<VRPNSink name="tracker3" type="tracker" station="2">
    <Any EventGenerator element type>
</VRPNSink>@endverbatim
 */

#ifndef _VRPNSINK_H
#define _VRPNSINK_H

#include "../OpenTracker.h"

#ifdef USE_VRPN

class vrpn_BaseClass;
class vrpn_Connection;

namespace ot {

/** 
 * The node representing a VRPN server device. 
 * @author Gerhard Reitmayr
 * @ingroup network
 */
class OPENTRACKER_API VRPNSink : public Node
{
// Members
public:
    /// name
    std::string name;
    /// station number
    int station;
    /// type of connection
    enum Type { TRACKER, BUTTON } type;    

// Methods
protected:
    
    /// the VRPN server object
    vrpn_BaseClass * server;

    /** constructor method */
    VRPNSink();

    /** 
     * create the required VRPN device server objects attached to the
     * local server connection
     */
    void start(vrpn_Connection * connection);

    /** 
     * call VRPN mainloop
     */
    void mainloop();

public:       
    
    /// destructor
    virtual ~VRPNSink();

    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
    
    /**
     * this method notifies the object that a new event was generated.
     * It stores a copy of the received event and passes the event on
     * to its observers.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( State& event, Node& generator);

    friend class VRPNModule;
};

} // namespace ot

#endif

#endif
