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
/** header file for GPSDirectionSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/GPSDirectionSource.h,v 1.3 2003/07/02 07:28:37 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section gpsdirectionsource GPSDirectionSource
 * The GPSDirectionSource node is a simple EventGenerator that outputs GPS 
 * direction and velocity data. The direction is encoded in the orientation 
 * field as a mathematically positive rotation around the Y axis starting from 
 * the X axis. The velocity is encoded in the X component of the position field
 * in meters / second. It is managed by the @ref gpsmodule, see there for
 * more information on how to configure GPS support.
 *
 * An example element looks like this :
 * @verbatim
<GPSDirectionSource/>@endverbatim
 */

#ifndef _GPSDIRECTIONSOURCE_H
#define _GPSDIRECTIONSOURCE_H

#include "../OpenTracker.h"
#include "GPSDriver.h"

/**
 * A very simple EventGenerator node for outputing GPS direction and velocity data.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API GPSDirectionSource : public Node, public GPSListener  
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
	GPSDirectionSource() {};

	friend class GPSModule;
};

inline void GPSDirectionSource::newData( const GPResult * res, const char * line, void * userData )
{
    assert( userData != NULL );
    if( res->type == GPResult::GPVTG){
        GPVTG * point = (GPVTG *) res;
        GPSModule * module = (GPSModule *)userData;
        if( !module->driver->hasFix() )
            return;
        module->lock();
        buffer.timeStamp();
        // klm/h = 3.6 * m/s, 1/3.6 = 0.27777777777777777777777777777778
        buffer.position[0] = point->speedKlm * 0.27777777777777777777777777777778;
        float temp[4];
        temp[0] = 0;
        temp[1] = 1;
        temp[2] = 0;
        temp[3] = point->trueCourse * MathUtils::GradToRad;
        MathUtils::axisAngleToQuaternion( temp, buffer.orientation );
        buffer.confidence = 1 / module->driver->getHdop();
        module->unlock();
    }
}

#endif // !defined(_GPSDIRECTIONSOURCE_H)
