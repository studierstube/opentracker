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
/** header file for WacomGraphireSource Node.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section wacomgraphiresource WacomGraphireSource
 * The WacomGraphireSource node is a simple EventGenerator that outputs the
 * the position of the pen on the tablet in the first two coordinates of
 * the position value and the button status of the used input device.
 * The node itself can be configured to report the position of a
 * particular device, such as pen, eraser or puck.
 * It has the following attributes :
 * @li @c device, type of pointing device (pen, eraser, puck)
 *
 * An example element looks like this :
 * @verbatim
<WacomGraphireSource device="1" />@endverbatim
 */

#ifndef _WACOMGRAPHIRESOURCE_H
#define _WACOMGRAPHIRESOURCE_H

#include "../OpenTracker.h"

#ifdef USE_WACOMGRAPHIRE

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Ivan Viola, Matej Mlejnek
 * @ingroup input
 */

namespace ot {

class OPENTRACKER_API WacomGraphireSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;
    /// the number of the device to use
	int device;
// Methods
public:
    /** simple constructor, sets members to initial values */
    WacomGraphireSource(int device_) : 
        Node(),
	    device(device_)
    {}
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
};

} // namespace ot

#endif

#endif
