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
/** header file for GPSSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSSource.h,v 1.4 2004/02/20 08:59:07 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpssource GPSSource
 * The GPSSource node is a simple EventGenerator that outputs GPS position data encoded
 * in the position part of the event. This data is in Latitue, Longitute, Height 
 * in the WGS84 coordinate system. It is managed by the @ref gpsmodule, see there for
 * more information on how to configure GPS support.
 * Use a filter after that to transform the data into your local coordinate system. The node
 * has no further attributes besides the ID attribute.
 *
 * An example element looks like this :
 * @verbatim
<GPSSource/>@endverbatim
 */

#ifndef _GPSSOURCE_H
#define _GPSSOURCE_H

#include <assert.h>

#include "../OpenTracker.h"
#include "GPSDriver.h"

/**
 * A very simple EventGenerator node for outputing GPS position data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API GPSSource : public Node, public GPSListener  
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
	GPSSource() {};

	friend class GPSModule;
};

inline void GPSSource::newData( const GPResult * res, const char * line, void * userData )
{
    assert( userData != NULL );
    if( res->type == GPResult::GPGGA){
        GPGGA * point = (GPGGA *) res;
        if( point->fix == 0)
            return;
        GPSModule * module = (GPSModule *)userData;
        module->lock();
        buffer.timeStamp();
        buffer.position[0] = point->lat * MathUtils::GradToRad;
        buffer.position[1] = point->lon * MathUtils::GradToRad;
        buffer.position[2] = point->altitude;
        buffer.confidence = 1 / point->hdop;
        module->unlock();
    }
}

#endif // !defined(_GPSSOURCE_H)
