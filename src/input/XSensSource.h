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
/** header file for XSensSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section XSensSource XSensSource
 * The XSensSource node is a simple EventGenerator that outputs orientation data
 * from a single XSens MT9-B orientation tracker. See the @ref XSensModule for more information.
 * The configuration element has the following attributes :
 * @li @c comport the number of the serial port the tracker is connected to, starting with 1
 * @li @c amd (true|false) false whether to use the automatic adjustment to magnetic disturbances.
 *
 * An example element looks like this :
 * @verbatim
<XSensSource comport="1"/>@endverbatim
 *
 * The coordinate system of the orientation data is as follows. X and Y form the horizontal plane and
 * Z is upwards. The measurement cube itself has a similar body system and reports orientations from the
 * body system into the global system. To convert the readings into an Open Inventor like system use the
 * following configuration. Then X and Z form the horizontal plane and Y is upwards for both global and body system.
 * @verbatim
<EventTransform rotationtype="matrix" rotation="1 0 0 0 0 1 0 -1 0">
    <EventVirtualTransform rotationtype="matrix" rotation="1 0 0 0 0 -1 0 1 0">
        <Ref USE="xsens"/>
    </EventVirtualTransform>
</EventTransform>@endverbatim
 *
 */

#ifndef _XSENSSOURCE_H
#define _XSENSSOURCE_H

#include "../OpenTracker.h"

/// XSens driver's interface object
struct IMotionTracker;

/**
 * The XSensSource reads and propagates data from an XSens MT9-B sensor.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API XSensSource : public Node 
{
public:

	 /// the state that is posted to the EventObservers
    State state;
    
	/** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
    
    /// serial comport numbered from 0
    int comport;

    /// whether to use automatic adjustment to magnetic disturbances or not
    bool amd;

    virtual ~XSensSource();

protected:
	/// protected constructor so it is only accessible by the module
	XSensSource(int _comport = 1, bool _amd = false);

    void push(void);

    IMotionTracker * pMT;

    friend class XSensModule;
};

#endif // !defined(_XSensSource_H)
