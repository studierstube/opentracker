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
/** header file for VRPNSource node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section vrpnsource VRPNSource
 *
 * The VRPNSource node acts as a client for a VRPN server. It supports receiving 
 * either a single tracker station or a button server with up to 8 buttons. The
 * attribute @c type selects between the two modes. The @c station attribute selects
 * the tracker station to report on. The most important attribute is @name which
 * specifies the server device to connect to in the usual VRPN format. See 
 * http://www.vrpn.org/ for details on VRPN. The @ref vrpnmodule manages this node.
 *
 * It has the following attributes :
 * @li @c name the VRPN server device name, something like "tracker0@somehost:12345"
 * @li @c type (tracker|button) decides whether to connect using the tracker or the button interface
 * @li @c station to select the tracker station to report on. starts with 0.
 *
 * An example element looks like this :
 * @verbatim<VRPNSource name="tracker0@somehost:12345" type="tracker" station="0"/>@endverbatim
 */

#ifndef _VRPNSOURCE_H
#define _VRPNSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_VRPN

class vrpn_BaseClass;

namespace ot {

/**
 * VRPN client interface node. Connects to a VRPN server and reports incoming 
 * tracking data.
 * @author Gerhard Reitmayr
 * @ingroup network
 */
class OPENTRACKER_API VRPNSource : public Node
{
public:
    /// name
    std::string name;
    /// type of connection
    enum Type { TRACKER, BUTTON } type;
    /// station number of station to report
    int station;
    /// state object for data flow
    State state;
    
protected:

    /// data pointer to underlying vrpn object
    vrpn_BaseClass * trackerObj;

    /** constructor method,sets commend member */
    VRPNSource();

    /** Opens connection to the VRPN server. Only for internal use
      * by the associated module. 
      */
    void start();

    /** Executes the vrpn object's mainloop. Only for internal use
      * by the associated module. 
      */
    void mainloop();    

public:
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
    
    /// destructor
    virtual ~VRPNSource();

    friend class VRPNModule;
};

} // namespace ot

#endif

#endif
