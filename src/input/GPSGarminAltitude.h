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
/** header file for GPSGarminAltitude Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section GPSGarminAltitude GPSGarminAltitude
 * The GPSGarminAltitude node is a simple EventGenerator that outputs GPS 
 * altitude data from a Garmin device interpreting the propriatory $PGRMZ string.
 * The altitude is given as a vector with the altitude encoded in the positive Y 
 * direction in meters. It is managed by the @ref gpsmodule, see there for
 * more information on how to configure GPS support.
 *
 * An example element looks like this :
 * @verbatim
<GPSGarminAltitude/>@endverbatim
 */

#ifndef _GPSGARMINALTITUDE_H
#define _GPSGARMINALTITUDE_H

#include "../OpenTracker.h"
#include "GPSDriver.h"

/**
 * A very simple EventGenerator node for outputing GPS direction and velocity data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API GPSGarminAltitude : public Node, public GPSListener  
{
public:

	 /// the state that is posted to the EventObservers
    State state;
    /// the buffer state for data from the GPS receiver
    State buffer;
    
	/** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    virtual void newData( const GPResult * point, const char * line, void * userData );

protected:
	/// protected constructor so it is only accessible by the module
	GPSGarminAltitude() {};

	friend class GPSModule;
};

inline void GPSGarminAltitude::newData( const GPResult * res, const char * line, void * userData )
{
    assert( userData != NULL );
    if( res->type == GPResult::PGRMZ){
        PGRMZ * point = (PGRMZ *) res;
        GPSModule * module = (GPSModule *)userData;
        if( !module->driver->hasFix() )
            return;
        module->lock();
        buffer.timeStamp();
        buffer.position[0] = 0;
        // 1 feet = 0.3048 meter, by google
        buffer.position[1] = point->altitude * 0.3048;
        buffer.position[0] = 0;
        buffer.confidence = 1 / module->driver->getHdop();
        module->unlock();
    }
}

#endif // !defined(_GPSGARMINALTITUDE_H)
