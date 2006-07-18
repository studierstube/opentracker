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
  * For further information please contact Gerhard Schall under
  * <Schall@ims.tuwien.ac.at> or write to Gerhard Schall,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for OrientationSource Node.
  *
  * @author Gerhard Schall
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section OrientationSource OrientationSource
 * The OrientationSource node is a simple EventGenerator that outputs orientation data
 * from a single Orientation MT9-B orientation tracker. See the @ref OrientationModule for more information.
 * The configuration element has the following attributes :
 * @li @c comport the number of the serial port the tracker is connected to, starting with 1
 * @li @c amd (true|false) false whether to use the automatic adjustment to magnetic disturbances.
 *
 * An example element looks like this :
 * @verbatim
<OrientationSource/>@endverbatim
 *
 * The coordinate system of the orientation data is as follows. X and Y form the horizontal plane and
 * Z is upwards. The measurement cube itself has a similar body system and reports orientations from the
 * body system into the global system. To convert the readings into an Open Inventor like system use the
 * following configuration. Then X and Z form the horizontal plane and Y is upwards for both global and body system.
 * @verbatim
<EventTransform rotationtype="matrix" rotation="1 0 0 0 0 1 0 -1 0">
    <EventVirtualTransform rotationtype="matrix" rotation="1 0 0 0 0 -1 0 1 0">
        <Ref USE="Orientation"/>
    </EventVirtualTransform>
</EventTransform>@endverbatim
 *
 */

#ifndef _ORIENTSOURCE_H
#define _ORIENTSOURCE_H

#include "../OpenTracker.h"


namespace ot {


/**
 * The OrientationSource reads and propagates data from an Orientation MT9-B sensor.
 * @author Gerhard Schall
 * @ingroup input
 */
class OPENTRACKER_API OrientationSource : public Node 
{
public:

	/// the new event
    Event event;

    
     /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
    

protected:
	/// protected constructor so it is only accessible by the module
	OrientationSource() : Node()
	{}


    friend class OrientationModule;
};


}  // namespace ot


#endif // !defined(_OrientSource_H)
